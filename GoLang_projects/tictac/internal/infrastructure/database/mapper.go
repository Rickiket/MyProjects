package database

import (
	"encoding/json"
	"tictac3/internal/domain"

	"github.com/google/uuid"
)

func toEntity(g domain.GameSession) (GameEntity, error) {
	status, err := json.Marshal(g.Status)
	if err != nil {
		return GameEntity{}, err
	}
	players, err := json.Marshal(g.Players)
	if err != nil {
		return GameEntity{}, err
	}
	return GameEntity{
		ID:           g.UUID.String(),
		Board:        g.GameBoard.Matrix,
		Status:       status,
		Players:      players,
		GameType:     string(g.Type),
		CreationDate: g.CreationDate,
	}, nil
}

func toDomain(g GameEntity) (domain.GameSession, error) {
	id, err := uuid.Parse(g.ID)
	if err != nil {
		return domain.GameSession{}, err
	}

	var status domain.GameStatus
	err = json.Unmarshal(g.Status, &status)
	if err != nil {
		return domain.GameSession{}, err
	}

	var players []domain.Player
	err = json.Unmarshal(g.Players, &players)
	if err != nil {
		return domain.GameSession{}, err
	}

	return domain.GameSession{
		UUID:         id,
		GameBoard:    domain.Board{Matrix: g.Board},
		Status:       status,
		Players:      players,
		Type:         domain.GameType(g.GameType),
		CreationDate: g.CreationDate,
	}, nil
}

func toUserEntity(u domain.User) UserEntity {
	return UserEntity{
		Uuid:     u.UUID,
		Login:    u.Login,
		Password: u.Password,
	}
}

func toUserDomain(u UserEntity) domain.User {
	return domain.User{
		UUID:     u.Uuid,
		Login:    u.Login,
		Password: u.Password,
	}
}

func toPlayerStatistcs(s PlayerStatisticsEntity) (domain.PlayerStatistics, error) {
	playerId, err := uuid.Parse(s.Id)
	if err != nil {
		return domain.PlayerStatistics{}, err
	}
	return domain.PlayerStatistics{
		Id:       playerId,
		Login:    s.Login,
		Wins:     s.Wins,
		Losses:   s.Losses,
		Draws:    s.Draws,
		WinRatio: s.WinRatio,
	}, nil
}
