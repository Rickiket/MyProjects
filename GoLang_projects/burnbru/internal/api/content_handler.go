package api

import (
	"Lizaweb/internal/application"
	"encoding/json"
	"errors"
	"io/fs"
	"log"
	"net/http"
	"os"
	"path/filepath"
	"strings"
	"sync"
)

const maxPhotoStorageBytes int64 = 4_950_000_000
const maxPhotoBytes int64 = 32 << 20
const maxPhotoRequestBytes int64 = maxPhotoBytes + (1 << 20)

type ContentHandler struct {
	Service  application.ContentService
	uploadMu sync.Mutex
}

func NewContentHandler(s application.ContentService) *ContentHandler {
	return &ContentHandler{Service: s}
}

func (ch *ContentHandler) GetAbout(w http.ResponseWriter, r *http.Request) {
	if r.Method != http.MethodGet {
		http.Error(w, "Wrong Method", http.StatusMethodNotAllowed)
		return
	}

	inf, err := ch.Service.GetAbout()
	if err != nil {
		http.Error(w, err.Error(), http.StatusInternalServerError)
		return
	}

	w.Header().Set("Content-Type", "application/json")
	err = json.NewEncoder(w).Encode(inf)
	if err != nil {
		http.Error(w, err.Error(), http.StatusInternalServerError)
		return
	}
}

func (ch *ContentHandler) UpdateAbout(w http.ResponseWriter, r *http.Request) {
	if r.Method != http.MethodPut {
		http.Error(w, "Wrong Method", http.StatusMethodNotAllowed)
		return
	}

	var inf application.About
	err := json.NewDecoder(r.Body).Decode(&inf)
	if err != nil {
		http.Error(w, "Invalid JSON", http.StatusBadRequest)
		return
	}

	err = ch.Service.UpdateAbout(inf)
	if err != nil {
		http.Error(w, "Failed to update about", http.StatusInternalServerError)
		return
	}

	w.WriteHeader(http.StatusOK)
}

func (ch *ContentHandler) GetContacts(w http.ResponseWriter, r *http.Request) {
	if r.Method != http.MethodGet {
		http.Error(w, "Wrong Method", http.StatusMethodNotAllowed)
		return
	}

	inf, err := ch.Service.GetContacts()
	if err != nil {
		http.Error(w, "Failed to get contacts information", http.StatusInternalServerError)
		return
	}

	w.Header().Set("Content-Type", "application/json")
	err = json.NewEncoder(w).Encode(inf)
	if err != nil {
		http.Error(w, err.Error(), http.StatusInternalServerError)
		return
	}
}

func (ch *ContentHandler) UpdateContacts(w http.ResponseWriter, r *http.Request) {
	if r.Method != http.MethodPut {
		http.Error(w, "Wrong Method", http.StatusMethodNotAllowed)
		return
	}

	var inf application.Contacts
	err := json.NewDecoder(r.Body).Decode(&inf)
	if err != nil {
		http.Error(w, "Invalid JSON", http.StatusBadRequest)
		return
	}

	err = ch.Service.UpdateContacts(inf)
	if err != nil {
		http.Error(w, "Failed to update contacts", http.StatusInternalServerError)
		return
	}

	w.WriteHeader(http.StatusOK)
}

