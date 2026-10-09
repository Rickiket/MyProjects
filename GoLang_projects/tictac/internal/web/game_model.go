package web

type GameEntity struct {
	ID       string        `json:"id"`
	Board    [][]int       `json:"board"`
	Status   GameStatusDTO `json:"status"`
	Players  []PlayerDTO   `json:"players"`
	GameType string        `json:"gametype"`
}

type GameStatusDTO struct {
	Status   string `json:"status"`
	IdPlayer string `json:"idPlayer"`
}

type PlayerDTO struct {
	ID     string `json:"id"`
	Symbol int    `json:"symbol"`
}

type PlayerStatisticsDTO struct {
	Id       string  `json:"id"`
	Login    string  `json:"login"`
	Wins     int     `json:"wins"`
	Losses   int     `json:"losses"`
	Draws    int     `json:"draws"`
	WinRatio float64 `json:"winRatio"`
}
