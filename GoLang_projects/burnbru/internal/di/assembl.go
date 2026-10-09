package di

import (
	"Lizaweb/internal/api"
	"Lizaweb/internal/application"

	"go.uber.org/fx"
)

var Module = fx.Options(fx.Provide(
	fx.Annotate(application.NewUserService, fx.As(new(application.UserService))),
	fx.Annotate(application.NewContentService, fx.As(new(application.ContentService))),
	api.NewLoginLimiter,
	api.NewUserHandler,
	api.NewAuthenticator,
	api.NewContentHandler,
	api.NewServer,
),
	fx.Invoke(api.StartServer))
