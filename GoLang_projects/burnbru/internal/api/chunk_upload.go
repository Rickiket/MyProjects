package api

import (
	"Lizaweb/internal/application"
	"bytes"
	"encoding/json"
	"errors"
	"fmt"
	"github.com/google/uuid"
	"io"
	"log"
	"net/http"
	"os"
	"path/filepath"
	"strconv"
	"strings"
	"time"
)

const chunkBytes = 9 << 20
const uploadTTL = 24 * time.Hour
const maxUploadSessions = 128

type uploadSession struct {
	Filename string `json:"filename"`
	Size     int64  `json:"size"`
}

// A sibling directory keeps unfinished files outside the public file server.
func chunkRoot() string            { return filepath.Clean(application.StorageDir()) + ".upload-tmp" }
func sessionPath(id string) string { return filepath.Join(chunkRoot(), id) }
func validUploadID(id string) bool {
	parsed, err := uuid.Parse(id)
	return err == nil && parsed.String() == id
}
func finalPhoto(id string, s uploadSession) string {
	return filepath.Join(application.StorageDir(), id+strings.ToLower(filepath.Ext(s.Filename)))
}

// Caller holds uploadMu. Reservations include all incomplete uploads, so both
// upload APIs share the disk quota without double-counting received chunks.
func (ch *ContentHandler) reservedUploads() (int64, int, error) {
	entries, err := os.ReadDir(chunkRoot())
	if errors.Is(err, os.ErrNotExist) {
		return 0, 0, nil
	}
	if err != nil {
		return 0, 0, err
	}
	var reserved int64
	count := 0
	for _, entry := range entries {
		if !entry.IsDir() || !validUploadID(entry.Name()) {
			continue
		}
		dir := sessionPath(entry.Name())
		info, err := os.Stat(filepath.Join(dir, "meta.json"))
		if err != nil {
			return 0, 0, err
		}
		if time.Since(info.ModTime()) > uploadTTL {
			if err = os.RemoveAll(dir); err != nil {
				return 0, 0, err
			}
			continue
		}
		var s uploadSession
		data, err := os.ReadFile(filepath.Join(dir, "meta.json"))
		if err != nil {
			return 0, 0, err
		}
		if err = json.Unmarshal(data, &s); err != nil {
			return 0, 0, err
		}
		// A completed receipt is retained for idempotent completion retries.
		if _, err = os.Stat(filepath.Join(dir, "data")); err == nil {
			reserved += s.Size
			count++
		} else if !errors.Is(err, os.ErrNotExist) {
			return 0, 0, err
		}
	}
	return reserved, count, nil
}

func (ch *ContentHandler) CleanupUploads() {
	ch.uploadMu.Lock()
	defer ch.uploadMu.Unlock()
	if _, _, err := ch.reservedUploads(); err != nil {
		log.Printf("cleanup uploads: %v", err)
	}
}

func (ch *ContentHandler) BeginUpload(w http.ResponseWriter, r *http.Request) {
	id := r.PathValue("id")
	if !validUploadID(id) {
		http.Error(w, "Неверный uploadId", 400)
		return
	}
	var s uploadSession
	r.Body = http.MaxBytesReader(w, r.Body, 4096)
	dec := json.NewDecoder(r.Body)
	if err := dec.Decode(&s); err != nil {
		http.Error(w, "Неверные параметры", 400)
		return
	}
	if s.Size <= 0 || s.Size > maxPhotoBytes {
		http.Error(w, "Максимальный размер фото 32 МиБ", 413)
		return
	}
	switch strings.ToLower(filepath.Ext(s.Filename)) {
	case ".jpg", ".jpeg", ".png", ".webp", ".gif":
	default:
		http.Error(w, "Неподдерживаемый формат", 415)
		return
	}
	ch.uploadMu.Lock()
	defer ch.uploadMu.Unlock()
	reserved, count, err := ch.reservedUploads()
	if err != nil {
		http.Error(w, "Ошибка хранилища", 500)
		return
	}
	dir := sessionPath(id)
	if data, err := os.ReadFile(filepath.Join(dir, "meta.json")); err == nil {
		var existing uploadSession
		if json.Unmarshal(data, &existing) != nil || existing != s {
			http.Error(w, "uploadId уже используется", 409)
			return
		}
		w.WriteHeader(200)
		return
	}
	if count >= maxUploadSessions {
		http.Error(w, "Слишком много незавершённых загрузок", 429)
		return
	}
	used, err := directorySize(application.StorageDir())
	if err != nil {
		http.Error(w, "Ошибка хранилища", 500)
		return
	}
	if used+reserved+s.Size > maxPhotoStorageBytes {
		http.Error(w, "Недостаточно места для фото", 507)
		return
	}
	if err = os.MkdirAll(dir, 0700); err != nil {
		http.Error(w, "Ошибка хранилища", 500)
		return
	}
	ok := false
	defer func() {
		if !ok {
			os.RemoveAll(dir)
		}
	}()
	if err = os.WriteFile(filepath.Join(dir, "data"), nil, 0600); err != nil {
		http.Error(w, "Ошибка хранилища", 500)
		return
	}
	data, _ := json.Marshal(s)
	if err = os.WriteFile(filepath.Join(dir, "meta.json"), data, 0600); err != nil {
		http.Error(w, "Ошибка хранилища", 500)
		return
	}
	ok = true
	w.WriteHeader(201)
}

