package domain

import "math/rand"

func (g *GameSession) MoveEnemy(idx, x, y int) bool { //может ли туда сходить
	var (
		newX int = g.Levels[g.CurrentLevel].Enemys[idx].Position.X + x
		newY int = g.Levels[g.CurrentLevel].Enemys[idx].Position.Y + y
	)
	for _, i := range g.Levels[g.CurrentLevel].Rooms {
		if newX > i.StartPosition.X &&
			newX < i.StartPosition.X+i.Lenght-1 &&
			newY > i.StartPosition.Y &&
			newY < i.StartPosition.Y+i.Width-1 {
			g.Levels[g.CurrentLevel].Enemys[idx].Position.X = newX
			g.Levels[g.CurrentLevel].Enemys[idx].Position.Y = newY
			return true
		}
	}
	return false
}

func (g *GameSession) MoveZomdie(idx int) {
	var (
		rand1 int = rand.Intn(2)
		rand2 int = rand.Intn(2)
	)
	if rand1 == 0 {
		if rand2 == 0 {
			g.MoveEnemy(idx, -1, 0)
		} else {
			g.MoveEnemy(idx, 1, 0)
		}
	} else {
		if rand2 == 0 {
			g.MoveEnemy(idx, 0, -1)
		} else {
			g.MoveEnemy(idx, 0, 1)
		}
	}
}

func (g *GameSession) MoveVampire(idx int) {
	var (
		rand1 int = rand.Intn(2)
		rand2 int = rand.Intn(2)
	)
	if rand1 == 0 {
		if rand2 == 0 {
			if g.MoveEnemy(idx, -1, 0) == false {
				g.MoveEnemy(idx, 1, 0)
			}
		} else {
			if g.MoveEnemy(idx, 1, 0) == false {
				g.MoveEnemy(idx, -1, 0)
			}
		}
	} else {
		if rand2 == 0 {
			if g.MoveEnemy(idx, 0, -1) == false {
				g.MoveEnemy(idx, 0, 1)
			}
		} else {
			if g.MoveEnemy(idx, 0, 1) == false {
				g.MoveEnemy(idx, 0, -1)
			}
		}
	}
}

func (g *GameSession) MoveGhost(idx int) {
	var (
		oldX int = g.Levels[g.CurrentLevel].Enemys[idx].Position.X
		oldY int = g.Levels[g.CurrentLevel].Enemys[idx].Position.Y
	)
	if rand.Intn(100) > 33 {
		g.Levels[g.CurrentLevel].Enemys[idx].Invisible = true
	} else {
		g.Levels[g.CurrentLevel].Enemys[idx].Invisible = false
	}
	for _, i := range g.Levels[g.CurrentLevel].Rooms {
		if oldX > i.StartPosition.X &&
			oldX < i.StartPosition.X+i.Lenght-1 &&
			oldY > i.StartPosition.Y &&
			oldY < i.StartPosition.Y+i.Width-1 {
			randX := rand.Intn(i.Lenght-2) + i.StartPosition.X + 1
			randY := rand.Intn(i.Width-2) + i.StartPosition.Y + 1
			if randX == g.Player.CurrentPosition.X &&
				randY == g.Player.CurrentPosition.Y {
				g.MoveGhost(idx)
			} else {
				g.Levels[g.CurrentLevel].Enemys[idx].Position.X = randX
				g.Levels[g.CurrentLevel].Enemys[idx].Position.Y = randY
			}
		}
	}
}

