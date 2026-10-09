package database

import (
	"context"
	"tictac3/internal/domain"

	"github.com/google/uuid"
	"github.com/jackc/pgx/v5/pgxpool"
)

type UserRepository interface {
	Create(user domain.User) error
	Get(log, pass string) (*domain.User, error)
	GetByUUID(userID uuid.UUID) (*domain.User, error)
}

type UserRepositoryImpl struct {
	db *pgxpool.Pool
}

func NewUserRepository(db *pgxpool.Pool) *UserRepositoryImpl {
	return &UserRepositoryImpl{
		db: db,
	}
}

func (ru *UserRepositoryImpl) Create(user domain.User) error {
	userEntity := toUserEntity(user)
	_, err := ru.db.Exec(context.Background(),
		`INSERT INTO users (uuid, login, password)
	 	 VALUES ($1, $2, $3)`,
		userEntity.Uuid, userEntity.Login, userEntity.Password)
	return err
}

func (ru *UserRepositoryImpl) Get(log, pass string) (*domain.User, error) {
	var userEntity UserEntity
	err := ru.db.QueryRow(context.Background(),
		`SELECT uuid, login, password
		FROM users
		WHERE login = $1 AND password = $2`,
		log, pass,
	).Scan(&userEntity.Uuid, &userEntity.Login, &userEntity.Password)
	if err != nil {
		return nil, err
	}
	res := toUserDomain(userEntity)
	return &res, nil
}

func (ru *UserRepositoryImpl) GetByUUID(userID uuid.UUID) (*domain.User, error) {
	var userEntity UserEntity

	userEntity.Uuid = userID
	err := ru.db.QueryRow(context.Background(),
		`SELECT login, password
		 FROM users
		 WHERE uuid = $1`,
		userID).Scan(&userEntity.Login, &userEntity.Password)
	if err != nil {
		return nil, err
	}

	res := toUserDomain(userEntity)
	return &res, nil
}
