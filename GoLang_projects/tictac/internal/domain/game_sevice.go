package domain

type GameService interface {
	GetNextMove(g *GameSession) Board
	ValidateBoard(oldBoard Board, newBoard Board, symbol int) error
	CheckGameOver(CurrBoard Board) (bool, int)
}
