package domain

import "github.com/google/uuid"

type Player struct {
	ID     uuid.UUID
	Symbol int
}