func (g *GameSession) MoveOgre(idx int) {
	var (
		x    int = g.Levels[g.CurrentLevel].Enemys[idx].Position.X
		y    int = g.Levels[g.CurrentLevel].Enemys[idx].Position.Y
		room Room
	)
	for _, room = range g.Levels[g.CurrentLevel].Rooms {
		if x > room.StartPosition.X &&
			x < room.StartPosition.X+room.Lenght-1 &&
			y > room.StartPosition.Y &&
			y < room.StartPosition.Y+room.Width-1 {
			break
		}
	}
	switch {
	case x+1 == room.StartPosition.X+room.Lenght-1:
		if g.MoveEnemy(idx, 0, 1) == false {
			g.MoveEnemy(idx, -1, 0)
		}
	case y+1 == room.StartPosition.Y+room.Width-1:
		if g.MoveEnemy(idx, -1, 0) == false {
			g.MoveEnemy(idx, 0, -1)
		}
	case x-1 == room.StartPosition.X:
		if g.MoveEnemy(idx, 0, -1) == false {
			g.MoveEnemy(idx, 1, 0)
		}
	case y-1 == room.StartPosition.Y:
		if g.MoveEnemy(idx, 1, 0) == false {
			g.MoveEnemy(idx, 0, 1)
		}
	default:
		g.MoveEnemy(idx, 1, 0)
	}
}

func (g *GameSession) MoveSnake(idx int) {
	var (
		rand1 int = rand.Intn(2)
		rand2 int = rand.Intn(2)
	)
	if rand1 == 0 {
		if rand2 == 0 {
			if g.MoveEnemy(idx, -1, -1) == false {
				g.MoveEnemy(idx, 1, 1)
			}
		} else {
			if g.MoveEnemy(idx, 1, 1) == false {
				g.MoveEnemy(idx, -1, -1)
			}
		}
	} else {
		if rand2 == 0 {
			if g.MoveEnemy(idx, 1, -1) == false {
				g.MoveEnemy(idx, -1, 1)
			}
		} else {
			if g.MoveEnemy(idx, -1, 1) == false {
				g.MoveEnemy(idx, 1, -1)
			}
		}
	}
}

func (g *GameSession) AttackZomdie_Vampire_Ogre(idx int) {
	var enemy Enemy = g.Levels[g.CurrentLevel].Enemys[idx]
	if g.AttackZone(enemy.Position.X, enemy.Position.Y) {
		if (enemy.Position.X-1 == g.Player.CurrentPosition.X && enemy.Position.Y == g.Player.CurrentPosition.Y) ||
			(enemy.Position.X == g.Player.CurrentPosition.X && enemy.Position.Y == g.Player.CurrentPosition.Y-1) ||
			(enemy.Position.X+1 == g.Player.CurrentPosition.X && enemy.Position.Y == g.Player.CurrentPosition.Y) ||
			(enemy.Position.X == g.Player.CurrentPosition.X && enemy.Position.Y == g.Player.CurrentPosition.Y+1) {
			chance := 75 + (enemy.Dexterity-g.Player.Dexterity)*5
			if chance >= rand.Intn(100)+1 && enemy.Sleep == false {
				g.Player.CurrentHeal = g.Player.CurrentHeal - enemy.Strength
				g.Inf["HitPlayer"]++
				if enemy.Type == EnemyVampire {
					g.Player.MaxHeal = g.Player.MaxHeal - rand.Intn(3)
				} else if enemy.Type == EnemyOgre {
					g.Levels[g.CurrentLevel].Enemys[idx].Sleep = true
				}
			} else if enemy.Type == EnemyOgre {
				g.Levels[g.CurrentLevel].Enemys[idx].Sleep = false
			}
		} else if g.Player.CurrentPosition.Y < enemy.Position.Y {
			g.MoveEnemy(idx, 0, -1)
		} else {
			g.MoveEnemy(idx, 0, 1)
		}
	} else {
		switch {
		case enemy.Position.X != g.Player.CurrentPosition.X:
			if g.Player.CurrentPosition.X > enemy.Position.X {
				g.MoveEnemy(idx, 1, 0)
			} else {
				g.MoveEnemy(idx, -1, 0)
			}
		case enemy.Position.Y != g.Player.CurrentPosition.Y:
			if g.Player.CurrentPosition.Y > enemy.Position.Y {
				g.MoveEnemy(idx, 0, 1)
			} else {
				g.MoveEnemy(idx, 0, -1)
			}
		}
	}
}

