package web

import (
	"encoding/json"
	"net/http"
	"strconv"
	"tictac3/internal/application"

	"github.com/google/uuid"
)

type GameHandler struct {
	service application.GameService
}

func NewGameHandler(s application.GameService) *GameHandler {
	return &GameHandler{
		service: s,
	}
}

func (h *GameHandler) MakeMove(w http.ResponseWriter, r *http.Request) {
	if r.Method != http.MethodPost {
		http.Error(w, "Wrong Method", http.StatusMethodNotAllowed)
		return
	}
	idString := r.PathValue("UUID")
	id, err := uuid.Parse(idString)

	if err != nil {
		http.Error(w, "Invalid uuid", http.StatusBadRequest)
		return
	}
	var request GameEntity
	err = json.NewDecoder(r.Body).Decode(&request)
	if err != nil {
		http.Error(w, "Bad JSON", 400)
		return
	}
	game := toDomain(request)
	result, err := h.service.MakeMove(id, game.GameBoard)
	if err != nil {
		http.Error(w, err.Error(), 400)
		return
	}
	response := toEntity(result)
	w.Header().Set("Content-Type", "application/json")
	json.NewEncoder(w).Encode(response)
}

func (h *GameHandler) CreateGame(w http.ResponseWriter, r *http.Request) {
	if r.Method != http.MethodPost {
		http.Error(w, "Wrong Method", http.StatusMethodNotAllowed)
		return
	}

	var request GameEntity
	err := json.NewDecoder(r.Body).Decode(&request)
	if err != nil {
		http.Error(w, "Bad JSON", 400)
		return
	}

	userId, ok := r.Context().Value("userID").(uuid.UUID)
	if ok != true {
		http.Error(w, "Unauthorized", http.StatusUnauthorized)
		return
	}

	game, err := h.service.CreateGame(userId, request.GameType)
	if err != nil {
		http.Error(w, err.Error(), 500)
		return
	}

	response := toEntity(*game)
	w.Header().Set("Content-Type", "application/json")
	err = json.NewEncoder(w).Encode(response)
	if err != nil {
		http.Error(w, err.Error(), http.StatusInternalServerError)
		return
	}
}

func (h *GameHandler) GetAvailableGames(w http.ResponseWriter, r *http.Request) {
	if r.Method != http.MethodGet {
		http.Error(w, "Wrong Method", http.StatusMethodNotAllowed)
		return
	}

	uuids, err := h.service.GetAvailableGames()
	if err != nil {
		http.Error(w, "Internal Server Error", http.StatusInternalServerError)
		return
	}

	var ids []string
	for _, i := range uuids {
		id := i.String()
		ids = append(ids, id)
	}

	w.Header().Set("Content-Type", "application/json")
	err = json.NewEncoder(w).Encode(ids)
	if err != nil {
		http.Error(w, err.Error(), http.StatusInternalServerError)
		return
	}
}

func (h *GameHandler) JoinGame(w http.ResponseWriter, r *http.Request) {
	if r.Method != http.MethodPost {
		http.Error(w, "Wrong Method", http.StatusMethodNotAllowed)
		return
	}

	playerID, ok := r.Context().Value("userID").(uuid.UUID)
	if ok != true {
		http.Error(w, "Unauthorized", http.StatusUnauthorized)
		return
	}

	gameIDstring := r.PathValue("UUID")
	gameID, err := uuid.Parse(gameIDstring)
	if err != nil {
		http.Error(w, "Invalid ID Game", http.StatusBadRequest)
		return
	}

	game, err := h.service.JoinGame(gameID, playerID)
	if err != nil {
		http.Error(w, err.Error(), http.StatusBadRequest)
		return
	}

	w.Header().Set("Content-Type", "application/json")
	err = json.NewEncoder(w).Encode(game)
	if err != nil {
		http.Error(w, err.Error(), http.StatusInternalServerError)
		return
	}
}

func (h *GameHandler) GetGame(w http.ResponseWriter, r *http.Request) {
	if r.Method != http.MethodGet {
		http.Error(w, "Wrong Method", http.StatusMethodNotAllowed)
		return
	}

	gameIDstring := r.PathValue("UUID")
	gameID, err := uuid.Parse(gameIDstring)
	if err != nil {
		http.Error(w, "Invalid ID Game", http.StatusBadRequest)
		return
	}

	game, err := h.service.GetGame(gameID)
	if err != nil {
		http.Error(w, err.Error(), http.StatusBadRequest)
		return
	}

	response := toEntity(game)
	w.Header().Set("Content-Type", "application/json")
	err = json.NewEncoder(w).Encode(response)
	if err != nil {
		http.Error(w, err.Error(), http.StatusInternalServerError)
		return
	}
}

func (h *GameHandler) GetCompletedGamesByUUIDPlayer(w http.ResponseWriter, r *http.Request) {
	if r.Method != http.MethodGet {
		http.Error(w, "Wrong Method", http.StatusMethodNotAllowed)
		return
	}

	playerId, ok := r.Context().Value("userID").(uuid.UUID)
	if !ok {
		http.Error(w, "Unauthorized", http.StatusUnauthorized)
		return
	}

	completedGames, err := h.service.GetCompletedGamesByUUIDPlayer(playerId)
	if err != nil {
		http.Error(w, err.Error(), http.StatusBadRequest)
		return
	}

	w.Header().Set("Content-Type", "application/json")
	if err = json.NewEncoder(w).Encode(completedGames); err != nil {
		http.Error(w, err.Error(), http.StatusInternalServerError)
		return
	}

}

func (h *GameHandler) GetNTopPlayers(w http.ResponseWriter, r *http.Request) {
	if r.Method != http.MethodGet {
		http.Error(w, "Wrong Method", http.StatusMethodNotAllowed)
		return
	}

	limitString := r.URL.Query().Get("limit")
	limit, err := strconv.Atoi(limitString)
	if err != nil {
		http.Error(w, "Invalid limit", http.StatusBadRequest)
		return
	}
	if limit <= 0 || limit > 50 {
		http.Error(w, "Invalid limit", http.StatusBadRequest)
		return
	}

	topPlayers, err := h.service.GetNTopPlayers(limit)
	if err != nil {
		http.Error(w, err.Error(), http.StatusInternalServerError)
		return
	}

	var response = make([]PlayerStatisticsDTO, 0, len(topPlayers))
	for _, player := range topPlayers {
		tmpresp := toPlayerStatisticsDTO(player)
		response = append(response, tmpresp)
	}

	w.Header().Set("Content-Type", "application/json")
	if err = json.NewEncoder(w).Encode(response); err != nil {
		http.Error(w, err.Error(), http.StatusInternalServerError)
		return
	}
}
