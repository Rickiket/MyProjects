package storage

import (
	"encoding/json"
	"os"
	"path/filepath"
	"rogue/domain"
	"sort"
)

const SavePath = "../storage/saves/game.json"
const Records = "../storage/records/records.json"

type SaveData struct {
	Session  *domain.GameSession
	Backpack *domain.Backpack
}

type RunStatistic struct {
	Treasures       int
	CompletetLevels int
	KilledEnemys    int
	CraversedCells  int
	KicksForEnemys  int
	KicksForPlayer  int
	UsedFoods       int
	UsedElixirs     int
	UsedScrolls     int
}

func SaveGame(g *domain.GameSession, bp *domain.Backpack) error {
	var tmpData = SaveData{
		Session:  g,
		Backpack: bp,
	}
	data, err := json.MarshalIndent(tmpData, "", " ")
	if err != nil {
		return err
	}
	if err = os.MkdirAll(filepath.Dir(SavePath), 0755); err != nil {
		return err
	}
	return os.WriteFile(SavePath, data, 0644)
}

func LoadGame() (*domain.GameSession, *domain.Backpack, error) {
	data, err := os.ReadFile(SavePath)
	if err != nil {
		return nil, nil, err
	}
	var game SaveData
	if err = json.Unmarshal(data, &game); err != nil {
		return nil, nil, err
	}
	return game.Session, game.Backpack, nil
}

func LoadRecords() ([]RunStatistic, error) {
	tmpData, err := os.ReadFile(Records)
	if os.IsNotExist(err) {
		return []RunStatistic{}, nil
	} else if err != nil {
		return nil, err
	}
	var stat []RunStatistic
	if err = json.Unmarshal(tmpData, &stat); err != nil {
		return nil, err
	}
	return stat, nil
}
func SaveRecord(g *domain.GameSession, bp *domain.Backpack) error {
	oldStat, err := LoadRecords()
	if err != nil {
		return err
	}
	newStat := RunStatistic{
		Treasures:       bp.Treasures,
		CompletetLevels: g.CurrentLevel,
		KilledEnemys:    g.Inf["KillEnemy"],
		CraversedCells:  g.Inf["Cell"],
		KicksForEnemys:  g.Inf["HitEnemy"],
		KicksForPlayer:  g.Inf["HitPlayer"],
		UsedFoods:       g.Inf["Food"],
		UsedElixirs:     g.Inf["Elixir"],
		UsedScrolls:     g.Inf["Scroll"],
	}
	if zeroRecord(newStat) {
		return nil
	}
	if len(oldStat) >= 6 {
		for idx, i := range oldStat {
			if newStat.Treasures > i.Treasures {
				copyStats(newStat, oldStat, idx)
				break
			} else if newStat.Treasures > i.Treasures &&
				newStat.CompletetLevels > i.CompletetLevels {
				copyStats(newStat, oldStat, idx)
				break
			}
		}
	} else {
		oldStat = append(oldStat, newStat)
		sort.Slice(oldStat, func(i, j int) bool {
			if oldStat[i].Treasures == oldStat[j].Treasures {
				return oldStat[i].CompletetLevels > oldStat[j].CompletetLevels
			}
			return oldStat[i].Treasures > oldStat[j].Treasures
		})
	}
	var data []byte
	data, err = json.MarshalIndent(oldStat, "", " ")
	if err != nil {
		return err
	}
	if err = os.MkdirAll(filepath.Dir(Records), 0755); err != nil {
		return err
	}
	return os.WriteFile(Records, data, 0644)
}

func copyStats(newStat RunStatistic, slice []RunStatistic, idx int) {
	slice[idx].Treasures = newStat.Treasures
	slice[idx].CompletetLevels = newStat.CompletetLevels
	slice[idx].KilledEnemys = newStat.KilledEnemys
	slice[idx].CraversedCells = newStat.CraversedCells
	slice[idx].KicksForEnemys = newStat.KicksForEnemys
	slice[idx].KicksForPlayer = newStat.KicksForPlayer
	slice[idx].UsedFoods = newStat.UsedFoods
	slice[idx].UsedElixirs = newStat.UsedElixirs
	slice[idx].UsedScrolls = newStat.UsedScrolls
}

func zeroRecord(r RunStatistic) bool {
	if r.Treasures == 0 &&
		r.CompletetLevels == 0 &&
		r.KilledEnemys == 0 &&
		r.CraversedCells == 0 &&
		r.KicksForEnemys == 0 &&
		r.KicksForPlayer == 0 &&
		r.UsedFoods == 0 &&
		r.UsedElixirs == 0 &&
		r.UsedScrolls == 0 {
		return true
	}
	return false
}
