package main

import (
	"os"
	"rogue/domain"
	"rogue/storage"
	render "rogue/ui"
)

func main() {
	var (
		input       string = ""
		gamesession *domain.GameSession
		backpack    *domain.Backpack
	)
	render.Init()
	defer render.Close()
	if _, err := os.Stat("../storage/saves/game.json"); err == nil {
		render.NewGame(1)
		for {
			input = render.ReadInput()
			if input == "0" || input == "1" {
				break
			}
		}
		if input == "0" {
			gamesession, backpack, err = storage.LoadGame()
			gamesession.State = domain.Play
			if err != nil {
				return
			}
		} else {
			gamesession = domain.NewGameSession()
			backpack = domain.GenerateBackPack()
		}
	} else {
		render.NewGame(2)
		for {
			input = render.ReadInput()
			if input == "y" {
				break
			}
		}
		gamesession = domain.NewGameSession()
		backpack = domain.GenerateBackPack()
	}
	gamesRecords, _ := storage.LoadRecords()
	domain.GenerateMap(gamesession)
	for gamesession.State != domain.GameOver {
		render.Render(gamesession, backpack, gamesRecords)
		input = render.ReadInput()
		if input == "|" {
			gamesession.State = domain.GameOver
			storage.SaveGame(gamesession, backpack)
		}
		switch gamesession.State {
		case domain.Play:
			domain.Go(gamesession, backpack, input)
		case domain.Choise:
			domain.WeaponChoise(gamesession, input)
		case domain.InventoryFood, domain.InventoryElixir, domain.InventoryScroll:
			domain.CheckInventory(gamesession, backpack, input)
		case domain.ViewRecors:
			domain.ViewRecord(gamesession, input)

		}
		if gamesession.State == domain.Play &&
			input != "left" &&
			input != "right" &&
			input != "v" &&
			input != "z" &&
			input != "q" {
			domain.ActionsEnemys(gamesession)
		}
	}
	storage.SaveRecord(gamesession, backpack)
	gamesession.GameOverLog(input, backpack)
	input = ""
	render.GameOver(gamesession.Logs)
	for input != "q" {
		input = render.ReadInput()
	}
}
