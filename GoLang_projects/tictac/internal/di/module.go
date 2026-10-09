package di

import (
	"tictac3/internal/application"
	"tictac3/internal/domain"
	"tictac3/internal/infrastructure/database"
	jwtprovider "tictac3/internal/infrastructure/jwt"
	"tictac3/internal/web"

	"go.uber.org/fx"
)

var Module = fx.Options(fx.Provide(
	fx.Annotate(domain.NewMinMaxService, fx.As(new(domain.GameService))),
	database.NewDB,
	fx.Annotate(database.NewRepository, fx.As(new(database.GameRepository))),
	fx.Annotate(database.NewUserRepository, fx.As(new(database.UserRepository))),
	fx.Annotate(application.NewUserService, fx.As(new(application.UserService))),
	application.NewAuthService,
	fx.Annotate(application.NewGameService, fx.As(new(application.GameService))),
	web.NewGameHandler,
	web.NewAuthHandler,
	web.NewAuthenticator,
	web.NewServer,
	web.NewUserHandler,
	jwtprovider.NewJwtProvider,
),
	fx.Invoke(web.StartServer))
