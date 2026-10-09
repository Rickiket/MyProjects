package api

import (
	"Lizaweb/internal/application"
	"net/http"
	"strings"
)

type UserHandler struct {
	Service application.UserService
	Limiter *LoginLimiter
}

func NewUserHandler(s application.UserService, limiter *LoginLimiter) *UserHandler {
	return &UserHandler{Service: s, Limiter: limiter}
}

func (h *UserHandler) SignIn(w http.ResponseWriter, r *http.Request) {
	if r.Method != http.MethodPost {
		http.Error(w, "Wrong Method", http.StatusMethodNotAllowed)
		return
	}

	ip := clientIP(r)
	if remaining, blocked := h.Limiter.IsBlocked(ip); blocked {
		writeBlockedResponse(w, remaining)
		return
	}

	authheader := r.Header.Get("Authorization")
	if authheader == "" {
		h.Limiter.RegisterFailure(ip)
		http.Error(w, "Authorization header required", http.StatusUnauthorized)
		return
	}

	prefix := "Basic "
	if !strings.HasPrefix(authheader, prefix) {
		h.Limiter.RegisterFailure(ip)
		http.Error(w, "Invalid authorization header", http.StatusUnauthorized)
		return
	}
	basecode := strings.TrimPrefix(authheader, prefix)

	err := h.Service.SignIn(basecode)
	if err != nil {
		h.Limiter.RegisterFailure(ip)
		http.Error(w, "Invalid Login or Password", http.StatusUnauthorized)
		return
	}
	h.Limiter.RegisterSuccess(ip)
	w.WriteHeader(http.StatusOK)
}
