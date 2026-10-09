package web

import (
	"encoding/json"
	"net/http"
	"tictac3/internal/application"
)

type AuthHandler struct {
	service *application.AuthService
}

func NewAuthHandler(a *application.AuthService) *AuthHandler {
	return &AuthHandler{
		service: a,
	}
}

func (h *AuthHandler) SignIn(w http.ResponseWriter, r *http.Request) {
	if r.Method != http.MethodPost {
		http.Error(w, "Wrong Method", http.StatusMethodNotAllowed)
		return
	}

	var jwtRequest application.JwtRequest
	err := json.NewDecoder(r.Body).Decode(&jwtRequest)
	if err != nil {
		http.Error(w, "Bad JSON", http.StatusBadRequest)
		return
	}

	jwtResponse, err := h.service.SignIn(jwtRequest)
	if err != nil {
		http.Error(w, "Unauthorized", http.StatusUnauthorized)
		return
	}

	w.Header().Set("Content-Type", "application/json")
	err = json.NewEncoder(w).Encode(jwtResponse)
	if err != nil {
		http.Error(w, err.Error(), http.StatusInternalServerError)
		return
	}
}

func (h *AuthHandler) SignUp(w http.ResponseWriter, r *http.Request) {
	if r.Method != http.MethodPost {
		http.Error(w, "Wrong Method", http.StatusMethodNotAllowed)
		return
	}
	var model application.SignUpRequest
	err := json.NewDecoder(r.Body).Decode(&model)
	if err != nil {
		http.Error(w, "Bad JSON", http.StatusBadRequest)
		return
	}
	flag, err := h.service.SignUp(model)
	if err != nil {
		http.Error(w, err.Error(), http.StatusInternalServerError)
		return
	}
	w.Header().Set("Content-Type", "application/json")
	err = json.NewEncoder(w).Encode(flag)
	if err != nil {
		http.Error(w, err.Error(), http.StatusInternalServerError)
		return
	}
}

func (h *AuthHandler) RefreshAccessToken(w http.ResponseWriter, r *http.Request) {
	if r.Method != http.MethodPost {
		http.Error(w, "Wrong Method", http.StatusMethodNotAllowed)
		return
	}

	var request application.RefreshJwtRequest
	err := json.NewDecoder(r.Body).Decode(&request)
	if err != nil {
		http.Error(w, "Bad Json", http.StatusBadRequest)
		return
	}

	jwtResponse, err := h.service.RefreshAccessToken(request.RefreshToken)
	if err != nil {
		http.Error(w, "Unauthorized", http.StatusUnauthorized)
		return
	}

	w.Header().Set("Content-Type", "application/json")
	err = json.NewEncoder(w).Encode(jwtResponse)
	if err != nil {
		http.Error(w, err.Error(), http.StatusInternalServerError)
		return
	}
}

func (h *AuthHandler) RefreshRefreshToken(w http.ResponseWriter, r *http.Request) {
	if r.Method != http.MethodPost {
		http.Error(w, "Wrong Method", http.StatusMethodNotAllowed)
		return
	}

	var request application.RefreshJwtRequest
	err := json.NewDecoder(r.Body).Decode(&request)
	if err != nil {
		http.Error(w, "Wrong Json", http.StatusBadRequest)
		return
	}

	jwtResponse, err := h.service.RefreshRefreshToken(request.RefreshToken)
	if err != nil {
		http.Error(w, "Unauthorized", http.StatusUnauthorized)
		return
	}

	w.Header().Set("Content-Type", "application/json")
	err = json.NewEncoder(w).Encode(jwtResponse)
	if err != nil {
		http.Error(w, err.Error(), http.StatusInternalServerError)
	}
}
