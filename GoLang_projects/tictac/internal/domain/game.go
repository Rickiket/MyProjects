package domain

import (
	"time"

	"github.com/google/uuid"
)

const (
	SymbolX int = 1
	SymbolO int = 2
)
const (
	GameWaiting string = "waiting"
	GameTurn    string = "turn"
	GameDraw    string = "draw"
	GameWin     string = "win"
)

type GameType string

const (
	PlayWithPlayer GameType = "player"
	PlayWithComp   GameType = "computer"
)

type GameStatus struct {
	Status   string    `json:"status"`
	IdPlayer uuid.UUID `json:"id_player"`
}

type PlayerStatistics struct {
	Id       uuid.UUID
	Login    string
	Wins     int
	Losses   int
	Draws    int
	WinRatio float64
}

type GameSession struct {
	UUID         uuid.UUID
	GameBoard    Board
	Status       GameStatus
	Players      []Player
	Type         GameType
	CreationDate time.Time
}

func NewGameSession(id uuid.UUID, typeGame GameType) *GameSession {
	var status string
	if typeGame == PlayWithComp {
		status = GameTurn
	} else {
		status = GameWaiting
	}
	return &GameSession{
		UUID:         uuid.New(),
		GameBoard:    NewBoard(),
		Status:       GameStatus{Status: status, IdPlayer: id},
		Players:      []Player{{ID: id, Symbol: SymbolX}},
		Type:         typeGame,
		CreationDate: time.Now(),
	}
}
