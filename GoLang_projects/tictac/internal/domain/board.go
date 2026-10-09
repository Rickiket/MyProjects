package domain

type Board struct {
	Matrix [][]int
}

func NewBoard() Board {
	cells := make([][]int, 3)
	for i := 0; i < 3; i++ {
		cells[i] = make([]int, 3)
		for j := 0; j < 3; j++ {
			cells[i][j] = 0
		}
	}
	return Board{
		Matrix: cells,
	}
}

func (b Board) Copy() Board {
	newCells := make([][]int, len(b.Matrix))

	for i := range b.Matrix {
		newCells[i] = make([]int, len(b.Matrix[i]))

		copy(newCells[i], b.Matrix[i])
	}

	return Board{
		Matrix: newCells,
	}
}
