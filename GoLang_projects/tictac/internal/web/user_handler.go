package web

import (
	"encoding/json"
	"net/http"
	"tictac3/internal/application"

	"github.com/google/uuid"
)

type UserHandler struct {
	service application.UserService
}

func NewUserHandler(s application.UserService) *UserHandler {
	return &UserHandler{service: s}
}

func (h *UserHandler) GetUserByUUID(w http.ResponseWriter, r *http.Request) {
	if r.Method != http.MethodGet {
		http.Error(w, "Wrong Method", http.StatusMethodNotAllowed)
		return
	}

	playerIDsring := r.PathValue("UUID")
	plaerID, err := uuid.Parse(playerIDsring)
	if err != nil {
		http.Error(w, "Invalid ID Player", http.StatusBadRequest)
		return
	}

	user, err := h.service.GetUserByUUID(plaerID)
	if err != nil {
		http.Error(w, err.Error(), http.StatusBadRequest)
		return
	}

	response := toUserEntity(*user)
	w.Header().Set("Content-Type", "application/json")
	err = json.NewEncoder(w).Encode(response)
	if err != nil {
		http.Error(w, err.Error(), http.StatusInternalServerError)
		return
	}
}

func (h *UserHandler) GetMe(w http.ResponseWriter, r *http.Request) {
	if r.Method != http.MethodGet {
		http.Error(w, "Wrong Method", http.StatusMethodNotAllowed)
		return
	}

	id, ok := r.Context().Value("userID").(uuid.UUID)
	if !ok {
		http.Error(w, "Unauthorized", http.StatusUnauthorized)
		return
	}

	user, err := h.service.GetUserByUUID(id)
	if err != nil {
		http.Error(w, "User not found", http.StatusNotFound)
		return
	}

	response := toUserEntity(*user)

	w.Header().Set("Content-Type", "application/json")
	err = json.NewEncoder(w).Encode(response)
	if err != nil {
		http.Error(w, err.Error(), http.StatusInternalServerError)
		return
	}
}
