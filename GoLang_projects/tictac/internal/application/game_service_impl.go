package application

import (
	"tictac3/internal/domain"
	"tictac3/internal/infrastructure/database"

	"github.com/google/uuid"
)

type GameServiceImpl struct {
	repository database.GameRepository
	strategy   domain.GameService
}

func NewGameService(rep database.GameRepository, st domain.GameService) *GameServiceImpl {
	return &GameServiceImpl{
		repository: rep,
		strategy:   st,
	}
}

func (gs *GameServiceImpl) MakeMove(id uuid.UUID, newBoard domain.Board) (domain.GameSession, error) {
	currGame, err := gs.repository.Get(id)
	if err != nil {
		return domain.GameSession{}, err
	}

	if currGame.Type == domain.PlayWithComp {
		err = gs.strategy.ValidateBoard(currGame.GameBoard, newBoard, domain.SymbolX)
	} else {
		var symbol int
		if currGame.Status.IdPlayer == currGame.Players[0].ID {
			symbol = currGame.Players[0].Symbol
		} else {
			symbol = currGame.Players[1].Symbol
		}
		err = gs.strategy.ValidateBoard(currGame.GameBoard, newBoard, symbol)
	}
	if err != nil {
		return domain.GameSession{}, err
	}

	currGame.GameBoard = newBoard

	over, winner := gs.strategy.CheckGameOver(currGame.GameBoard)
	if over == true && currGame.Type == domain.PlayWithComp {
		switch winner {
		case domain.ComputerCell:
			currGame.Status = domain.GameStatus{Status: domain.GameWin, IdPlayer: uuid.Nil}
		case domain.PlayerCell:
			currGame.Status = domain.GameStatus{Status: domain.GameWin, IdPlayer: currGame.Status.IdPlayer}
		case domain.NothingWin:
			currGame.Status = domain.GameStatus{Status: domain.GameDraw, IdPlayer: uuid.Nil}
		}
		err = gs.repository.Save(currGame)
		if err != nil {
			return domain.GameSession{}, err
		}
		return currGame, err
	} else if over == true {
		var playerWinId uuid.UUID
		if winner == currGame.Players[0].Symbol {
			playerWinId = currGame.Players[0].ID
		} else {
			playerWinId = currGame.Players[1].ID
		}
		switch winner {
		case domain.SymbolO, domain.SymbolX:
			currGame.Status = domain.GameStatus{Status: domain.GameWin, IdPlayer: playerWinId}
		case domain.NothingWin:
			currGame.Status = domain.GameStatus{Status: domain.GameDraw, IdPlayer: uuid.Nil}
		}
		err = gs.repository.Save(currGame)
		if err != nil {
			return domain.GameSession{}, err
		}
		return currGame, err
	}
	//пердать ход игроку и проверить если комп закончена ли игра
	if currGame.Type == domain.PlayWithComp {
		currGame.GameBoard = gs.strategy.GetNextMove(&currGame)
		over, winner = gs.strategy.CheckGameOver(currGame.GameBoard)
		if over == true {
			switch winner {
			case domain.ComputerCell:
				currGame.Status = domain.GameStatus{Status: domain.GameWin, IdPlayer: uuid.Nil}
			case domain.PlayerCell:
				currGame.Status = domain.GameStatus{Status: domain.GameWin, IdPlayer: currGame.Status.IdPlayer}
			case domain.NothingWin:
				currGame.Status = domain.GameStatus{Status: domain.GameDraw, IdPlayer: uuid.Nil}
			}
			err = gs.repository.Save(currGame)
			if err != nil {
				return domain.GameSession{}, err
			}
			return currGame, err
		}
	} else {
		if currGame.Status.IdPlayer == currGame.Players[0].ID {
			currGame.Status.IdPlayer = currGame.Players[1].ID
		} else {
			currGame.Status.IdPlayer = currGame.Players[0].ID
		}
	}

	err = gs.repository.Save(currGame)
	if err != nil {
		return domain.GameSession{}, err
	}
	return currGame, nil
}

func (gs *GameServiceImpl) CreateGame(id uuid.UUID, typeGame string) (*domain.GameSession, error) {
	game := domain.NewGameSession(id, domain.GameType(typeGame))
	err := gs.repository.Save(*game)
	if err != nil {
		return &domain.GameSession{}, err
	}
	return game, nil
}

func (gs *GameServiceImpl) GetAvailableGames() ([]uuid.UUID, error) {
	return gs.repository.GetAvailableGames()
}

func (gs *GameServiceImpl) JoinGame(gameID uuid.UUID, playerID uuid.UUID) (domain.GameSession, error) {
	game, err := gs.repository.Get(gameID)
	if err != nil {
		return domain.GameSession{}, err
	}
	err = game.JoinPlayer(playerID)
	if err != nil {
		return domain.GameSession{}, err
	}
	err = gs.repository.Save(game)
	if err != nil {
		return domain.GameSession{}, err
	}
	return game, err
}

func (gs *GameServiceImpl) GetGame(gameID uuid.UUID) (domain.GameSession, error) {
	return gs.repository.Get(gameID)
}

func (gs *GameServiceImpl) GetCompletedGamesByUUIDPlayer(playerId uuid.UUID) ([]domain.GameSession, error) {
	return gs.repository.GetCompletedGamesByUUIDPlayer(playerId)
}

func (gs *GameServiceImpl) GetNTopPlayers(limit int) ([]domain.PlayerStatistics, error) {
	return gs.repository.GetNTopPlayers(limit)
}