func loadSession(id string) (uploadSession, error) {
	var s uploadSession
	if !validUploadID(id) {
		return s, os.ErrNotExist
	}
	path := filepath.Join(sessionPath(id), "meta.json")
	info, err := os.Stat(path)
	if err != nil {
		return s, err
	}
	if time.Since(info.ModTime()) > uploadTTL {
		return s, os.ErrNotExist
	}
	data, err := os.ReadFile(path)
	if err != nil {
		return s, err
	}
	err = json.Unmarshal(data, &s)
	return s, err
}

func (ch *ContentHandler) PutChunk(w http.ResponseWriter, r *http.Request) {
	index, err := strconv.ParseInt(r.PathValue("index"), 10, 64)
	if err != nil || index < 0 || index >= (maxPhotoBytes+chunkBytes-1)/chunkBytes {
		http.Error(w, "Неверный номер части", 400)
		return
	}
	r.Body = http.MaxBytesReader(w, r.Body, chunkBytes)
	data, err := io.ReadAll(r.Body)
	if err != nil {
		http.Error(w, "Неполная или слишком большая часть", 413)
		return
	}
	ch.uploadMu.Lock()
	defer ch.uploadMu.Unlock()
	id := r.PathValue("id")
	s, err := loadSession(id)
	if err != nil {
		http.Error(w, "Загрузка не найдена или истекла", 404)
		return
	}
	offset := index * chunkBytes
	expected := min(int64(chunkBytes), s.Size-offset)
	if expected <= 0 || int64(len(data)) != expected {
		http.Error(w, "Неверный размер части", 400)
		return
	}
	f, err := os.OpenFile(filepath.Join(sessionPath(id), "data"), os.O_RDWR, 0600)
	if err != nil {
		http.Error(w, "Загрузка уже завершена или недоступна", 409)
		return
	}
	defer f.Close()
	info, err := f.Stat()
	if err != nil {
		http.Error(w, "Ошибка диска", 500)
		return
	}
	if offset > info.Size() {
		http.Error(w, "Сначала отправьте предыдущую часть", 409)
		return
	}
	// A process may have stopped in the middle of a write. The client resends
	// the entire unacknowledged chunk, replacing that partial tail.
	if offset < info.Size() && info.Size() < offset+expected {
		if err = f.Truncate(offset); err != nil {
			http.Error(w, "Ошибка диска", 500)
			return
		}
	}
	if offset+expected <= info.Size() {
		previous := make([]byte, len(data))
		_, err = f.ReadAt(previous, offset)
		if err != nil || !bytes.Equal(previous, data) {
			http.Error(w, "Повторная часть отличается", 409)
			return
		}
	} else {
		if _, err = f.WriteAt(data, offset); err != nil {
			f.Truncate(offset)
			http.Error(w, "Ошибка записи", 500)
			return
		}
		if err = f.Sync(); err != nil {
			f.Truncate(offset)
			http.Error(w, "Ошибка диска", 500)
			return
		}
	}
	now := time.Now()
	os.Chtimes(filepath.Join(sessionPath(id), "meta.json"), now, now)
	w.WriteHeader(204)
}

func (ch *ContentHandler) CompleteUpload(w http.ResponseWriter, r *http.Request) {
	ch.uploadMu.Lock()
	defer ch.uploadMu.Unlock()
	id := r.PathValue("id")
	s, err := loadSession(id)
	if err != nil {
		http.Error(w, "Загрузка не найдена или истекла", 404)
		return
	}
	target := finalPhoto(id, s)
	if _, err = os.Stat(target); errors.Is(err, os.ErrNotExist) {
		source := filepath.Join(sessionPath(id), "data")
		info, err := os.Stat(source)
		if err != nil || info.Size() != s.Size {
			http.Error(w, "Получены не все части", 409)
			return
		}
		if err = os.MkdirAll(application.StorageDir(), 0755); err != nil {
			http.Error(w, "Ошибка диска", 500)
			return
		}
		if err = os.Rename(source, target); err != nil {
			http.Error(w, "Ошибка сохранения", 500)
			return
		}
	} else if err != nil {
		http.Error(w, "Ошибка диска", 500)
		return
	}
	w.Header().Set("Content-Type", "application/json")
	fmt.Fprintf(w, `{"filename":%q,"url":%q}`, filepath.Base(target), "/uploads/"+filepath.Base(target))
}
