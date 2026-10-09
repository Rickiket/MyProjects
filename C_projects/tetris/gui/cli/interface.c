#include "../../game.h"

static GameInfo_t tetrisGame;

void draw_block(int y, int x, int color_type) {
  if (color_type > 0 && color_type <= 7) {
    attron(COLOR_PAIR(color_type));
    mvprintw(y, x, BLOCK);
    attroff(COLOR_PAIR(color_type));
  } else {
    attron(COLOR_PAIR(COLOR_GRID));
    mvprintw(y, x, GRID);
    attroff(COLOR_PAIR(COLOR_GRID));
  }
}

void draw_game_field(GameInfo_t tetrisGame) {
  for (int i = 0; i < 20; i++) {
    for (int j = 0; j < 10; j++) {
      draw_block(i, j * 2, tetrisGame.field[i][j]);
    }
  }
}

void draw_current_figure(GameInfo_t tetrisGame) {
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      if (tetrisGame.figure[i][j]) {
        int y = i + tetrisGame.positionFigureY;
        int x = (j + tetrisGame.positionFigureX) * 2;
        if (y >= 0 && y < 20 && x >= 0 && x < 20) {
          draw_block(y, x, tetrisGame.figure[i][j]);
        }
      }
    }
  }
}

void init_colors() {
  start_color();
  init_color(COLOR_CYAN, 0, 800, 800);
  init_color(COLOR_BLUE, 0, 0, 800);
  init_color(COLOR_WHITE, 1000, 1000, 1000);
  init_color(COLOR_YELLOW, 1000, 1000, 0);
  init_color(COLOR_GREEN, 0, 800, 0);
  init_color(COLOR_RED, 800, 0, 0);
  init_color(COLOR_MAGENTA, 800, 0, 800);
  init_pair(COLOR_I, COLOR_CYAN, COLOR_CYAN);
  init_pair(COLOR_J, COLOR_BLUE, COLOR_BLUE);
  init_pair(COLOR_L, COLOR_WHITE, COLOR_WHITE);
  init_pair(COLOR_O, COLOR_YELLOW, COLOR_YELLOW);
  init_pair(COLOR_S, COLOR_GREEN, COLOR_GREEN);
  init_pair(COLOR_Z, COLOR_RED, COLOR_RED);
  init_pair(COLOR_T, COLOR_MAGENTA, COLOR_MAGENTA);
  init_pair(COLOR_GRID, COLOR_WHITE, COLOR_BLACK);
}

void draw_static_info() {
  mvprintw(20, 2, "Controls:");
  mvprintw(21, 2, "← → - Move");
  mvprintw(22, 2, "↑ - Nothing");
  mvprintw(23, 2, "↓ - Speed up");
  mvprintw(24, 2, "Space - Rotate");
  mvprintw(25, 2, "P - Pause");
  mvprintw(26, 2, "Q - Quit");
  mvprintw(2, 25, "SCORE:");
  mvprintw(4, 25, "HIGH SCORE:");
  mvprintw(6, 25, "LEVEL:");
  mvprintw(8, 25, "NEXT:");
  refresh();
}

void draw_info(GameInfo_t tetrisGame) {
  mvprintw(3, 25, "%10d", tetrisGame.score);
  mvprintw(5, 25, "%10d", tetrisGame.high_score);
  mvprintw(7, 25, "%10d", tetrisGame.level);
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      int y = 10 + i;
      int x = 27 + j * 2;
      draw_block(y, x, tetrisGame.next[i][j]);
    }
  }
}

int statePause() {
  attron(A_STANDOUT);
  mvprintw(LINES / 2, COLS / 2 - 5, " PAUSED ");
  attroff(A_STANDOUT);
  refresh();
  int ch, flag = 0, stateGameOver = 0;
  ;
  while (!flag) {
    ch = getch();
    if (ch == 'p') {
      clear();
      flag = 1;
      redrawwin(stdscr);
      refresh();
      draw_static_info();
      render();
    }
    if (ch == 'q' || ch == 'Q') {
      clear();
      endwin();
      flag = 1;
      stateGameOver = 1;
    }
    napms(50);
  }
  return stateGameOver;
}

int stateStart() {
  int flag = 0;
  attron(A_STANDOUT);
  mvprintw(LINES / 2, COLS / 2 - 15, " Press SPACE to start game... ");
  attroff(A_STANDOUT);
  refresh();
  int ch;
  while ((ch = getch()) != ' ' && !flag) {
    if (ch == 'q') {
      endwin();
      flag = 1;
    }
    napms(50);
  }
  return flag;
}

void stateGameOver() {
  clear();
  int ch;
  while ((ch = getch()) != 'q') {
    attron(A_STANDOUT);
    mvprintw(LINES / 2, COLS / 2 - 6, " GAME OVER ");
    mvprintw((LINES / 2) + 1, COLS / 2 - 9, " PRESS Q FOR EXIT ");
    attroff(A_STANDOUT);
    refresh();
    napms(50);
  }
}

void render() {
  finishGame();
  tetrisGame = updateCurrentState();
  draw_info(tetrisGame);
  draw_game_field(tetrisGame);
  draw_current_figure(tetrisGame);
}

void finishGame() {
  freeMatrix(tetrisGame.field, 20);
  freeMatrix(tetrisGame.tempField, 20);
  freeMatrix(tetrisGame.figure, 4);
  freeMatrix(tetrisGame.next, 4);
}
