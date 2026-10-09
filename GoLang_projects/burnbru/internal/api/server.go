package api

import (
	"Lizaweb/internal/application"
	"context"
	"embed"
	"fmt"
	"io/fs"
	"net/http"
	"time"

	"go.uber.org/fx"
)

//go:embed static/*
var staticFiles embed.FS

func NewServer(auth *Authenticator, uh *UserHandler, ch *ContentHandler) *http.Server {
	mux := http.NewServeMux()
	mux.HandleFunc("POST /signin", uh.SignIn)
	mux.HandleFunc("GET /about", ch.GetAbout)
	mux.Handle("PUT /about", auth.Autentificate(http.HandlerFunc(ch.UpdateAbout)))
	mux.HandleFunc("GET /contacts", ch.GetContacts)
	mux.Handle("PUT /contacts", auth.Autentificate(http.HandlerFunc(ch.UpdateContacts)))
	mux.HandleFunc("GET /photo", ch.GetPhotos)
	mux.Handle("PUT /photo/uploads/{id}", auth.Autentificate(http.HandlerFunc(ch.BeginUpload)))
	mux.Handle("PUT /photo/uploads/{id}/chunks/{index}", auth.Autentificate(http.HandlerFunc(ch.PutChunk)))
	mux.Handle("POST /photo/uploads/{id}/complete", auth.Autentificate(http.HandlerFunc(ch.CompleteUpload)))
	mux.Handle("POST /photo", auth.Autentificate(http.HandlerFunc(ch.UploadPhoto)))
	mux.Handle("DELETE /photo/{filename}", auth.Autentificate(http.HandlerFunc(ch.DeletePhoto)))
	mux.Handle("GET /uploads/", http.StripPrefix("/uploads/", http.FileServer(http.Dir(application.StorageDir()))))

	staticRoot, err := fs.Sub(staticFiles, "static")
	if err != nil {
		panic(fmt.Sprintf("load embedded frontend: %v", err))
	}
	mux.Handle("GET /", http.FileServer(http.FS(staticRoot)))

	return &http.Server{
		Addr:              ":8080",
		ReadHeaderTimeout: 10 * time.Second,
		ReadTimeout:       5 * time.Minute,
		WriteTimeout:      6 * time.Minute,
		IdleTimeout:       60 * time.Second,
		Handler:           mux,
	}
}

func StartServer(lc fx.Lifecycle, server *http.Server, ch *ContentHandler) {
	stopCleanup := make(chan struct{})
	lc.Append(fx.Hook{
		OnStart: func(ctx context.Context) error {
			ch.CleanupUploads()
			go func() {
				ticker := time.NewTicker(time.Hour)
				defer ticker.Stop()
				for {
					select {
					case <-ticker.C:
						ch.CleanupUploads()
					case <-stopCleanup:
						return
					}
				}
			}()
			go func() {
				err := server.ListenAndServe()
				if err != nil && err != http.ErrServerClosed {
					fmt.Println("Server error:", err)
				}
			}()
			return nil
		},
		OnStop: func(ctx context.Context) error {
			close(stopCleanup)
			return server.Shutdown(ctx)
		},
	})
}
