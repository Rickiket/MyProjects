package domain

import (
	"errors"

	"github.com/google/uuid"
)

const Computerstring string = "computer"
const Playerstring string = "player"
const EmptyCell int = 0
const PlayerCell int = 1
const ComputerCell int = 2
const ForFirstIterationComputer int = -999
const ForFirstIterationPlayer int = 999
const ComputerWin int = 10
const PlayerWin int = -10
const NothingWin int = 0

type MinMaxService struct{}

func NewMinMaxService() *MinMaxService { return &MinMaxService{} }

func (m *MinMaxService) GetNextMove(g *GameSession) Board {
	cols := len(g.GameBoard.Matrix[0])
	rows := len(g.GameBoard.Matrix)
	var (
		bestScore int = ForFirstIterationComputer
		bestMove  Board
	)
	for x := 0; x < rows; x++ {
		for y := 0; y < cols; y++ {
			if g.GameBoard.Matrix[x][y] == EmptyCell {
				tmpBoard := g.GameBoard.Copy()
				tmpBoard.Matrix[x][y] = ComputerCell
				tmpScore := m.minmax(tmpBoard, Playerstring)
				if tmpScore > bestScore {
					bestMove = tmpBoard
					bestScore = tmpScore
				}
			}
		}
	}
	return bestMove
}

func (m *MinMaxService) minmax(b Board, who string) int {
	checkGame, winner := m.CheckGameOver(b)
	if checkGame == true {
		switch winner {
		case ComputerCell:
			return ComputerWin
		case PlayerCell:
			return PlayerWin
		case NothingWin:
			return NothingWin
		}
	}
	var rows = len(b.Matrix)
	var cols = len(b.Matrix[0])
	var resScore int
	switch who {
	case Playerstring:
		resScore = ForFirstIterationPlayer
		for i := 0; i < rows; i++ {
			for j := 0; j < cols; j++ {
				if b.Matrix[i][j] == EmptyCell {
					tmpBoard := b.Copy()
					tmpBoard.Matrix[i][j] = PlayerCell
					tmpScore := m.minmax(tmpBoard, Computerstring)
					if tmpScore < resScore {
						resScore = tmpScore
					}
				}
			}
		}
	case Computerstring:
		resScore = ForFirstIterationComputer
		for i := 0; i < rows; i++ {
			for j := 0; j < cols; j++ {
				if b.Matrix[i][j] == EmptyCell {
					tmpBoard := b.Copy()
					tmpBoard.Matrix[i][j] = ComputerCell
					tmpScore := m.minmax(tmpBoard, Playerstring)
					if tmpScore > resScore {
						resScore = tmpScore
					}
				}
			}
		}
	}
	return resScore
}

func (m *MinMaxService) CheckGameOver(CurrBoard Board) (bool, int) {
	board := CurrBoard.Matrix
	var flag bool
	cols := len(board[0])
	rows := len(board)
	//проверка столбцов
	for i := 0; i < cols; i++ {
		flag = true
		var winner int
		for j := 1; j < rows; j++ {
			winner = board[j][i]
			if board[j][i] != board[j-1][i] ||
				board[j][i] == EmptyCell {
				flag = false
			}
		}
		if flag == true {
			return true, winner
		}
	}
	//проверка строк
	for i := 0; i < rows; i++ {
		flag = true
		var winner int
		for j := 1; j < cols; j++ {
			winner = board[i][j]
			if board[i][j] != board[i][j-1] ||
				board[i][j] == EmptyCell {
				flag = false
			}
		}
		if flag == true {
			return true, winner
		}
	}
	//проверка диагонали 1
	flag = true
	var winner int
	for i := 1; i < rows; i++ {
		winner = board[i][i]
		if board[i][i] != board[i-1][i-1] ||
			board[i][i] == EmptyCell {
			flag = false
		}
	}
	if flag == true {
		return true, winner
	}
	//проверка диагонали 2
	flag = true
	var j int = (cols - 1) - 1
	for i := 1; i < rows; i++ {
		winner = board[i][j]
		if board[i][j] != board[i-1][j+1] ||
			board[i][j] == EmptyCell {
			flag = false
		}
		j--
	}
	if flag == true {
		return true, winner
	}

	//проверка ничьей
	flag = true
	for i := 0; i < rows; i++ {
		for j := 0; j < cols; j++ {
			if board[i][j] == EmptyCell {
				return false, NothingWin
			}
		}
	}
	if flag == true {
		return true, NothingWin
	}
	return true, NothingWin
}

func (m *MinMaxService) ValidateBoard(oldBoard Board, newBoard Board, symbol int) error {
	rows := len(oldBoard.Matrix)
	cols := len(oldBoard.Matrix[0])
	var change int = 0
	for i := 0; i < rows; i++ {
		for j := 0; j < cols; j++ {
			if oldBoard.Matrix[i][j] != newBoard.Matrix[i][j] {
				if oldBoard.Matrix[i][j] != EmptyCell ||
					newBoard.Matrix[i][j] != symbol {
					return errors.New("Invalid Move")
				}
				change++
			}
		}
	}
	if change != 1 {
		return errors.New("Player must change one cell")
	}
	return nil
}

func (g *GameSession) JoinPlayer(playerid uuid.UUID) error {
	if g.Type == PlayWithComp {
		return errors.New("Cannot join computer game")
	}
	if len(g.Players) >= 2 {
		return errors.New("Game is full")
	}
	if g.Status.Status != GameWaiting {
		return errors.New("Game is not waiting")
	}
	for _, player := range g.Players {
		if player.ID == playerid {
			return errors.New("Player is already in the game")
		}
	}
	g.Players = append(g.Players, Player{ID: playerid, Symbol: SymbolO})
	g.Status = GameStatus{Status: GameTurn, IdPlayer: g.Players[0].ID}
	return nil
}
