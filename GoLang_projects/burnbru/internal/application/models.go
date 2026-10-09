package application

type About struct {
	Title string `json:"title"`
	Text  string `json:"text"`
}

type Contacts struct {
	Tel  string `json:"tel"`
	Mail string `json:"mail"`
	Inst string `json:"inst"`
	Tg   string `json:"tg"`
}

type Photo struct {
	Filename string `json:"filename"`
	URL      string `json:"url"`
	Width    int    `json:"width,omitempty"`
	Height   int    `json:"height,omitempty"`
}
