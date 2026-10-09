package application

import (
	"encoding/base64"
	"errors"
	"os"
	"strings"
)

type UserService interface {
	SignIn(basecode string) error
}

type UserServiceImpl struct{}

func NewUserService() *UserServiceImpl { return &UserServiceImpl{} }

func (u *UserServiceImpl) SignIn(basecode string) error {
	decodeBytes, err := base64.StdEncoding.DecodeString(basecode)
	if err != nil {
		return err
	}

	tmparray := strings.SplitN(string(decodeBytes), ":", 2)
	if len(tmparray) != 2 {
		return errors.New("Error Login or Password")
	}

	login := tmparray[0]
	password := tmparray[1]
	loginEnv := os.Getenv("SITE_ADMIN_LOGIN")
	passwordEnv := os.Getenv("SITE_ADMIN_PASSWORD")
	if login != loginEnv || password != passwordEnv {
		return errors.New("Error Login or Password")
	}
	return nil
}
