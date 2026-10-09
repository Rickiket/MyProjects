package render

import (
	"math"
	"rogue/domain"
	"sort"

	"github.com/gdamore/tcell/v2"
)

func render3D(g *domain.GameSession) {
	var enemysItemsExit []renderEnemyItemExit
	var lastEnemyidx int = -1
	var lastItemidx int = -1
	for x := 0; x < renderLenght; x++ {
		renderAngle := g.Player.Angle - fov/2 + (float64(x)/float64(renderLenght))*fov
		dist := 0.0
		var whatIsIt string = "wall"
		var enemyFlag bool = false
		var itemFlag bool = false
		var exitFlag bool = false
		var enemyIdx int = -1 // минус для проверки чтобы не рисовать дальних врагов за ближними
		var itemIdx int = -1
		var enemyDist float64 = 0.0
		var itemDist float64 = 0.0
		var exitDist float64 = 0.0
		for {
			dist += 0.2
			rendX := int(math.Round(math.Cos(renderAngle) * dist))
			rendY := int(math.Round(math.Sin(renderAngle) * dist))
			cellX := g.Player.CurrentPosition.X + rendX
			cellY := g.Player.CurrentPosition.Y + rendY
			if cellX < 0 || cellX >= domain.MapLenAndWidth ||
				cellY < 0 || cellY >= domain.MapLenAndWidth {
				break
			}
			if g.WorldMap[cellX][cellY] == "#" {
				break
			} else if g.WorldMap[cellX][cellY] == "d" {
				whatIsIt = "door"
				break
			}
			if enemyIdx == -1 {
				for idx, i := range g.Levels[g.CurrentLevel].Enemys {
					if cellX == i.Position.X &&
						cellY == i.Position.Y &&
						idx != lastEnemyidx &&
						i.Invisible == false {
						enemyFlag = true
						enemyIdx = idx
						enemyDist = dist
						lastEnemyidx = idx
					}
				}
			}
			if itemIdx == -1 &&
				enemyIdx == -1 {
				for idx, i := range g.Levels[g.CurrentLevel].Items {
					if cellX == i.Position.X &&
						cellY == i.Position.Y &&
						idx != lastItemidx {
						itemFlag = true
						itemIdx = idx
						lastItemidx = idx
						itemDist = dist
					}
				}
			}
			if exitFlag == false &&
				enemyIdx == -1 {
				if cellX == g.Levels[g.CurrentLevel].Exit.X &&
					cellY == g.Levels[g.CurrentLevel].Exit.Y {
					exitDist = dist
					exitFlag = true
				}
			}
		}
		wallHeight := int(float64(renderHeight) / dist)
		celling := renderHeight/2 - wallHeight/2
		floor := renderHeight - celling

		var wall string
		switch {
		case dist < 2:
			wall = "█"
		case dist < 4:
			wall = "▓"
		case dist < 6:
			wall = "▒"
		case dist < 8:
			wall = "░"
		default:
			wall = "."
		}
		for y := 0; y < renderHeight; y++ {
			switch {
			case y < celling:
				print(screen, x, y, "▓", tcell.StyleDefault.Foreground(tcell.ColorBlack))
			case y < floor:
				if whatIsIt == "wall" {
					print(screen, x, y, wall, tcell.StyleDefault)
				} else {
					print(screen, x, y, wall, tcell.StyleDefault.Foreground(tcell.ColorBrown))
				}
			default:
				print(screen, x, y, "■", tcell.StyleDefault.Foreground(tcell.ColorBlack))
			}
		}
		if itemFlag == true {
			itemHeight := int(float64(renderHeight) / itemDist * 0.3)
			itemBottom := renderHeight - (renderHeight/2 - int(float64(renderHeight)/itemDist)/2)
			itemTop := itemBottom - itemHeight
			item := renderEnemyItemExit{
				Idx:    itemIdx,
				Dist:   itemDist,
				Height: itemHeight,
				Bottom: itemBottom,
				Top:    itemTop,
				Type:   "item",
			}
			enemysItemsExit = append(enemysItemsExit, item)
		}
		if exitFlag == true {
			exitHeight := int(float64(renderHeight) / exitDist * 0.4)
			exitBottom := renderHeight - (renderHeight/2 - int(float64(renderHeight)/exitDist)/2)
			exitTop := exitBottom - exitHeight
			exit := renderEnemyItemExit{
				Dist:   exitDist,
				Height: exitHeight,
				Bottom: exitBottom,
				Top:    exitTop,
				Type:   "exit",
			}
			enemysItemsExit = append(enemysItemsExit, exit)
		}
		if enemyFlag == true {
			enemyHeight := int(float64(renderHeight) / enemyDist * 0.6)
			enemyBottom := renderHeight - (renderHeight/2 - int(float64(renderHeight)/enemyDist)/2)
			enemyTop := enemyBottom - enemyHeight
			enemy := renderEnemyItemExit{
				Idx:    enemyIdx,
				Dist:   enemyDist,
				Height: enemyHeight,
				Bottom: enemyBottom,
				Top:    enemyTop,
				Type:   "enemy",
			}
			enemysItemsExit = append(enemysItemsExit, enemy)
		}
	}
	sort.Slice(enemysItemsExit, func(i, j int) bool {
		if enemysItemsExit[i].Dist == enemysItemsExit[j].Dist {
			return enemysItemsExit[i].Type > enemysItemsExit[j].Type
		}
		return enemysItemsExit[i].Dist > enemysItemsExit[j].Dist
	})
	for _, i := range enemysItemsExit {
		var objX, objY int
		var style tcell.Style
		var ch string
		switch i.Type {
		case "enemy":
			objX = g.Levels[g.CurrentLevel].Enemys[i.Idx].Position.X
			objY = g.Levels[g.CurrentLevel].Enemys[i.Idx].Position.Y
			switch g.Levels[g.CurrentLevel].Enemys[i.Idx].Type {
			case domain.EnemyZombie:
				style = tcell.StyleDefault.Foreground(tcell.ColorGreen)
				ch = "Z"
			case domain.EnemyVampire:
				style = tcell.StyleDefault.Foreground(tcell.ColorRed)
				ch = "V"
			case domain.EnemyGhost:
				style = tcell.StyleDefault.Foreground(tcell.ColorWhite)
				ch = "G"
			case domain.EnemyOgre:
				style = tcell.StyleDefault.Foreground(tcell.ColorYellow)
				ch = "O"
			case domain.EnemySnakeMage:
				style = tcell.StyleDefault.Foreground(tcell.ColorWhite)
				ch = "S"
			}
		case "item":
			objX = g.Levels[g.CurrentLevel].Items[i.Idx].Position.X
			objY = g.Levels[g.CurrentLevel].Items[i.Idx].Position.Y
			style = tcell.StyleDefault.Foreground(tcell.ColorGold)
			ch = "!"
		default:
			objX = g.Levels[g.CurrentLevel].Exit.X
			objY = g.Levels[g.CurrentLevel].Exit.Y
			style = tcell.StyleDefault.Foreground(tcell.ColorViolet)
			ch = "╬"
		}

		dx := objX - g.Player.CurrentPosition.X
		dy := objY - g.Player.CurrentPosition.Y
		enemyAngle := math.Atan2(float64(dy), float64(dx))
		diffAngle := enemyAngle - g.Player.Angle
		lenght := i.Height
		for diffAngle > math.Pi {
			diffAngle -= 2 * math.Pi
		}
		for diffAngle < -math.Pi {
			diffAngle += 2 * math.Pi
		}
		middleScreenX := int(float64(renderLenght)/2 + (diffAngle/fov)*(float64(renderLenght)/2))
		for x := middleScreenX - lenght/2; x <= middleScreenX+lenght/2; x++ {
			for y := i.Top; y < i.Bottom; y++ {
				print(screen, x, y, ch, style)
			}
		}
	}
}
