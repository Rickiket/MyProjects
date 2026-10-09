package jwtprovider

import (
	"errors"
	"os"
	"tictac3/internal/domain"
	"time"

	"github.com/golang-jwt/jwt/v5"
	"github.com/google/uuid"
)

type JwtProvider struct {
	secret string
}

func NewJwtProvider() *JwtProvider {
	secret := os.Getenv("JWT_SECRET")
	if secret == "" {
		panic("JWT_SECRET is not set")
	}
	return &JwtProvider{secret: secret}
}

func (j *JwtProvider) GenerateAccessToken(user domain.User) (string, error) {
	claims := jwt.MapClaims{
		"uuid": user.UUID.String(),
		"type": "access",
		"exp":  time.Now().Add(15 * time.Minute).Unix(),
	}
	token := jwt.NewWithClaims(jwt.SigningMethodHS256, claims)
	return token.SignedString([]byte(j.secret))
}

func (j *JwtProvider) GenerateRefreshToken(user domain.User) (string, error) {
	claims := jwt.MapClaims{
		"uuid": user.UUID.String(),
		"type": "refresh",
		"exp":  time.Now().Add(7 * 24 * time.Hour).Unix(),
	}
	token := jwt.NewWithClaims(jwt.SigningMethodHS256, claims)
	return token.SignedString([]byte(j.secret))
}

func (j *JwtProvider) ValidateAccessToken(tokenString string) error {
	token, err := jwt.Parse(tokenString, func(t *jwt.Token) (any, error) {
		if t.Method != jwt.SigningMethodHS256 {
			return nil, errors.New("Unexpected Signing Method")
		}
		return []byte(j.secret), nil
	})
	if err != nil {
		return err
	}
	if token.Valid == false {
		return errors.New("Invalid Access Token")
	}

	claims, ok := token.Claims.(jwt.MapClaims)
	if ok == false {
		return errors.New("Invalid Claims")
	}

	tokenType, ok := claims["type"].(string)
	if ok == false {
		return errors.New("Token Type Not Found")
	}

	if tokenType != "access" {
		return errors.New("Token Is Not Access")
	}

	return nil
}

func (j *JwtProvider) ValidateRefreshToken(tokenString string) error {
	token, err := jwt.Parse(tokenString, func(t *jwt.Token) (any, error) {
		if t.Method != jwt.SigningMethodHS256 {
			return nil, errors.New("Unexpected Signing Method")
		}
		return []byte(j.secret), nil
	})
	if err != nil {
		return err
	}
	if token.Valid == false {
		return errors.New("Invalid Refresh Token")
	}

	claims, ok := token.Claims.(jwt.MapClaims)
	if ok == false {
		return errors.New("Invalid Claims")
	}

	tokenType, ok := claims["type"].(string)
	if ok == false {
		return errors.New("Token Type Not Found")
	}

	if tokenType != "refresh" {
		return errors.New("Token Is Not Refresh")
	}

	return nil
}

func (j *JwtProvider) GetUUIDFromToken(tokenString string) (uuid.UUID, error) {
	token, err := jwt.Parse(tokenString, func(t *jwt.Token) (any, error) {
		if t.Method != jwt.SigningMethodHS256 {
			return nil, errors.New("Unexpected Signing Method")
		}
		return []byte(j.secret), nil
	})
	if err != nil {
		return uuid.Nil, err
	}
	if token.Valid == false {
		return uuid.Nil, errors.New("Invalid Token")
	}

	claims, ok := token.Claims.(jwt.MapClaims)
	if ok == false {
		return uuid.Nil, errors.New("Invalid Claims")
	}

	uuidString, ok := claims["uuid"].(string)
	if ok == false {
		return uuid.Nil, errors.New("UUID Not Found In Token")
	}

	id, err := uuid.Parse(uuidString)
	if err != nil {
		return uuid.Nil, err
	}

	return id, nil
}
