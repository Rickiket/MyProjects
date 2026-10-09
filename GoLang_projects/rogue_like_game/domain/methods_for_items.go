package domain

func (g *GameSession) CheckItem(bp *Backpack) {
	for idx := len(g.Levels[g.CurrentLevel].Items) - 1; idx >= 0; idx-- {
		i := g.Levels[g.CurrentLevel].Items[idx]
		if g.Player.CurrentPosition.X == i.Position.X &&
			g.Player.CurrentPosition.Y == i.Position.Y &&
			i.Type == ItemWeapon {
			g.State = Choise
			switch i.Subtype {
			case KnifeWeapon:
				g.Logs = append(g.Logs, "Вы нашли НОЖ (+4 к силе)\nЭкипировать Y/N?")
			case SwordWeapon:
				g.Logs = append(g.Logs, "Вы нашли МЕЧ (+8 к силе)\nЭкипировать Y/N?")
			case AxeWeapon:
				g.Logs = append(g.Logs, "Вы нашли ТОПОР (+12 к силе)\nЭкипировать Y/N?")
			case OnePunchWeapon:
				g.Logs = append(g.Logs, "Вы нашли OnePunch (+50 к силе)\nЭкипировать Y/N?")
			}
			return
		}
		if g.Player.CurrentPosition.X == i.Position.X &&
			g.Player.CurrentPosition.Y == i.Position.Y {
			if g.AddItem(i, bp) {
				g.Levels[g.CurrentLevel].Items = append(g.Levels[g.CurrentLevel].Items[:idx], g.Levels[g.CurrentLevel].Items[idx+1:]...)
			}
		}
	}
}

func (g *GameSession) AddItem(item Item, bp *Backpack) bool { //есть ли место
	var res bool = true
	var tmp int = 0
	switch item.Type {
	case ItemFood:
		tmp = len(bp.Foods)
	case ItemElixir:
		tmp = len(bp.Elixirs)
	case ItemScroll:
		tmp = len(bp.Scrolls)
	}
	if tmp == bp.Limit {
		res = false
		switch item.Type {
		case ItemFood:
			g.Logs = append(g.Logs, "Нет места для еды")
		case ItemElixir:
			g.Logs = append(g.Logs, "Нет места для элексира")
		case ItemScroll:
			g.Logs = append(g.Logs, "Нет места для свитка")
		}
	} else {
		switch item.Type {
		case ItemFood:
			bp.Foods = append(bp.Foods, Item{Type: item.Type, Subtype: item.Subtype, Heal: item.Heal})
		case ItemElixir:
			bp.Elixirs = append(bp.Elixirs, Item{Type: item.Type, Subtype: item.Subtype, Heal: item.Heal})
		case ItemScroll:
			bp.Scrolls = append(bp.Scrolls, Item{Type: item.Type, Subtype: item.Subtype, AddHeal: item.AddHeal, AddDexterity: item.AddDexterity, AddStrength: item.AddStrength})
		}
	}
	return res
}
