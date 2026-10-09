package domain

import "strconv"

func (g *GameSession) GameOverLog(key string, bp *Backpack) {
	g.Logs = nil
	switch {
	case key == "|":
		g.Logs = append(g.Logs, "Game Over")
	case g.Player.CurrentHeal <= 0:
		g.Logs = append(g.Logs, "You Died")
	default:
		g.Logs = append(g.Logs, "You Win")
	}
	g.Logs = append(g.Logs, "Statistics:")
	g.Logs = append(g.Logs, "Treasures Collected - "+strconv.Itoa(bp.Treasures))
	g.Logs = append(g.Logs, "Completet Levels - "+strconv.Itoa(g.CurrentLevel))
	g.Logs = append(g.Logs, "Killed Enemys - "+strconv.Itoa(g.Inf["KillEnemy"]))
	g.Logs = append(g.Logs, "Craversed Cells - "+strconv.Itoa(g.Inf["Cell"]))
	g.Logs = append(g.Logs, "Kicks For Enemys - "+strconv.Itoa(g.Inf["HitEnemy"]))
	g.Logs = append(g.Logs, "Kicks For Player - "+strconv.Itoa(g.Inf["HitPlayer"]))
	g.Logs = append(g.Logs, "Used Foods - "+strconv.Itoa(g.Inf["Food"]))
	g.Logs = append(g.Logs, "Used Elixirs - "+strconv.Itoa(g.Inf["Elixir"]))
	g.Logs = append(g.Logs, "Used Scrolls - "+strconv.Itoa(g.Inf["Scroll"]))
	g.Logs = append(g.Logs, "Press Q For Exit")
}

func (g *GameSession) LogInventory(bp *Backpack) {
	switch g.State {
	case InventoryFood:
		if len(bp.Foods) != 0 {
			g.Logs = append(g.Logs, "Выберите предмет:")
			for idx, i := range bp.Foods {
				g.Logs = append(g.Logs, string(i.Type)+"-"+strconv.Itoa(idx))
			}
			g.Logs = append(g.Logs, "Выйти - q:")
		} else {
			g.Logs = append(g.Logs, "Предметов нет")
			g.Logs = append(g.Logs, "Выйти - q:")
		}
	case InventoryElixir:
		if len(bp.Elixirs) != 0 {
			g.Logs = append(g.Logs, "Выберите предмет:")
			for idx, i := range bp.Elixirs {
				g.Logs = append(g.Logs, string(i.Type)+"-"+strconv.Itoa(idx))
			}
			g.Logs = append(g.Logs, "Выйти - q:")
		} else {
			g.Logs = append(g.Logs, "Предметов нет")
			g.Logs = append(g.Logs, "Выйти - q:")
		}
	case InventoryScroll:
		if len(bp.Scrolls) != 0 {
			g.Logs = append(g.Logs, "Выберите предмет:")
			for idx, i := range bp.Scrolls {
				g.Logs = append(g.Logs, string(i.Subtype)+"-"+strconv.Itoa(idx))
			}
			g.Logs = append(g.Logs, "Выйти - q:")
		} else {
			g.Logs = append(g.Logs, "Предметов нет")
			g.Logs = append(g.Logs, "Выйти - q:")
		}
	}
}
