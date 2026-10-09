package application

import (
	"encoding/json"
	"errors"
	"image"
	_ "image/gif"
	_ "image/jpeg"
	_ "image/png"
	"io"
	"os"
	"path/filepath"
	"strings"

	"github.com/google/uuid"
	_ "golang.org/x/image/webp"
)

type ContentService interface {
	GetAbout() (About, error)
	UpdateAbout(inf About) error
	GetContacts() (Contacts, error)
	UpdateContacts(inf Contacts) error
	UploadPhoto(filename string, photo io.Reader) error
	GetPhotos() ([]Photo, error)
	DeletePhoto(filename string) error
}

type ContentServiceImpl struct{}

func NewContentService() *ContentServiceImpl { return &ContentServiceImpl{} }

func (service *ContentServiceImpl) GetAbout() (About, error) {
	databytes, err := readStorageFile("about_me.json")
	if err != nil {
		return About{}, err
	}
	if len(databytes) == 0 {
		return About{}, nil
	}

	var request = About{}
	err = json.Unmarshal(databytes, &request)
	if err != nil {
		return About{}, err
	}

	return request, nil
}

func (service *ContentServiceImpl) UpdateAbout(inf About) error {
	jsonbytes, err := json.MarshalIndent(inf, "", "    ")
	if err != nil {
		return err
	}

	if err := os.MkdirAll(StorageDir(), 0755); err != nil {
		return err
	}

	return os.WriteFile(filepath.Join(StorageDir(), "about_me.json"), jsonbytes, 0644)
}

func (service *ContentServiceImpl) GetContacts() (Contacts, error) {
	databytes, err := readStorageFile("contacts.json")
	if err != nil {
		return Contacts{}, err
	}

	if len(databytes) == 0 {
		return Contacts{}, nil
	}

	var request = Contacts{}
	err = json.Unmarshal(databytes, &request)
	if err != nil {
		return Contacts{}, err
	}

	return request, nil
}

func (service *ContentServiceImpl) UpdateContacts(inf Contacts) error {
	jsonbytes, err := json.MarshalIndent(inf, "", "    ")
	if err != nil {
		return err
	}

	if err := os.MkdirAll(StorageDir(), 0755); err != nil {
		return err
	}

	return os.WriteFile(filepath.Join(StorageDir(), "contacts.json"), jsonbytes, 0644)
}

func (service *ContentServiceImpl) UploadPhoto(filename string, photo io.Reader) error {
	extension := strings.ToLower(filepath.Ext(filename))
	switch extension {
	case ".jpg", ".jpeg", ".png", ".webp", ".gif":
	default:
		return errors.New("unsupported format file")
	}

	if err := os.MkdirAll(StorageDir(), 0755); err != nil {
		return err
	}

	// Publish only after the complete file has been written and closed.
	destination, err := os.CreateTemp(StorageDir(), ".upload-*")
	if err != nil {
		return err
	}
	defer os.Remove(destination.Name())
	if _, err = io.Copy(destination, photo); err != nil {
		destination.Close()
		return err
	}
	if err = destination.Close(); err != nil {
		return err
	}
	return os.Rename(destination.Name(), filepath.Join(StorageDir(), uuid.New().String()+extension))
}

func (service *ContentServiceImpl) GetPhotos() ([]Photo, error) {
	directory := StorageDir()

	if err := os.MkdirAll(directory, 0755); err != nil {
		return nil, err
	}

	files, err := os.ReadDir(directory)
	if err != nil {
		return nil, err
	}

	photos := make([]Photo, 0, len(files))

	for _, i := range files {
		if i.IsDir() {
			continue
		}

		filename := i.Name()
		extension := strings.ToLower(filepath.Ext(filename))

		switch extension {
		case ".jpg", ".jpeg", ".png", ".webp", ".gif":
		default:
			continue
		}

		photo := Photo{
			Filename: filename,
			URL:      "/uploads/" + filename,
		}
		file, openErr := os.Open(filepath.Join(directory, filename))
		if openErr == nil {
			config, _, decodeErr := image.DecodeConfig(file)
			file.Close()
			if decodeErr == nil {
				photo.Width = config.Width
				photo.Height = config.Height
			}
		}
		photos = append(photos, photo)
	}

	return photos, nil
}

func (service *ContentServiceImpl) DeletePhoto(filename string) error {
	if strings.EqualFold(filepath.Ext(filename), ".json") {
		return errors.New("JSON files cannot be deleted as photos")
	}

	return os.Remove(filepath.Join(StorageDir(), filename))
}

func StorageDir() string {
	dir := strings.TrimSpace(os.Getenv("PHOTO_STORAGE_DIR"))
	if dir != "" {
		return dir
	}

	return "uploads"
}

func readStorageFile(filename string) ([]byte, error) {
	data, err := os.ReadFile(filepath.Join(StorageDir(), filename))
	if err == nil || !errors.Is(err, os.ErrNotExist) {
		return data, err
	}

	return os.ReadFile(filepath.Join("uploads", filename))
}
