package api

import (
	"Lizaweb/internal/application"
	"bytes"
	"encoding/base64"
	"encoding/json"
	"fmt"
	"github.com/google/uuid"
	"net/http"
	"net/http/httptest"
	"os"
	"path/filepath"
	"testing"
	"time"
)

func chunkTestServer(t *testing.T) http.Handler {
	t.Helper()
	t.Setenv("PHOTO_STORAGE_DIR", filepath.Join(t.TempDir(), "uploads"))
	t.Setenv("SITE_ADMIN_LOGIN", "test")
	t.Setenv("SITE_ADMIN_PASSWORD", "password")
	return newChunkTestServer()
}
func newChunkTestServer() http.Handler {
	limiter := NewLoginLimiter()
	user := application.NewUserService()
	return NewServer(NewAuthenticator(user, limiter), NewUserHandler(user, limiter), NewContentHandler(application.NewContentService())).Handler
}
func chunkCall(t *testing.T, h http.Handler, method, path string, body []byte, status int) *httptest.ResponseRecorder {
	t.Helper()
	r := httptest.NewRequest(method, path, bytes.NewReader(body))
	r.Header.Set("Authorization", "Basic "+base64.StdEncoding.EncodeToString([]byte("test:password")))
	w := httptest.NewRecorder()
	h.ServeHTTP(w, r)
	if w.Code != status {
		t.Fatalf("%s %s: got %d want %d: %s", method, path, w.Code, status, w.Body.String())
	}
	return w
}
func beginChunkTest(t *testing.T, h http.Handler, size int64) (string, []byte) {
	id := uuid.NewString()
	data, _ := json.Marshal(uploadSession{Filename: "photo.jpeg", Size: size})
	chunkCall(t, h, "PUT", "/photo/uploads/"+id, data, 201)
	return id, data
}
func TestChunk12MiBRestartAndRetries(t *testing.T) {
	h := chunkTestServer(t)
	original := bytes.Repeat([]byte("photobytes12"), (12<<20)/12)
	id, meta := beginChunkTest(t, h, int64(len(original)))
	base := "/photo/uploads/" + id
	chunkCall(t, h, "PUT", base, meta, 200)
	chunkCall(t, h, "POST", base+"/complete", nil, 409)
	for offset, index := 0, 0; offset < len(original); offset, index = offset+chunkBytes, index+1 {
		data := original[offset:min(offset+chunkBytes, len(original))]
		path := fmt.Sprintf("%s/chunks/%d", base, index)
		chunkCall(t, h, "PUT", path, data, 204)
		if index == 0 {
			h = newChunkTestServer()
			chunkCall(t, h, "PUT", path, data, 204)
		}
	}
	first := chunkCall(t, h, "POST", base+"/complete", nil, 200)
	h = newChunkTestServer()
	second := chunkCall(t, h, "POST", base+"/complete", nil, 200)
	if first.Body.String() != second.Body.String() {
		t.Fatal("completion not idempotent")
	}
	got := chunkCall(t, h, "GET", "/uploads/"+id+".jpeg", nil, 200)
	if !bytes.Equal(got.Body.Bytes(), original) {
		t.Fatal("original bytes changed")
	}
	photos, _ := application.NewContentService().GetPhotos()
	if len(photos) != 1 {
		t.Fatal("duplicate photo")
	}
	if _, err := os.Stat(filepath.Join(sessionPath(id), "data")); !os.IsNotExist(err) {
		t.Fatal("temporary data retained")
	}
}
func TestChunkValidationAndQuota(t *testing.T) {
	h := chunkTestServer(t)
	id, _ := beginChunkTest(t, h, chunkBytes+5)
	base := "/photo/uploads/" + id
	chunkCall(t, h, "PUT", base+"/chunks/1", []byte("12345"), 409)
	chunkCall(t, h, "PUT", base+"/chunks/0", []byte("short"), 400)
	chunkCall(t, h, "PUT", base+"/chunks/0", make([]byte, chunkBytes+1), 413)
	chunkCall(t, h, "PUT", base+"/chunks/0", make([]byte, chunkBytes), 204)
	changed := make([]byte, chunkBytes)
	changed[0] = 1
	chunkCall(t, h, "PUT", base+"/chunks/0", changed, 409)
	// The unfinished reservation must count against the shared quota.
	os.MkdirAll(application.StorageDir(), 0755)
	f, err := os.Create(filepath.Join(application.StorageDir(), "quota"))
	if err != nil {
		t.Fatal(err)
	}
	if err = f.Truncate(maxPhotoStorageBytes - chunkBytes); err != nil {
		t.Fatal(err)
	}
	f.Close()
	meta, _ := json.Marshal(uploadSession{Filename: "new.jpg", Size: 1})
	chunkCall(t, h, "PUT", "/photo/uploads/"+uuid.NewString(), meta, 507)
	// Temporary data isn't available through the public file route.
	chunkCall(t, h, "GET", "/uploads/"+id+"/data", nil, 404)
	r := httptest.NewRequest("PUT", base+"/chunks/1", bytes.NewReader([]byte("12345")))
	w := httptest.NewRecorder()
	h.ServeHTTP(w, r)
	if w.Code != 401 {
		t.Fatal("chunk route not authenticated")
	}
	old := time.Now().Add(-25 * time.Hour)
	os.Chtimes(filepath.Join(sessionPath(id), "meta.json"), old, old)
	NewContentHandler(application.NewContentService()).CleanupUploads()
	if _, err = os.Stat(sessionPath(id)); !os.IsNotExist(err) {
		t.Fatal("expired upload retained")
	}
}
func Test65ChunkPhotos(t *testing.T) {
	h := chunkTestServer(t)
	for i := 0; i < 65; i++ {
		id, _ := beginChunkTest(t, h, 3)
		base := "/photo/uploads/" + id
		chunkCall(t, h, "PUT", base+"/chunks/0", []byte("abc"), 204)
		chunkCall(t, h, "POST", base+"/complete", nil, 200)
	}
	photos, _ := application.NewContentService().GetPhotos()
	if len(photos) != 65 {
		t.Fatalf("got %d photos", len(photos))
	}
}