func (ch *ContentHandler) UploadPhoto(w http.ResponseWriter, r *http.Request) {
	if r.Method != http.MethodPost {
		http.Error(w, "Wrong Method", http.StatusMethodNotAllowed)
		return
	}

	r.Body = http.MaxBytesReader(w, r.Body, maxPhotoRequestBytes)
	err := r.ParseMultipartForm(1 << 20)
	if r.MultipartForm != nil {
		defer r.MultipartForm.RemoveAll()
	}
	if err != nil {
		var tooLarge *http.MaxBytesError
		if errors.As(err, &tooLarge) {
			http.Error(w, "Слишком большой запрос: максимум 33 МиБ", http.StatusRequestEntityTooLarge)
		} else {
			log.Printf("photo multipart: %v", err)
			http.Error(w, "Не удалось прочитать файл: повреждённый или прерванный запрос", http.StatusBadRequest)
		}
		return
	}

	files := r.MultipartForm.File["photos"]
	if len(files) != 1 {
		http.Error(w, "Отправляйте ровно одну фотографию за запрос", http.StatusBadRequest)
		return
	}

	var newPhotosSize int64
	for _, i := range files {
		if i.Size == 0 || i.Size > maxPhotoBytes {
			http.Error(w, "Размер фото должен быть от 1 байта до 32 МиБ", http.StatusRequestEntityTooLarge)
			return
		}
		switch strings.ToLower(filepath.Ext(i.Filename)) {
		case ".jpg", ".jpeg", ".png", ".webp", ".gif":
		default:
			http.Error(w, "Поддерживаются JPG, PNG, WEBP и GIF", http.StatusUnsupportedMediaType)
			return
		}
		newPhotosSize += i.Size
	}

	ch.uploadMu.Lock()
	defer ch.uploadMu.Unlock()

	currentSize, err := directorySize(application.StorageDir())
	if err != nil {
		http.Error(w, "Не удалось проверить занятое место", http.StatusInternalServerError)
		return
	}

	reserved, _, err := ch.reservedUploads()
	if err != nil {
		http.Error(w, "Ошибка хранилища", 500)
		return
	}
	currentSize += reserved
	if newPhotosSize > maxPhotoStorageBytes ||
		currentSize > maxPhotoStorageBytes-newPhotosSize {
		http.Error(w, "Невозможно загрузить: будет превышен лимит фотографий 5 ГБ", http.StatusInsufficientStorage)
		return
	}

	for _, fileHeader := range files {
		file, err := fileHeader.Open()
		if err != nil {
			http.Error(w, "Failed to open file", http.StatusBadRequest)
			return
		}
		err = ch.Service.UploadPhoto(fileHeader.Filename, file)
		file.Close()
		if err != nil {
			log.Printf("save photo: %v", err)
			http.Error(w, "Не удалось сохранить фото на диске", http.StatusInternalServerError)
			return
		}
	}

	w.WriteHeader(http.StatusCreated)
}

func (ch *ContentHandler) GetPhotos(w http.ResponseWriter, r *http.Request) {
	if r.Method != http.MethodGet {
		http.Error(w, "Wrong Method", http.StatusMethodNotAllowed)
		return
	}

	inf, err := ch.Service.GetPhotos()
	if err != nil {
		http.Error(w, "Failed to get photos", http.StatusInternalServerError)
		return
	}

	w.Header().Set("Content-Type", "application/json")
	err = json.NewEncoder(w).Encode(inf)
	if err != nil {
		http.Error(w, err.Error(), http.StatusInternalServerError)
		return
	}
}

func (ch *ContentHandler) DeletePhoto(w http.ResponseWriter, r *http.Request) {
	if r.Method != http.MethodDelete {
		http.Error(w, "Wrong Method", http.StatusMethodNotAllowed)
		return
	}

	filename := r.PathValue("filename")
	if filename == "" {
		http.Error(w, "Filename is required", http.StatusBadRequest)
		return
	}

	err := ch.Service.DeletePhoto(filename)
	if err != nil {
		http.Error(w, "Failed to delete photo", http.StatusInternalServerError)
		return
	}

	w.WriteHeader(http.StatusOK)
}

func directorySize(directory string) (int64, error) {
	var totalSize int64
	err := filepath.WalkDir(directory, func(_ string, fil fs.DirEntry, walkerr error) error {
		if walkerr != nil {
			return walkerr
		}

		if fil.IsDir() {
			return nil
		}

		inf, err := fil.Info()
		if err != nil {
			return err
		}
		totalSize += inf.Size()
		return nil
	})
	if errors.Is(err, os.ErrNotExist) {
		return 0, nil
	}

	return totalSize, err
}
