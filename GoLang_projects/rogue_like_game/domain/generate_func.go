package domain

import "math/rand"

func GenerateMap(g *GameSession) {
	for x := 0; x < MapLenAndWidth; x++ {
		for y := 0; y < MapLenAndWidth; y++ {
			g.WorldMap[x][y] = "#"
		}
	}
	for _, i := range g.Levels[g.CurrentLevel].Rooms {
		for x := i.StartPosition.X + 1; x < i.StartPosition.X+i.Lenght-1; x++ {
			for y := i.StartPosition.Y + 1; y < i.StartPosition.Y+i.Width-1; y++ {
				g.WorldMap[x][y] = "."
			}
		}
	}
	for _, i := range g.Levels[g.CurrentLevel].Coridors {
		for idx, j := range i.Cells {
			if idx == 0 ||
				idx == len(i.Cells)-1 {
				g.WorldMap[j.X][j.Y] = "d"
			} else {
				g.WorldMap[j.X][j.Y] = "."
			}
		}
	}
}

func GenerateBackPack() *Backpack {
	return &Backpack{
		Foods:     []Item{},
		Elixirs:   []Item{},
		Scrolls:   []Item{},
		Treasures: 0,
		Limit:     9,
	}
}

func NewGameSession() *GameSession {
	var levels []Level
	for i := 0; i < 21; i++ {
		levels = append(levels, GenerateLevels(i))
	}
	player := GeneratePlayer(levels[0])
	inf := make(map[string]int)
	world := make([][]string, MapLenAndWidth)
	for x := 0; x < MapLenAndWidth; x++ {
		world[x] = make([]string, MapLenAndWidth)
	}
	return &GameSession{
		CurrentLevel: 0,
		Levels:       levels,
		Player:       player,
		State:        Play,
		Inf:          inf,
		ThreeD:       false,
		WorldMap:     world,
	}
}

func GeneratePlayer(lvl Level) *Character {
	position := randPosition(lvl.Rooms[0])
	return &Character{
		CurrentRoom:     0,
		CurrentPosition: position,
		MaxHeal:         100,
		CurrentHeal:     100,
		Dexterity:       5,
		Strength:        5,
		Sleep:           false,
		Angle:           0,
		Weapon:          false,
	}
}

func GenerateLevels(currentLevel int) Level {
	var rooms []Room
	var coridors []Coridor
	var enemys []Enemy
	var items []Item
	var exit Position
	var openRoom []int
	openRoom = append(openRoom, 0)
	for i := 0; i < 9; i++ {
		rooms = append(rooms, GenerateRoom(i))
	}
	rooms[0].Visible = true
	coridors = GenerateCoridors(rooms)
	coridors[0].Cells[0].Visible = true
	coridors[6].Cells[0].Visible = true
	enemys = GenerateEnemy(currentLevel, rooms)
	items = GenerateItems(currentLevel, rooms)
	exit = randPositionExit(rooms[rand.Intn(8)+1])
	return Level{
		ID:       currentLevel,
		Rooms:    rooms,
		Coridors: coridors,
		Enemys:   enemys,
		Items:    items,
		Exit:     exit,
	}
}

func GenerateRoom(numRoom int) Room {
	row := (numRoom) / 3
	col := (numRoom) % 3
	sectorX := col * SectorSize
	sectorY := row * SectorSize
	roomLen := rand.Intn(8) + 6
	roomWidth := rand.Intn(8) + 6
	resX := rand.Intn(SectorSize-roomLen-2) + sectorX + 1
	resY := rand.Intn(SectorSize-roomWidth-2) + sectorY + 1
	return Room{
		ID: numRoom,
		StartPosition: Position{
			X: resX,
			Y: resY,
		},
		Lenght: roomLen,
		Width:  roomWidth,
	}
}

func GenerateCoridors(rooms []Room) []Coridor {
	resPositions := []Position{}
	res := []Coridor{}
	for i := 0; i < 8; {
		for j := 0; j < 2; j++ {
			resPositions = BuildCoridor(rooms, i, i+1, "horizontal")
			res = append(res, Coridor{
				FromRoom: i,
				ToRoom:   i + 1,
				Cells:    resPositions,
			})
			i++
		}
		i++
	}
	for i := 0; i < 6; i++ {
		resPositions = BuildCoridor(rooms, i, i+3, "vertical")
		res = append(res, Coridor{
			FromRoom: i,
			ToRoom:   i + 3,
			Cells:    resPositions,
		})

	}
	return res
}

