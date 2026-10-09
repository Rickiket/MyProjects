package web

import (
	"tictac3/internal/domain"

	"github.com/google/uuid"
)

func toEntity(g domain.GameSession) GameEntity {
	players := make([]PlayerDTO, 0, 2)
	for _, i := range g.Players {
		players = append(players, PlayerDTO{i.ID.String(), i.Symbol})
	}
	return GameEntity{
		ID:       g.UUID.String(),
		Board:    g.GameBoard.Matrix,
		Status:   GameStatusDTO{Status: g.Status.Status, IdPlayer: g.Status.IdPlayer.String()},
		Players:  players,
		GameType: string(g.Type),
	}
}

func toDomain(g GameEntity) domain.GameSession {
	id, _ := uuid.Parse(g.ID)
	players := make([]domain.Player, 0, 2)
	tempStatusId, _ := uuid.Parse(g.Status.IdPlayer)
	for _, i := range g.Players {
		tempId, _ := uuid.Parse(i.ID)
		players = append(players, domain.Player{ID: tempId, Symbol: i.Symbol})
	}
	return domain.GameSession{
		UUID: id,
		GameBoard: domain.Board{
			Matrix: g.Board,
		},
		Status:  domain.GameStatus{Status: g.Status.Status, IdPlayer: tempStatusId},
		Players: players,
		Type:    domain.GameType(g.GameType),
	}
}

func toUserEntity(u domain.User) UserEntity {
	id := u.UUID.String()
	return UserEntity{
		ID:    id,
		Login: u.Login,
	}
}

func toPlayerStatisticsDTO(s domain.PlayerStatistics) PlayerStatisticsDTO {
	return PlayerStatisticsDTO{
		Id:       s.Id.String(),
		Login:    s.Login,
		Wins:     s.Wins,
		Losses:   s.Losses,
		Draws:    s.Draws,
		WinRatio: s.WinRatio,
	}
}
