package database

import (
	"context"

	"github.com/jackc/pgx/v5/pgxpool"
)

func NewDB() (*pgxpool.Pool, error) {
	return pgxpool.New(context.Background(), "postgres://rickiket@localhost:5432/tictacv3")
}
