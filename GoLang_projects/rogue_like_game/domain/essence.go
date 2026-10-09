package domain

type GameSession struct {
	CurrentLevel int
	Levels       []Level
	Player       *Character
	State        GameState
	Logs         []string
	Inf          map[string]int //cell|, killEnemy|, hitAEnemy|, hitAPlayer|, elixir, scroll, food, -- lvl|, treasure|
	ThreeD       bool
	WorldMap     [][]string
}
type GameState string

const (
	Play            GameState = "playing"
	Choise          GameState = "choise"
	InventoryFood   GameState = "inventoryfood"
	InventoryElixir GameState = "inventoryelixir"
	InventoryScroll GameState = "inventoryscroll"
	GameOver        GameState = "gameover"
	ViewRecors      GameState = "records"
)

type Position struct {
	X       int
	Y       int
	Visible bool
}

type Level struct {
	ID       int
	Rooms    []Room
	Coridors []Coridor
	Enemys   []Enemy
	Items    []Item
	Exit     Position
}

type Coridor struct {
	FromRoom int
	ToRoom   int
	Cells    []Position
}

const (
	MapLenAndWidth int = 60
	SectorSize     int = 20
)

type Room struct {
	ID            int
	StartPosition Position
	Lenght        int
	Width         int
	Visible       bool
}

type Character struct {
	CurrentPosition Position
	CurrentRoom     int
	MaxHeal         int
	CurrentHeal     int
	Dexterity       int
	Strength        int
	CurrentWeapon   Item
	Sleep           bool
	Angle           float64
	Weapon          bool
}

type Backpack struct {
	Foods     []Item
	Elixirs   []Item
	Scrolls   []Item
	Treasures int
	Limit     int
}

type EnemyType string

const (
	EnemyZombie    EnemyType = "zombie"
	EnemyVampire   EnemyType = "vampire"
	EnemyGhost     EnemyType = "ghost"
	EnemyOgre      EnemyType = "ogre"
	EnemySnakeMage EnemyType = "snake-mage"
)

type Enemy struct {
	Type       EnemyType
	Position   Position
	Health     int
	Dexterity  int
	Strength   int
	Hostility  int
	Invisible  bool
	IsAttacked bool
	Aggresive  bool
	Sleep      bool
}

type ItemType string

const (
	ItemWeapon   ItemType = "Weapon"
	ItemFood     ItemType = "Food"
	ItemElixir   ItemType = "Elixir"
	ItemScroll   ItemType = "Scroll"
	ItemTreasure ItemType = "Treasure"
)

type ItemSubtype string

const (
	SwordWeapon     ItemSubtype = "sword"
	AxeWeapon       ItemSubtype = "axe"
	KnifeWeapon     ItemSubtype = "knife"
	OnePunchWeapon  ItemSubtype = "onePunch"
	Food            ItemSubtype = "Food"
	HealthElixir    ItemSubtype = "Health Elixir"
	HealthScroll    ItemSubtype = "Health Scroll"
	DexterityScroll ItemSubtype = "Dexterity Scroll"
	StrengthScroll  ItemSubtype = "Strength Scroll"
)

type Item struct {
	Type         ItemType
	Subtype      ItemSubtype
	Strength     int
	Heal         int
	AddHeal      int
	AddDexterity int
	AddStrength  int
	Value        int
	Position     Position
}
