package domain

import "strconv"

func WeaponChoise(g *GameSession, key string) {
	if key != "y" && key != "n" && key != "н" && key != "т" {
		return
	}
	g.Logs = nil
	var i Item
	var idx int
	for idx = len(g.Levels[g.CurrentLevel].Items) - 1; idx >= 0; idx-- {
		i = g.Levels[g.CurrentLevel].Items[idx]
		if g.Player.CurrentPosition.X == i.Position.X &&
			g.Player.CurrentPosition.Y == i.Position.Y &&
			i.Type == ItemWeapon {
			break
		}
	}
	var freePos Position
	for _, j := range g.Levels[g.CurrentLevel].Rooms {
		if g.Player.CurrentPosition.X > j.StartPosition.X &&
			g.Player.CurrentPosition.X < j.StartPosition.X+j.Lenght-1 &&
			g.Player.CurrentPosition.Y > j.StartPosition.Y &&
			g.Player.CurrentPosition.Y < j.StartPosition.Y+j.Width-1 {
			if g.Player.CurrentPosition.Y-1 == j.StartPosition.Y {
				freePos = Position{X: g.Player.CurrentPosition.X, Y: g.Player.CurrentPosition.Y + 1}
			} else {
				freePos = Position{X: g.Player.CurrentPosition.X, Y: g.Player.CurrentPosition.Y - 1}
			}
		}
	}
	switch key {
	case "y", "н":
		if g.Player.CurrentWeapon.Type != "" {
			oldWeap := g.Player.CurrentWeapon
			g.Levels[g.CurrentLevel].Items = append(g.Levels[g.CurrentLevel].Items, Item{Type: oldWeap.Type, Subtype: oldWeap.Subtype, Strength: oldWeap.Strength, Position: freePos})
		}
		g.Player.CurrentWeapon = Item{Type: i.Type, Subtype: i.Subtype, Strength: i.Strength}
		g.Levels[g.CurrentLevel].Items = append(g.Levels[g.CurrentLevel].Items[:idx], g.Levels[g.CurrentLevel].Items[idx+1:]...)
		g.Player.Weapon = true
		g.State = Play
	case "n", "т":
		g.Levels[g.CurrentLevel].Items[idx].Position = Position{X: freePos.X, Y: freePos.Y}
		g.State = Play
	}
}

func CheckInventory(g *GameSession, bp *Backpack, key string) {
	g.Logs = nil
	if key == "q" {
		g.State = Play
		return
	}
	switch g.State {
	case InventoryFood:
		i := len(bp.Foods)
		idx, err := strconv.Atoi(key)
		if err != nil {
			g.LogInventory(bp)
			return
		}
		if idx < i {
			useItem(g, bp.Foods[idx])
			bp.Foods = append(bp.Foods[:idx], bp.Foods[idx+1:]...)
			g.LogInventory(bp)
		} else {
			g.LogInventory(bp)
		}
	case InventoryElixir:
		i := len(bp.Elixirs)
		idx, err := strconv.Atoi(key)
		if err != nil {
			g.LogInventory(bp)
			return
		}
		if idx < i {
			useItem(g, bp.Elixirs[idx])
			bp.Elixirs = append(bp.Elixirs[:idx], bp.Elixirs[idx+1:]...)
			g.LogInventory(bp)
		} else {
			g.LogInventory(bp)
		}
	case InventoryScroll:
		i := len(bp.Scrolls)
		idx, err := strconv.Atoi(key)
		if err != nil {
			g.LogInventory(bp)
			return
		}
		if idx < i {
			useItem(g, bp.Scrolls[idx])
			bp.Scrolls = append(bp.Scrolls[:idx], bp.Scrolls[idx+1:]...)
			g.LogInventory(bp)
		} else {
			g.LogInventory(bp)
		}
	}
}

func useItem(g *GameSession, item Item) {
	switch item.Type {
	case ItemFood, ItemElixir:
		g.Player.CurrentHeal = g.Player.CurrentHeal + item.Heal
		if g.Player.CurrentHeal > g.Player.MaxHeal {
			g.Player.CurrentHeal = g.Player.MaxHeal
		}
		if item.Type == ItemFood {
			g.Inf["Food"]++
		} else {
			g.Inf["Elixir"]++
		}
	case ItemScroll:
		switch item.Subtype {
		case HealthScroll:
			g.Player.MaxHeal = g.Player.MaxHeal + item.AddHeal
			g.Player.CurrentHeal = g.Player.CurrentHeal + item.AddHeal
		case DexterityScroll:
			g.Player.Dexterity = g.Player.Dexterity + item.AddDexterity
		case StrengthScroll:
			g.Player.Strength = g.Player.Strength + item.AddStrength
		}
		g.Inf["Scroll"]++
	}
}
