package render

import (
	"rogue/domain"
	"rogue/storage"
	"strconv"

	"github.com/gdamore/tcell/v2"
)

func NewGame(state int) {
	screen.Clear()
	switch state {
	case 1:
		print(screen, 0, 0, "0 - Продолжить старую игру", tcell.StyleDefault)
		print(screen, 0, 1, "1 - Начать новую игру", tcell.StyleDefault)
	case 2:
		print(screen, 0, 0, "Нажмите Y чтобы начать", tcell.StyleDefault)
	}
	screen.Show()
}

func GameOver(logs []string) {
	start2dX = 0
	screen.Clear()
	drawLogs(logs)
	screen.Show()
}

func Render(g *domain.GameSession, bp *domain.Backpack, rc []storage.RunStatistic) {
	screen.Clear()
	if g.State == domain.ViewRecors {
		for idx, i := range rc {
			drawRecord(i, idx)
		}
		print(screen, 60, 0, "Press Q for exir", tcell.StyleDefault)
		screen.Show()
		return
	}
	if g.ThreeD == true {
		render3D(g)
		start2dX = 100
		start2dY = 61
	} else {
		start2dX = 0
	}
	level := g.Levels[g.CurrentLevel]
	for idx, room := range level.Rooms {
		if room.Visible == true && idx == g.Player.CurrentRoom {
			drawRoom(room)
		} else if room.Visible == true {
			drawWall(room)
		}
	}
	for _, cor := range level.Coridors {
		for _, cell := range cor.Cells {
			if cell.Visible == true {
				draw(
					cell.X+start2dX,
					cell.Y,
					'.',
					tcell.StyleDefault,
				)
			}
		}
	}
	for _, item := range level.Items {
		if visibleRoom(item.Position.X, item.Position.Y, level, g.Player.CurrentRoom) {
			draw(
				item.Position.X+start2dX,
				item.Position.Y,
				'!',
				tcell.StyleDefault.Foreground(tcell.ColorYellow),
			)
		}
	}
	if visibleRoom(level.Exit.X, level.Exit.Y, level, g.Player.CurrentRoom) {
		draw(level.Exit.X+start2dX, level.Exit.Y, '№', tcell.StyleDefault.Foreground(tcell.ColorYellow))
	}
	for _, enemy := range level.Enemys {
		style := tcell.StyleDefault
		var ch rune
		switch enemy.Type {
		case domain.EnemyZombie:
			ch = 'z'
			style = style.Foreground(tcell.ColorGreen)

		case domain.EnemyVampire:
			ch = 'v'
			style = style.Foreground(tcell.ColorRed)

		case domain.EnemyGhost:
			ch = 'g'
			style = style.Foreground(tcell.ColorWhite)

		case domain.EnemyOgre:
			ch = 'O'
			style = style.Foreground(tcell.ColorYellow)

		case domain.EnemySnakeMage:
			ch = 's'
			style = style.Foreground(tcell.ColorWhite)
		}
		if enemy.Invisible == false && visibleRoom(enemy.Position.X, enemy.Position.Y, level, g.Player.CurrentRoom) {
			draw(
				enemy.Position.X+start2dX,
				enemy.Position.Y,
				ch,
				style,
			)
		}
	}
	draw(
		g.Player.CurrentPosition.X+start2dX,
		g.Player.CurrentPosition.Y,
		'@',
		tcell.StyleDefault.Foreground(tcell.NewRGBColor(0, 255, 255)),
	)
	print(screen, 61+start2dX, 0, "HP:", tcell.StyleDefault.Foreground(tcell.ColorGreen))
	print(screen, 65+start2dX, 0, strconv.Itoa(g.Player.CurrentHeal), tcell.StyleDefault)
	print(screen, 68+start2dX, 0, "(", tcell.StyleDefault)
	print(screen, 69+start2dX, 0, strconv.Itoa(g.Player.MaxHeal), tcell.StyleDefault)
	print(screen, 72+start2dX, 0, ")", tcell.StyleDefault)
	print(screen, 61+start2dX, 1, "STR:", tcell.StyleDefault.Foreground(tcell.ColorRed))
	print(screen, 66+start2dX, 1, strconv.Itoa(g.Player.Strength), tcell.StyleDefault)
	print(screen, 70+start2dX, 1, "+", tcell.StyleDefault)
	if g.Player.Weapon == true {
		print(screen, 71+start2dX, 1, strconv.Itoa(g.Player.CurrentWeapon.Strength), tcell.StyleDefault)
	} else {
		print(screen, 71+start2dX, 1, "0", tcell.StyleDefault)
	}
	if g.Player.CurrentWeapon.Type != "" &&
		g.Player.Weapon == false {
		print(screen, 73+start2dX, 1, string(g.Player.CurrentWeapon.Subtype)+" Не в руках", tcell.StyleDefault)
	} else {
		print(screen, 73+start2dX, 1, string(g.Player.CurrentWeapon.Subtype), tcell.StyleDefault)
	}
	print(screen, 61+start2dX, 2, "DEX:", tcell.StyleDefault.Foreground(tcell.ColorBlue))
	print(screen, 66+start2dX, 2, strconv.Itoa(g.Player.Dexterity), tcell.StyleDefault)
	print(screen, 61+start2dX, 4, "====УПРАВЛЕНИЕ====", tcell.StyleDefault)
	print(screen, 61+start2dX, 5, "W - Вверх", tcell.StyleDefault)
	print(screen, 61+start2dX, 6, "S - Вниз", tcell.StyleDefault)
	print(screen, 61+start2dX, 7, "A - Влево", tcell.StyleDefault)
	print(screen, 61+start2dX, 8, "D - Вправо", tcell.StyleDefault)
	print(screen, 61+start2dX, 9, "<- ->  - Поворачиваться", tcell.StyleDefault)
	print(screen, 61+start2dX, 10, "J - Еда", tcell.StyleDefault)
	print(screen, 61+start2dX, 11, "K - Элексиры", tcell.StyleDefault)
	print(screen, 61+start2dX, 12, "E - Свитки", tcell.StyleDefault)
	print(screen, 61+start2dX, 13, "Z - Убрать/взять оружие", tcell.StyleDefault)
	print(screen, 61+start2dX, 14, "| - Завершить игру", tcell.StyleDefault)
	print(screen, 61+start2dX, 15, "R - Открыть таблицу рекордов", tcell.StyleDefault)
	print(screen, 61+start2dX, 16, "V - Переключить вид", tcell.StyleDefault)
	print(screen, 61+start2dX, 17, "==================", tcell.StyleDefault)
	print(screen, 61+start2dX, 18, "Еда - ", tcell.StyleDefault)
	print(screen, 67+start2dX, 18, strconv.Itoa(len(bp.Foods)), tcell.StyleDefault)
	print(screen, 61+start2dX, 19, "Элексиры - ", tcell.StyleDefault)
	print(screen, 72+start2dX, 19, strconv.Itoa(len(bp.Elixirs)), tcell.StyleDefault)
	print(screen, 61+start2dX, 20, "Свитки - ", tcell.StyleDefault)
	print(screen, 70+start2dX, 20, strconv.Itoa(len(bp.Scrolls)), tcell.StyleDefault)
	print(screen, 61+start2dX, 21, "--------------------------", tcell.StyleDefault)
	drawLogs(g.Logs)
	screen.Show()
}

