package domain

import "github.com/google/uuid"

type User struct {
	UUID     uuid.UUID
	Login    string
	Password string
}

func CreateNewUser(log, pass string) User {
	id := uuid.New()
	return User{
		UUID:     id,
		Login:    log,
		Password: pass,
	}
}
