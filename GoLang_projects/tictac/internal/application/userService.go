package application

import (
	"tictac3/internal/domain"
	"tictac3/internal/infrastructure/database"

	"github.com/google/uuid"
)

type UserService interface {
	CreateUser(log, pass string) error
	GetUser(log, pass string) (*domain.User, error)
	GetUserByUUID(playerID uuid.UUID) (*domain.User, error)
}

type UserServiceImpl struct {
	rep database.UserRepository
}

func NewUserService(r database.UserRepository) *UserServiceImpl {
	return &UserServiceImpl{
		rep: r,
	}
}

func (u *UserServiceImpl) CreateUser(log, pass string) error {
	user := domain.CreateNewUser(log, pass)
	err := u.rep.Create(user)
	if err != nil {
		return err
	}
	return nil
}

func (u *UserServiceImpl) GetUser(log, pass string) (*domain.User, error) {
	user, err := u.rep.Get(log, pass)
	if err != nil {
		return nil, err
	}
	return user, nil
}

func (u *UserServiceImpl) GetUserByUUID(playerID uuid.UUID) (*domain.User, error) {
	return u.rep.GetByUUID(playerID)
}
