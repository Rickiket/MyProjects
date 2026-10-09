package application

import (
	"tictac3/internal/domain"

	"github.com/google/uuid"
)

type GameService interface {
	MakeMove(id uuid.UUID, newBoard domain.Board) (domain.GameSession, error)
	CreateGame(id uuid.UUID, typeGame string) (*domain.GameSession, error)
	GetAvailableGames() ([]uuid.UUID, error)
	JoinGame(gameID uuid.UUID, playerID uuid.UUID) (domain.GameSession, error)
	GetGame(gameID uuid.UUID) (domain.GameSession, error)
	GetCompletedGamesByUUIDPlayer(playerId uuid.UUID) ([]domain.GameSession, error)
	GetNTopPlayers(limit int) ([]domain.PlayerStatistics, error)
}
