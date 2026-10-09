package render

import (
	"math"

	"github.com/gdamore/tcell/v2"
)

const renderLenght int = 100
const renderHeight int = 60
const fov = math.Pi / 3

var screen tcell.Screen
var start2dX int = 0
var start2dY int = 0
var wMap [][]string

type renderEnemyItemExit struct {
	Idx    int
	Dist   float64
	Height int
	Top    int
	Bottom int
	Type   string
}

func Init() error {
	var err error
	screen, err = tcell.NewScreen()
	if err != nil {
		return err
	}
	err = screen.Init()
	if err != nil {
		return err
	}
	screen.Clear()
	return nil
}

func Close() {
	if screen != nil {
		screen.Fini()
	}
}

func ReadInput() string {
	for {
		ev := screen.PollEvent()
		if e, ok := ev.(*tcell.EventKey); ok {
			switch e.Rune() {
			case 'w', 'W', 'ц', 'Ц':
				return "w"
			case 'a', 'A', 'ф', 'Ф':
				return "a"
			case 's', 'S', 'ы', 'Ы':
				return "s"
			case 'd', 'D', 'в', 'В':
				return "d"
			case 'y', 'Y', 'н', 'Н':
				return "y"
			case 'n', 'N', 'т', 'Т':
				return "n"
			case 'q', 'Q', 'й', 'Й':
				return "q"
			case 'j', 'J', 'о', 'О':
				return "j"
			case 'k', 'K', 'л', 'Л':
				return "k"
			case 'e', 'E', 'у', 'У':
				return "e"
			case 'r', 'R', 'к', 'К':
				return "r"
			case 'v', 'V', 'м', 'М':
				return "v"
			case 'z', 'Z', 'я', 'Я':
				return "z"
			case '\\', '|', 'ё', 'Ё':
				return "|"
			case '0':
				return "0"
			case '1':
				return "1"
			case '2':
				return "2"
			case '3':
				return "3"
			case '4':
				return "4"
			case '5':
				return "5"
			case '6':
				return "6"
			case '7':
				return "7"
			case '8':
				return "8"
			}
			switch e.Key() {
			case tcell.KeyLeft:
				return "left"
			case tcell.KeyRight:
				return "right"
			}
		}
	}
}