func (g *GameSession) AttackGhost(idx int) {
	g.Levels[g.CurrentLevel].Enemys[idx].Invisible = false
	var enemy Enemy = g.Levels[g.CurrentLevel].Enemys[idx]
	if (enemy.Position.X-1 == g.Player.CurrentPosition.X && enemy.Position.Y == g.Player.CurrentPosition.Y) ||
		(enemy.Position.X == g.Player.CurrentPosition.X && enemy.Position.Y == g.Player.CurrentPosition.Y-1) ||
		(enemy.Position.X+1 == g.Player.CurrentPosition.X && enemy.Position.Y == g.Player.CurrentPosition.Y) ||
		(enemy.Position.X == g.Player.CurrentPosition.X && enemy.Position.Y == g.Player.CurrentPosition.Y+1) {
		chance := 75 + (enemy.Dexterity-g.Player.Dexterity)*5
		if chance >= rand.Intn(100)+1 {
			g.Player.CurrentHeal = g.Player.CurrentHeal - enemy.Strength
			g.Inf["HitPlayer"]++
		}
	} else {
		var newX int = g.Player.CurrentPosition.X - 1
		for _, i := range g.Levels[g.CurrentLevel].Rooms {
			if enemy.Position.X > i.StartPosition.X &&
				enemy.Position.X < i.StartPosition.X+i.Lenght-1 &&
				enemy.Position.Y > i.StartPosition.Y &&
				enemy.Position.Y < i.StartPosition.Y+i.Width-1 {
				if newX == i.StartPosition.X {
					newX = newX + 2
				}
				break
			}
		}
		g.Levels[g.CurrentLevel].Enemys[idx].Position.X = newX
		g.Levels[g.CurrentLevel].Enemys[idx].Position.Y = g.Player.CurrentPosition.Y
	}
}

func (g *GameSession) AttackSnake(idx int) {
	var enemy Enemy = g.Levels[g.CurrentLevel].Enemys[idx]
	if g.AttackZone(enemy.Position.X, enemy.Position.Y) {
		chance := 75 + (enemy.Dexterity-g.Player.Dexterity)*5
		if chance >= rand.Intn(100)+1 {
			if rand.Intn(100) < 20 {
				g.Player.Sleep = true
			}
			g.Player.CurrentHeal = g.Player.CurrentHeal - enemy.Strength
			g.Inf["HitPlayer"]++
		}
	} else {
		switch {
		case enemy.Position.X > g.Player.CurrentPosition.X && enemy.Position.Y > g.Player.CurrentPosition.Y:
			g.MoveEnemy(idx, -1, -1)
		case enemy.Position.X < g.Player.CurrentPosition.X && enemy.Position.Y > g.Player.CurrentPosition.Y:
			g.MoveEnemy(idx, 1, -1)
		case enemy.Position.X > g.Player.CurrentPosition.X && enemy.Position.Y < g.Player.CurrentPosition.Y:
			g.MoveEnemy(idx, -1, 1)
		case enemy.Position.X < g.Player.CurrentPosition.X && enemy.Position.Y < g.Player.CurrentPosition.Y:
			g.MoveEnemy(idx, 1, 1)
		case enemy.Position.X == g.Player.CurrentPosition.X:
			if enemy.Position.Y < g.Player.CurrentPosition.Y {
				g.MoveEnemy(idx, 0, 1)
			} else {
				g.MoveEnemy(idx, 0, -1)
			}
		case enemy.Position.Y == g.Player.CurrentPosition.Y:
			if enemy.Position.X < g.Player.CurrentPosition.X {
				g.MoveEnemy(idx, 1, 0)
			} else {
				g.MoveEnemy(idx, -1, 0)
			}
		}
	}
}

func (g *GameSession) AttackZone(x, y int) bool {
	if g.Player.CurrentPosition.X >= x-1 &&
		g.Player.CurrentPosition.X <= x+1 &&
		g.Player.CurrentPosition.Y >= y-1 &&
		g.Player.CurrentPosition.Y <= y+1 {
		return true
	}
	return false
}
