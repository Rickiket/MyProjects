package database

import "time"

type GameEntity struct {
	ID           string  `json:"uuid"`
	Board        [][]int `json:"board"`
	Status       []byte  `json:"status"`
	Players      []byte  `json:"players"`
	GameType     string  `json:"gametype"`
	CreationDate time.Time
}

type PlayerStatisticsEntity struct {
	Id       string
	Login    string
	Wins     int
	Losses   int
	Draws    int
	WinRatio float64
}
