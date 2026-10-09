package database

import (
	"tictac3/internal/domain"

	"github.com/google/uuid"
)

type GameRepository interface {
	Save(game domain.GameSession) error
	Get(uuid uuid.UUID) (domain.GameSession, error)
	GetAvailableGames() ([]uuid.UUID, error)
	GetCompletedGamesByUUIDPlayer(idPlayer uuid.UUID) ([]domain.GameSession, error)
	GetNTopPlayers(limit int) ([]domain.PlayerStatistics, error)
}