func TestChunkRecoversPartialDiskWrite(t *testing.T) {
	h := chunkTestServer(t)
	id, _ := beginChunkTest(t, h, chunkBytes)
	data := bytes.Repeat([]byte("x"), chunkBytes)
	// Simulate termination after part of a chunk was written, before any reply.
	if err := os.WriteFile(filepath.Join(sessionPath(id), "data"), data[:123], 0600); err != nil {
		t.Fatal(err)
	}
	base := "/photo/uploads/" + id
	chunkCall(t, h, "PUT", base+"/chunks/0", data, 204)
	chunkCall(t, h, "POST", base+"/complete", nil, 200)
	got := chunkCall(t, h, "GET", "/uploads/"+id+".jpeg", nil, 200)
	if !bytes.Equal(data, got.Body.Bytes()) {
		t.Fatal("partial write recovery changed bytes")
	}
}

func TestChunkMaximumPhotoPartialTail(t *testing.T) {
	h := chunkTestServer(t)
	id, _ := beginChunkTest(t, h, maxPhotoBytes)
	base := "/photo/uploads/" + id
	for offset, index := int64(0), 0; offset < maxPhotoBytes; offset, index = offset+chunkBytes, index+1 {
		chunkCall(t, h, "PUT", fmt.Sprintf("%s/chunks/%d", base, index), make([]byte, min(int64(chunkBytes), maxPhotoBytes-offset)), 204)
	}
	chunkCall(t, h, "POST", base+"/complete", nil, 200)
	info, err := os.Stat(filepath.Join(application.StorageDir(), id+".jpeg"))
	if err != nil {
		t.Fatal(err)
	}
	if info.Size() != maxPhotoBytes {
		t.Fatalf("size=%d", info.Size())
	}
}
