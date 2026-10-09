package web

import (
	"context"
	"embed"
	"fmt"
	"io/fs"
	"net/http"

	"go.uber.org/fx"
)

//go:embed static/*
var staticFiles embed.FS

func NewServer(gh *GameHandler, ah *AuthHandler, auth *UserAuthenticator, us *UserHandler) *http.Server {
	mux := http.NewServeMux()
	mux.HandleFunc("POST /signup", ah.SignUp)
	mux.HandleFunc("POST /signin", ah.SignIn)

	mux.HandleFunc("POST /refresh/access", ah.RefreshAccessToken)
	mux.HandleFunc("POST /refresh/refresh", ah.RefreshRefreshToken)

	mux.Handle("POST /game", auth.Authenticate(http.HandlerFunc(gh.CreateGame)))
	mux.Handle("POST /game/{UUID}", auth.Authenticate(http.HandlerFunc(gh.MakeMove)))
	mux.Handle("POST /game/{UUID}/join", auth.Authenticate(http.HandlerFunc(gh.JoinGame)))
	mux.Handle("GET /game", auth.Authenticate(http.HandlerFunc(gh.GetAvailableGames)))
	mux.Handle("GET /game/{UUID}", auth.Authenticate(http.HandlerFunc(gh.GetGame)))
	mux.Handle("GET /games/completed", auth.Authenticate(http.HandlerFunc(gh.GetCompletedGamesByUUIDPlayer)))

	mux.Handle("GET /user/{UUID}", auth.Authenticate(http.HandlerFunc(us.GetUserByUUID)))
	mux.Handle("GET /user", auth.Authenticate(http.HandlerFunc(us.GetMe)))
	mux.Handle("GET /user/top", auth.Authenticate(http.HandlerFunc(gh.GetNTopPlayers)))

	staticRoot, err := fs.Sub(staticFiles, "static")
	if err != nil {
		panic(fmt.Sprintf("load embedded frontend: %v", err))
	}
	mux.Handle("GET /", http.FileServer(http.FS(staticRoot)))

	return &http.Server{
		Addr:    ":8080",
		Handler: mux,
	}
}

func StartServer(lc fx.Lifecycle, server *http.Server) {
	lc.Append(fx.Hook{
		OnStart: func(ctx context.Context) error {
			go func() {
				err := server.ListenAndServe()
				if err != nil && err != http.ErrServerClosed {
					fmt.Println("Server error:", err)
				}
			}()
			return nil
		},
		OnStop: func(ctx context.Context) error {
			return server.Shutdown(ctx)
		},
	})
}
