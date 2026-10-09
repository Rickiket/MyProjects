package domain

import "math/rand"

func (g *GameSession) MovePlayer(x, y int) bool { //атакуем или нет
	newX := x + g.Player.CurrentPosition.X
	newY := y + g.Player.CurrentPosition.Y
	flag := false
	res := false
	for idx1, i := range g.Levels[g.CurrentLevel].Coridors {
		for idx2, j := range i.Cells {
			if newX == j.X &&
				newY == j.Y {
				flag = true
				g.Player.CurrentRoom = 10
				g.VisibleCoridorCell(idx1, idx2+1)
				g.VisibleCoridorCell(idx1, idx2-1)
				break
			}
		}
	}
	if !flag {
		for idx, i := range g.Levels[g.CurrentLevel].Rooms {
			if newX > i.StartPosition.X &&
				newX < i.StartPosition.X+i.Lenght-1 &&
				newY > i.StartPosition.Y &&
				newY < i.StartPosition.Y+i.Width-1 {
				flag = true
				g.Player.CurrentRoom = idx
				if i.Visible == false {
					g.Levels[g.CurrentLevel].Rooms[idx].Visible = true
					g.FindCoridors(idx)
					break
				}
			}
		}
	}
	for _, i := range g.Levels[g.CurrentLevel].Enemys {
		if newX == i.Position.X &&
			newY == i.Position.Y {
			res = true
			flag = false
		}
	}
	if flag {
		g.Player.CurrentPosition.X = g.Player.CurrentPosition.X + x
		g.Player.CurrentPosition.Y = g.Player.CurrentPosition.Y + y
		g.Inf["Cell"]++
	}
	return res
}

func (g *GameSession) AttackEnemy(x, y int, bp *Backpack) {
	var chance int
	var indEnemy int
	for ind, i := range g.Levels[g.CurrentLevel].Enemys {
		if x == i.Position.X && y == i.Position.Y {
			indEnemy = ind
			break
		}
	}
	chance = 75 + (g.Player.Dexterity-g.Levels[g.CurrentLevel].Enemys[indEnemy].Dexterity)*5
	if g.Levels[g.CurrentLevel].Enemys[indEnemy].Type == EnemyVampire && g.Levels[g.CurrentLevel].Enemys[indEnemy].IsAttacked == true {
		if chance >= rand.Intn(100)+1 {
			if g.Player.Weapon == true {
				g.Levels[g.CurrentLevel].Enemys[indEnemy].Health = g.Levels[g.CurrentLevel].Enemys[indEnemy].Health - (g.Player.Strength + g.Player.CurrentWeapon.Strength)
			} else {
				g.Levels[g.CurrentLevel].Enemys[indEnemy].Health = g.Levels[g.CurrentLevel].Enemys[indEnemy].Health - g.Player.Strength
			}
			g.Inf["HitEnemy"]++
		}
	} else if g.Levels[g.CurrentLevel].Enemys[indEnemy].Type != EnemyVampire {
		if chance >= rand.Intn(100)+1 {
			if g.Player.CurrentWeapon.Type != "" {
				g.Levels[g.CurrentLevel].Enemys[indEnemy].Health = g.Levels[g.CurrentLevel].Enemys[indEnemy].Health - (g.Player.Strength + g.Player.CurrentWeapon.Strength)
			} else {
				g.Levels[g.CurrentLevel].Enemys[indEnemy].Health = g.Levels[g.CurrentLevel].Enemys[indEnemy].Health - g.Player.Strength
			}
			g.Inf["HitEnemy"]++
		}
	}
	g.Levels[g.CurrentLevel].Enemys[indEnemy].IsAttacked = true
	if g.Levels[g.CurrentLevel].Enemys[indEnemy].Health <= 0 {
		switch g.Levels[g.CurrentLevel].Enemys[indEnemy].Type {
		case EnemyZombie:
			bp.Treasures++
		case EnemyVampire:
			bp.Treasures = bp.Treasures + 2
		case EnemyGhost:
			bp.Treasures = bp.Treasures + 3
		case EnemyOgre:
			bp.Treasures = bp.Treasures + 4
		case EnemySnakeMage:
			bp.Treasures = bp.Treasures + 5
		}
		g.Levels[g.CurrentLevel].Enemys = append(g.Levels[g.CurrentLevel].Enemys[:indEnemy], g.Levels[g.CurrentLevel].Enemys[indEnemy+1:]...)
		g.Inf["KillEnemy"]++
	}
}

func (g *GameSession) VisibleCoridorCell(idxCor int, idxCell int) {
	if idxCell < len(g.Levels[g.CurrentLevel].Coridors[idxCor].Cells) && idxCell >= 0 {
		g.Levels[g.CurrentLevel].Coridors[idxCor].Cells[idxCell].Visible = true
	}
}

func (g *GameSession) FindCoridors(idRoom int) {
	for idx, i := range g.Levels[g.CurrentLevel].Coridors {
		if i.FromRoom == idRoom {
			g.VisibleCoridorCell(idx, 0)
		} else if i.ToRoom == idRoom {
			g.VisibleCoridorCell(idx, len(g.Levels[g.CurrentLevel].Coridors[idx].Cells)-1)
		}
	}
}
