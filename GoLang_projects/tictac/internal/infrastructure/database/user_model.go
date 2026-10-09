package database

import "github.com/google/uuid"

type UserEntity struct {
	Uuid     uuid.UUID `json:"uuid"`
	Login    string    `json:"login"`
	Password string    `json:"password"`
}