func drawRecord(rc storage.RunStatistic, pos int) {
	var (
		x int
		y int
	)
	switch pos {
	case 1:
		x = 0
		y = 0
	case 2:
		x = 30
		y = 0
	case 3:
		x = 0
		y = 12
	case 4:
		x = 30
		y = 12
	case 5:
		x = 0
		y = 24
	case 6:
		x = 30
		y = 24
	}
	print(screen, x, y, "Record - "+strconv.Itoa(pos), tcell.StyleDefault)
	print(screen, x, y+1, "Treasures - "+strconv.Itoa(rc.Treasures), tcell.StyleDefault)
	print(screen, x, y+2, "Copleted Levels - "+strconv.Itoa(rc.CompletetLevels), tcell.StyleDefault)
	print(screen, x, y+3, "Killed Enemys - "+strconv.Itoa(rc.KilledEnemys), tcell.StyleDefault)
	print(screen, x, y+4, "Craversed Cells - "+strconv.Itoa(rc.CraversedCells), tcell.StyleDefault)
	print(screen, x, y+5, "Kicks For Enemys - "+strconv.Itoa(rc.KicksForEnemys), tcell.StyleDefault)
	print(screen, x, y+6, "Kicks For Player - "+strconv.Itoa(rc.KicksForPlayer), tcell.StyleDefault)
	print(screen, x, y+7, "Used Foods - "+strconv.Itoa(rc.UsedFoods), tcell.StyleDefault)
	print(screen, x, y+8, "Used Elixirs - "+strconv.Itoa(rc.UsedElixirs), tcell.StyleDefault)
	print(screen, x, y+9, "Used Scrolls - "+strconv.Itoa(rc.UsedScrolls), tcell.StyleDefault)
}

func visibleRoom(x, y int, level domain.Level, curRoom int) bool {
	for idx, i := range level.Rooms {
		if x > i.StartPosition.X &&
			x < i.StartPosition.X+i.Lenght-1 &&
			y > i.StartPosition.Y &&
			y < i.StartPosition.Y+i.Width-1 {
			if i.Visible == true && curRoom == idx {
				return true
			}
		}
	}
	return false
}