func BuildCoridor(rooms []Room, from int, to int, corridorType string) []Position {
	res := []Position{}
	switch corridorType {
	case "horizontal":

		startPositionX := rooms[from].StartPosition.X + rooms[from].Lenght - 1
		startPositionY := rand.Intn(rooms[from].Width-2) + rooms[from].StartPosition.Y + 1
		endPositionX := rooms[to].StartPosition.X
		endPositionY := rand.Intn(rooms[to].Width-2) + rooms[to].StartPosition.Y + 1

		for startPositionX != endPositionX {
			res = append(res, Position{X: startPositionX, Y: startPositionY})
			startPositionX++
		}
		startPositionX--

		for startPositionY != endPositionY {
			if startPositionY > endPositionY {
				startPositionY--
			} else {
				startPositionY++
			}
			res = append(res, Position{X: startPositionX, Y: startPositionY})
		}
		res = append(res, Position{X: endPositionX, Y: endPositionY})

	case "vertical":

		startPositionX := rand.Intn(rooms[from].Lenght-2) + rooms[from].StartPosition.X + 1
		startPositionY := rooms[from].StartPosition.Y + rooms[from].Width - 1
		endPositionX := rand.Intn(rooms[to].Lenght-2) + rooms[to].StartPosition.X + 1
		endPositionY := rooms[to].StartPosition.Y

		for startPositionY != endPositionY {
			res = append(res, Position{X: startPositionX, Y: startPositionY})
			startPositionY++
		}
		startPositionY--

		for startPositionX != endPositionX {
			if startPositionX > endPositionX {
				startPositionX--
			} else {
				startPositionX++
			}
			res = append(res, Position{X: startPositionX, Y: startPositionY})
		}
		res = append(res, Position{X: endPositionX, Y: endPositionY})
	}
	return res
}

func GenerateEnemy(currentLevel int, rooms []Room) []Enemy {
	var res []Enemy
	var countE int = rand.Intn(3) + currentLevel
	for i := 0; i < countE; i++ {
		typeE := randTypeEnemy(currentLevel)
		room := rand.Intn(8) + 1
		positionE := randPosition(rooms[room])
		oneEnemy := createEnemy(typeE, positionE)
		res = append(res, oneEnemy)
	}
	return res
}

func createEnemy(typeE EnemyType, pos Position) Enemy {
	var res Enemy
	switch typeE {
	case EnemyZombie:
		res = Enemy{
			Type:       typeE,
			Position:   pos,
			Health:     120,
			Dexterity:  3,
			Strength:   6,
			Hostility:  1,
			IsAttacked: false,
			Aggresive:  false,
		}
	case EnemyVampire:
		res = Enemy{
			Type:       typeE,
			Position:   pos,
			Health:     100,
			Dexterity:  9,
			Strength:   6,
			Hostility:  2,
			IsAttacked: false,
			Aggresive:  false,
		}
	case EnemyGhost:
		res = Enemy{
			Type:       typeE,
			Position:   pos,
			Health:     50,
			Dexterity:  10,
			Strength:   3,
			Hostility:  0,
			IsAttacked: false,
			Aggresive:  false,
		}
	case EnemyOgre:
		res = Enemy{
			Type:       typeE,
			Position:   pos,
			Health:     180,
			Dexterity:  2,
			Strength:   12,
			Hostility:  1,
			IsAttacked: false,
			Aggresive:  false,
		}
	case EnemySnakeMage:
		res = Enemy{
			Type:       typeE,
			Position:   pos,
			Health:     70,
			Dexterity:  12,
			Strength:   5,
			Hostility:  3,
			IsAttacked: false,
			Aggresive:  false,
		}
	}
	return res
}

func GenerateItems(currentLevel int, rooms []Room) []Item {
	var res []Item
	var countI int = 13 - currentLevel/2
	if countI < 4 {
		countI = 4
	}
	for i := 0; i < countI; i++ {
		typeItem, subTypeItem := randItem()
		room := rand.Intn(8) + 1
		pos := randPosition(rooms[room])
		item := generateItem(typeItem, subTypeItem, pos)
		res = append(res, item)
	}
	return res
}

func generateItem(typeI ItemType, subTI ItemSubtype, pos Position) Item {
	var res Item
	switch typeI {
	case ItemWeapon:
		switch subTI {
		case KnifeWeapon:
			res = Item{Type: typeI, Subtype: subTI, Strength: 4, Position: pos}
		case SwordWeapon:
			res = Item{Type: typeI, Subtype: subTI, Strength: 8, Position: pos}
		case AxeWeapon:
			res = Item{Type: typeI, Subtype: subTI, Strength: 12, Position: pos}
		case OnePunchWeapon:
			res = Item{Type: typeI, Subtype: subTI, Strength: 50, Position: pos}
		}
	case ItemFood:
		res = Item{Type: typeI, Subtype: subTI, Heal: rand.Intn(10) + 31, Position: pos}
	case ItemElixir:
		res = Item{Type: typeI, Subtype: subTI, Heal: 80, Position: pos}
	case ItemScroll:
		switch subTI {
		case HealthScroll:
			res = Item{Type: typeI, Subtype: subTI, AddHeal: 15, Position: pos}
		case DexterityScroll:
			res = Item{Type: typeI, Subtype: subTI, AddDexterity: 1, Position: pos}
		case StrengthScroll:
			res = Item{Type: typeI, Subtype: subTI, AddStrength: 2, Position: pos}
		}
	}
	return res
}
