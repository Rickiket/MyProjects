package api

import (
	"Lizaweb/internal/application"
	"net/http"
	"strings"
)

type Authenticator struct {
	Service application.UserService
	Limiter *LoginLimiter
}

func NewAuthenticator(s application.UserService, limiter *LoginLimiter) *Authenticator {
	return &Authenticator{Service: s, Limiter: limiter}
}

func (auth *Authenticator) Autentificate(next http.Handler) http.Handler {
	return http.HandlerFunc(func(w http.ResponseWriter, r *http.Request) {
		ip := clientIP(r)
		if remaining, blocked := auth.Limiter.IsBlocked(ip); blocked {
			writeBlockedResponse(w, remaining)
			return
		}

		authheader := r.Header.Get("Authorization")
		if authheader == "" {
			auth.Limiter.RegisterFailure(ip)
			http.Error(w, "Authorization header required", http.StatusUnauthorized)
			return
		}

		prefix := "Basic "
		if !strings.HasPrefix(authheader, prefix) {
			auth.Limiter.RegisterFailure(ip)
			http.Error(w, "Invalid authorization header", http.StatusUnauthorized)
			return
		}
		basecode := strings.TrimPrefix(authheader, prefix)

		err := auth.Service.SignIn(basecode)
		if err != nil {
			auth.Limiter.RegisterFailure(ip)
			http.Error(w, "Invalid Login or Password", http.StatusUnauthorized)
			return
		}
		auth.Limiter.RegisterSuccess(ip)

		next.ServeHTTP(w, r)
	})
}
