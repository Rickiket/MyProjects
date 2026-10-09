package main

import (
	"tictac3/internal/di"

	"go.uber.org/fx"
)

func main() {
	app := fx.New(di.Module)
	app.Run()
}
