package application

import (
	jwtprovider "tictac3/internal/infrastructure/jwt"
)

type AuthService struct {
	userService UserService
	jwtService  *jwtprovider.JwtProvider
}

func NewAuthService(u UserService, jwtService *jwtprovider.JwtProvider) *AuthService {
	return &AuthService{
		userService: u,
		jwtService:  jwtService,
	}
}

func (au *AuthService) SignUp(request SignUpRequest) (bool, error) {
	err := au.userService.CreateUser(request.Login, request.Password)
	if err != nil {
		return false, err
	}
	return true, nil
}

func (au *AuthService) SignIn(request JwtRequest) (JwtResponse, error) {
	user, err := au.userService.GetUser(request.Login, request.Password)
	if err != nil {
		return JwtResponse{}, err
	}

	accessToken, err := au.jwtService.GenerateAccessToken(*user)
	if err != nil {
		return JwtResponse{}, err
	}

	refreshToken, err := au.jwtService.GenerateRefreshToken(*user)
	if err != nil {
		return JwtResponse{}, err
	}

	return JwtResponse{Type: "Bearer", AccessToken: accessToken, RefreshToken: refreshToken}, nil
}

func (au *AuthService) RefreshAccessToken(refreshToken string) (JwtResponse, error) {
	err := au.jwtService.ValidateRefreshToken(refreshToken)
	if err != nil {
		return JwtResponse{}, err
	}

	id, err := au.jwtService.GetUUIDFromToken(refreshToken)
	if err != nil {
		return JwtResponse{}, err
	}

	user, err := au.userService.GetUserByUUID(id)
	if err != nil {
		return JwtResponse{}, err
	}

	accessToken, err := au.jwtService.GenerateAccessToken(*user)
	if err != nil {
		return JwtResponse{}, err
	}

	return JwtResponse{Type: "Bearer", AccessToken: accessToken, RefreshToken: refreshToken}, nil
}

func (au *AuthService) RefreshRefreshToken(refreshToken string) (JwtResponse, error) {
	err := au.jwtService.ValidateRefreshToken(refreshToken)
	if err != nil {
		return JwtResponse{}, err
	}

	id, err := au.jwtService.GetUUIDFromToken(refreshToken)
	if err != nil {
		return JwtResponse{}, err
	}

	user, err := au.userService.GetUserByUUID(id)
	if err != nil {
		return JwtResponse{}, err
	}

	accessToken, err := au.jwtService.GenerateAccessToken(*user)
	if err != nil {
		return JwtResponse{}, err
	}

	newRefreshToken, err := au.jwtService.GenerateRefreshToken(*user)
	if err != nil {
		return JwtResponse{}, err
	}

	return JwtResponse{Type: "Bearer", AccessToken: accessToken, RefreshToken: newRefreshToken}, nil
}
