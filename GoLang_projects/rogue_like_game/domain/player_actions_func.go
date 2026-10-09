package domain

import "math"

func Go(g *GameSession, bp *Backpack, key string) {
	if g.Player.Sleep == true {
		g.Player.Sleep = false
		return
	}
	g.Logs = nil
	g.CheckItem(bp)
	if g.State == Choise {
		return
	}
	var attack bool = false
	var (
		x int
		y int
	)
	switch key {
	case "a":
		x := 0
		y := 0
		if g.ThreeD == true {
			leftAngle := g.Player.Angle - math.Pi/2
			x = int(math.Round(math.Cos(leftAngle)))
			y = int(math.Round(math.Sin(leftAngle)))
		} else {
			x = -1
		}
		attack = g.MovePlayer(x, y)
	case "w":
		x := 0
		y := 0
		if g.ThreeD == true {
			x = int(math.Round(math.Cos(g.Player.Angle)))
			y = int(math.Round(math.Sin(g.Player.Angle)))
		} else {
			y = -1
		}
		attack = g.MovePlayer(x, y)
	case "d":
		x := 0
		y := 0
		if g.ThreeD == true {
			rightAngle := g.Player.Angle + math.Pi/2
			x = int(math.Round(math.Cos(rightAngle)))
			y = int(math.Round(math.Sin(rightAngle)))
		} else {
			x = 1
		}
		attack = g.MovePlayer(x, y)
	case "s":
		x := 0
		y := 0
		if g.ThreeD == true {
			x = int(math.Round(math.Cos(g.Player.Angle)))
			y = int(math.Round(math.Sin(g.Player.Angle)))
		} else {
			y = -1
		}
		attack = g.MovePlayer(-x, -y)
	//j-food k-elixir e-scroll
	case "j", "о":
		g.State = InventoryFood
		g.LogInventory(bp)
	case "k", "л":
		g.State = InventoryElixir
		g.LogInventory(bp)
	case "e", "у":
		g.State = InventoryScroll
		g.LogInventory(bp)
	case "r":
		g.State = ViewRecors
	case "left":
		g.Player.Angle = g.Player.Angle - math.Pi/2
		for g.Player.Angle > math.Pi {
			g.Player.Angle -= 2 * math.Pi
		}
		for g.Player.Angle < -math.Pi {
			g.Player.Angle += 2 * math.Pi
		}
	case "right":
		g.Player.Angle = g.Player.Angle + math.Pi/2
		for g.Player.Angle > math.Pi {
			g.Player.Angle -= 2 * math.Pi
		}
		for g.Player.Angle < -math.Pi {
			g.Player.Angle += 2 * math.Pi
		}
	case "v":
		if g.ThreeD == true {
			g.ThreeD = false
		} else {
			g.ThreeD = true
		}
	case "z":
		if g.Player.Weapon == true {
			g.Player.Weapon = false
		} else if g.Player.CurrentWeapon.Type != "" {
			g.Player.Weapon = true
		} else {
			g.Logs = append(g.Logs, "Оружия нет")
		}
	}
	if g.Player.CurrentPosition.X == g.Levels[g.CurrentLevel].Exit.X &&
		g.Player.CurrentPosition.Y == g.Levels[g.CurrentLevel].Exit.Y {
		if g.CurrentLevel == 20 {
			g.State = GameOver
		} else {
			g.CurrentLevel++
			g.Player.CurrentPosition = randPosition(g.Levels[g.CurrentLevel].Rooms[0])
			GenerateMap(g)
		}
	}
	if attack == true {
		g.AttackEnemy(g.Player.CurrentPosition.X+x, g.Player.CurrentPosition.Y+y, bp)
	} else {
		g.CheckItem(bp)
	}
}

func ViewRecord(g *GameSession, key string) {
	if key == "q" {
		g.State = Play
	}
}
