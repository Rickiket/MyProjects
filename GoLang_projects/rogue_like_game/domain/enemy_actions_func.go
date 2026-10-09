package domain

func ActionsEnemys(g *GameSession) {
	for idx, i := range g.Levels[g.CurrentLevel].Enemys {
		if !fight(g, idx) {
			switch i.Type {
			case EnemyZombie:
				g.MoveZomdie(idx)
			case EnemyVampire:
				g.MoveVampire(idx)
			case EnemyGhost:
				g.MoveGhost(idx)
			case EnemyOgre:
				g.MoveOgre(idx)
				g.MoveOgre(idx)
			case EnemySnakeMage:
				g.MoveSnake(idx)
			}
		} else {
			switch i.Type {
			case EnemyZombie:
				g.AttackZomdie_Vampire_Ogre(idx)
			case EnemyVampire:
				g.AttackZomdie_Vampire_Ogre(idx)
			case EnemyGhost:
				g.AttackGhost(idx)
			case EnemyOgre:
				g.AttackZomdie_Vampire_Ogre(idx)
			case EnemySnakeMage:
				g.AttackSnake(idx)
			}
		}
	}
	if g.Player.CurrentHeal <= 0 {
		g.State = GameOver
	}
}

func fight(g *GameSession, idx int) bool {
	var (
		room   Room
		enemy  Enemy = g.Levels[g.CurrentLevel].Enemys[idx]
		inRoom bool  = false
	)
	for _, room = range g.Levels[g.CurrentLevel].Rooms {
		if enemy.Position.X > room.StartPosition.X &&
			enemy.Position.X < room.StartPosition.X+room.Lenght-1 &&
			enemy.Position.Y > room.StartPosition.Y &&
			enemy.Position.Y < room.StartPosition.Y+room.Width-1 &&
			g.Player.CurrentPosition.X > room.StartPosition.X &&
			g.Player.CurrentPosition.X < room.StartPosition.X+room.Lenght-1 &&
			g.Player.CurrentPosition.Y > room.StartPosition.Y &&
			g.Player.CurrentPosition.Y < room.StartPosition.Y+room.Width-1 {
			inRoom = true
			break
		} else {
			g.Levels[g.CurrentLevel].Enemys[idx].Aggresive = false
		}
	}
	if (inRoom && g.Levels[g.CurrentLevel].Enemys[idx].IsAttacked) || g.Levels[g.CurrentLevel].Enemys[idx].Aggresive {
		return true
	} else if inRoom {
		if g.Player.CurrentPosition.X >= enemy.Position.X-enemy.Hostility &&
			g.Player.CurrentPosition.X <= enemy.Position.X+enemy.Hostility &&
			g.Player.CurrentPosition.Y >= enemy.Position.Y-enemy.Hostility &&
			g.Player.CurrentPosition.Y <= enemy.Position.Y+enemy.Hostility {
			g.Levels[g.CurrentLevel].Enemys[idx].Aggresive = true
			return true
		}
	}
	return false
}
