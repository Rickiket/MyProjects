package web

import (
	"context"
	"net/http"
	"strings"
	jwtprovider "tictac3/internal/infrastructure/jwt"
)

type UserAuthenticator struct {
	jwtProvider *jwtprovider.JwtProvider
}

func NewAuthenticator(jwtProvider *jwtprovider.JwtProvider) *UserAuthenticator {
	return &UserAuthenticator{
		jwtProvider: jwtProvider,
	}
}

func (a *UserAuthenticator) Authenticate(next http.Handler) http.Handler {
	return http.HandlerFunc(func(w http.ResponseWriter, r *http.Request) {
		authHeader := r.Header.Get("Authorization")
		if authHeader == "" {
			http.Error(w, "Authorization header required", http.StatusUnauthorized)
			return
		}

		prefix := "Bearer "
		if !strings.HasPrefix(authHeader, prefix) {
			http.Error(w, "Invalid authorization header", http.StatusUnauthorized)
			return
		}
		accessToken := strings.TrimPrefix(authHeader, prefix)

		err := a.jwtProvider.ValidateAccessToken(accessToken)
		if err != nil {
			http.Error(w, "Unauthorized", http.StatusUnauthorized)
			return
		}

		id, err := a.jwtProvider.GetUUIDFromToken(accessToken)
		if err != nil {
			http.Error(w, "Unauthorized", http.StatusUnauthorized)
			return
		}

		ctx := context.WithValue(r.Context(), "userID", id)

		next.ServeHTTP(w, r.WithContext(ctx))
	})
}
