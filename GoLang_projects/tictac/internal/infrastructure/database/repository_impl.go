package database

import (
	"context"
	"encoding/json"
	"errors"
	"tictac3/internal/domain"
	"time"

	"github.com/google/uuid"
	"github.com/jackc/pgx/v5/pgxpool"
)

type GameRepositoryImpl struct {
	db *pgxpool.Pool
}

func NewRepository(db *pgxpool.Pool) *GameRepositoryImpl {
	return &GameRepositoryImpl{
		db: db,
	}
}
func (rep *GameRepositoryImpl) Save(g domain.GameSession) error {
	entity, err := toEntity(g)
	if err != nil {
		return err
	}

	boardJSON, err := json.Marshal(entity.Board)
	if err != nil {
		return err
	}

	_, err = rep.db.Exec(
		context.Background(),
		`INSERT INTO game_session (uuid, board, status, players, type, creation_date)
		 VALUES ($1, $2, $3, $4, $5, $6)
		 ON CONFLICT (uuid)
		 DO UPDATE SET board = EXCLUDED.board,
		 			   status = EXCLUDED.status,
					   players = EXCLUDED.players,
					   type = EXCLUDED.type,
					   creation_date = EXCLUDED.creation_date`,
		entity.ID, boardJSON, entity.Status, entity.Players, entity.GameType, entity.CreationDate,
	)
	if err != nil {
		return err
	}
	return nil
}

func (rep *GameRepositoryImpl) Get(id uuid.UUID) (domain.GameSession, error) {
	var (
		boardJSON    []byte
		status       []byte
		players      []byte
		typeGame     string
		creationDate time.Time
	)
	err := rep.db.QueryRow(
		context.Background(),
		`SELECT board, status, players, type, creation_date
		 FROM game_session
		 WHERE uuid = $1`,
		id,
	).Scan(&boardJSON, &status, &players, &typeGame, &creationDate)
	if err != nil {
		return domain.GameSession{}, err
	}
	var entityBoard [][]int
	err = json.Unmarshal(boardJSON, &entityBoard)
	if err != nil {
		return domain.GameSession{}, err
	}
	var entity = GameEntity{
		ID:           id.String(),
		Board:        entityBoard,
		Status:       status,
		Players:      players,
		GameType:     typeGame,
		CreationDate: creationDate,
	}
	result, err := toDomain(entity)
	if err != nil {
		return domain.GameSession{}, err
	}
	return result, nil
}

func (rep *GameRepositoryImpl) GetAvailableGames() ([]uuid.UUID, error) {
	rows, err := rep.db.Query(
		context.Background(),
		`SELECT uuid
		 FROM game_session
		 WHERE type = 'player'
		 	AND status->>'status' = 'waiting'`,
	)
	if err != nil {
		return []uuid.UUID{}, err
	}

	defer rows.Close()
	var uuids []uuid.UUID

	for rows.Next() == true {
		var id uuid.UUID
		err = rows.Scan(&id)
		if err != nil {
			return []uuid.UUID{}, err
		}
		uuids = append(uuids, id)
	}

	if err = rows.Err(); err != nil {
		return []uuid.UUID{}, err
	}
	return uuids, nil
}

func (rep *GameRepositoryImpl) GetCompletedGamesByUUIDPlayer(idPlayer uuid.UUID) ([]domain.GameSession, error) {
	idPlayerSring := idPlayer.String()
	rows, err := rep.db.Query(
		context.Background(),
		`SELECT *
		 FROM game_session
		 WHERE status ->> 'status' IN ('win','draw')
			AND players @> jsonb_build_array(jsonb_build_object('ID', $1))`,
		idPlayerSring,
	)

	if err != nil {
		return nil, err
	}

	defer rows.Close()

	var completedgames = make([]domain.GameSession, 0)

	for rows.Next() == true {
		var (
			JSONBoard  []byte
			gameEntity GameEntity
		)
		err = rows.Scan(
			&gameEntity.ID,
			&JSONBoard,
			&gameEntity.Status,
			&gameEntity.Players,
			&gameEntity.GameType,
			&gameEntity.CreationDate,
		)
		if err != nil {
			return nil, err
		}

		err = json.Unmarshal(JSONBoard, &gameEntity.Board)
		if err != nil {
			return nil, err
		}

		gameDomain, err := toDomain(gameEntity)
		if err != nil {
			return nil, err
		}

		completedgames = append(completedgames, gameDomain)
	}

	if err = rows.Err(); err != nil {
		return nil, err
	}

	return completedgames, nil
}

func (rep *GameRepositoryImpl) GetNTopPlayers(limit int) ([]domain.PlayerStatistics, error) {
	if limit <= 0 {
		return nil, errors.New("limit must be greater than 0")
	}
	rows, err := rep.db.Query(context.Background(),
		`WITH players_games AS (
		      SELECT
					p ->> 'ID' AS player_id,
					gs.status ->> 'status' AS game_status,
					gs.status ->> 'id_player' AS winner_id
			  FROM game_session AS gs
			  CROSS JOIN LATERAL jsonb_array_elements(gs.players) AS p
					WHERE gs.status ->> 'status' IN ('win', 'draw')
		),
		statistics AS (
			  SELECT
					player_id,
					COUNT(*) FILTER (
						WHERE game_status = 'win'
						AND player_id = winner_id
					) AS wins,
					COUNT(*) FILTER (
						WHERE game_status = 'win'
						AND player_id != winner_id
					) AS losses,
					COUNT(*) FILTER (
						WHERE game_status = 'draw'
					) AS draws
			  FROM players_games
			  GROUP BY player_id
		)
		SELECT
			s.player_id,
			u.login,
			s.wins,
			s.losses,
			s.draws,
			CASE
				WHEN s.losses + s.draws = 0
					THEN s.wins::numeric
					ELSE s.wins::numeric / (s.losses + s.draws)
			END AS winratio
		FROM statistics s
		INNER JOIN users AS u
			ON u.uuid = s.player_id::uuid
		ORDER BY winratio DESC
		LIMIT $1`, limit)
	if err != nil {
		return nil, err
	}
	defer rows.Close()
	topPlayers := make([]domain.PlayerStatistics, 0, limit)

	for rows.Next() {
		var playerStat PlayerStatisticsEntity
		err = rows.Scan(
			&playerStat.Id,
			&playerStat.Login,
			&playerStat.Wins,
			&playerStat.Losses,
			&playerStat.Draws,
			&playerStat.WinRatio)
		if err != nil {
			return nil, err
		}

		res, err := toPlayerStatistcs(playerStat)
		if err != nil {
			return nil, err
		}

		topPlayers = append(topPlayers, res)
	}
	if err = rows.Err(); err != nil {
		return nil, err
	}

	return topPlayers, nil
}
