package domain

import "math/rand"

func randPositionExit(room Room) Position {
	return Position{
		X: rand.Intn(room.Lenght-4) + room.StartPosition.X + 2,
		Y: rand.Intn(room.Width-4) + room.StartPosition.Y + 2,
	}
}

func randPosition(room Room) Position {
	return Position{
		X: rand.Intn(room.Lenght-2) + room.StartPosition.X + 1,
		Y: rand.Intn(room.Width-2) + room.StartPosition.Y + 1,
	}
}

func randTypeEnemy(currentLevel int) EnemyType {
	switch {
	case currentLevel < 4:
		return EnemyType(EnemyZombie)
	case currentLevel < 8:
		types := []EnemyType{EnemyZombie, EnemyVampire}
		return types[rand.Intn(len(types))]
	case currentLevel < 13:
		types := []EnemyType{EnemyZombie, EnemyVampire, EnemyGhost}
		return types[rand.Intn(len(types))]
	case currentLevel < 18:
		types := []EnemyType{EnemyZombie, EnemyVampire, EnemyGhost, EnemyOgre}
		return types[rand.Intn(len(types))]
	default:
		types := []EnemyType{EnemyZombie, EnemyVampire, EnemyGhost, EnemyOgre, EnemySnakeMage}
		return types[rand.Intn(len(types))]
	}
}

func randItem() (ItemType, ItemSubtype) {
	var temp int = rand.Intn(100)
	var resType ItemType
	var resSubType ItemSubtype
	switch {
	case temp < 40:
		resType = ItemFood
	case temp < 65:
		resType = ItemElixir
	case temp < 86:
		resType = ItemScroll
	default:
		resType = ItemWeapon
	}
	switch resType {
	case ItemFood:
		resSubType = Food
	case ItemElixir:
		resSubType = HealthElixir
	case ItemScroll:
		if temp < 72 {
			resSubType = HealthScroll
		} else if temp < 79 {
			resSubType = DexterityScroll
		} else {
			resSubType = StrengthScroll
		}
	case ItemWeapon:
		if temp < 91 {
			resSubType = KnifeWeapon
		} else if temp < 95 {
			resSubType = AxeWeapon
		} else if temp < 99 {
			resSubType = SwordWeapon
		} else {
			resSubType = OnePunchWeapon
		}
	}
	return resType, resSubType
}
