package render

import (
	"rogue/domain"

	"github.com/gdamore/tcell/v2"
)

func print(screen tcell.Screen, x, y int, txt string, style tcell.Style) {
	if screen == nil {
		return
	}
	for _, i := range txt {
		screen.SetContent(x, y, i, nil, style)
		x++
	}
}

func draw(x, y int, ch rune, style tcell.Style) {
	if screen == nil {
		return
	}
	screen.SetContent(
		x,
		y,
		ch,
		nil,
		style,
	)
}

func drawWall(room domain.Room) {
	for y := room.StartPosition.Y; y < room.StartPosition.Y+room.Width; y++ {
		for x := room.StartPosition.X; x < room.StartPosition.X+room.Lenght; x++ {

			ch := '#'

			if y == room.StartPosition.Y ||
				y == room.StartPosition.Y+room.Width-1 ||
				x == room.StartPosition.X ||
				x == room.StartPosition.X+room.Lenght-1 {
				draw(x+start2dX, y, ch, tcell.StyleDefault)
			}
		}
	}
}

func drawRoom(room domain.Room) {
	for y := room.StartPosition.Y; y < room.StartPosition.Y+room.Width; y++ {
		for x := room.StartPosition.X; x < room.StartPosition.X+room.Lenght; x++ {

			ch := '.'

			if y == room.StartPosition.Y ||
				y == room.StartPosition.Y+room.Width-1 ||
				x == room.StartPosition.X ||
				x == room.StartPosition.X+room.Lenght-1 {

				ch = '#'
			}

			draw(x+start2dX, y, ch, tcell.StyleDefault)
		}
	}
}

func drawLogs(logs []string) {
	for idx, i := range logs {
		print(screen, 61+start2dX, 22+idx, i, tcell.StyleDefault)
	}
}
