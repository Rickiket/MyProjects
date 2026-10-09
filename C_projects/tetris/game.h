#ifndef GAME_H
#define GAME_H

#include <locale.h>
#include <math.h>
#include <ncurses.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define None 0
#define BLOCK "\u2588\u2588"
#define GRID "\u2591\u2591"

typedef struct {
  int **tempField;
  int **field;
  int **figure;
  int tempFigureUpend[4][4];
  int positionFigureY;
  int positionFigureX;
  int **next;
  int score;
  int high_score;
  int level;
} GameInfo_t;

typedef struct {
  int I_figure[4][4];
  int L1_figure[4][4];
  int L2_figure[4][4];
  int O_figure[4][4];
  int Z1_figure[4][4];
  int Z2_figure[4][4];
  int T_figure[4][4];
} Figures;

typedef struct {
  int start;
  int gameOver;
} gameState;

enum Colors {
  COLOR_I = 1,
  COLOR_J,
  COLOR_L,
  COLOR_O,
  COLOR_S,
  COLOR_Z,
  COLOR_T,
  COLOR_GRID
};

typedef enum {
  Start,
  Pause,
  Terminate,
  Left,
  Right,
  Up,
  Down,
  Action
} UserAction_t;

void calculate();
void fallingFigure();
void finishGame();
void userInput(UserAction_t action, bool hold);
void initState();

GameInfo_t updateCurrentState();
void lineCheck();
void deleteLine(int line);
void copyField();
void initializeGame();
int **createField(int lenght, int width);
void freeMatrix(int **field, int length);
void initializeFigures();
int **randFigure();
int spawnFigure();
int collision();
int collisionUpend();
int down();
int left();
int right();
void up();
void upend();
void drop();
void cleanField(int positionX, int positionY);
void finishBackend();
int speedDetection();
void process_input();
void updateHighScore();
void replacementFigure();
void game_loop();

void draw_block(int y, int x, int color_type);
void draw_game_field(GameInfo_t tetrisGame);
void draw_current_figure(GameInfo_t tetrisGame);
void init_colors();
void draw_static_info();
void draw_info(GameInfo_t tetrisGame);
int statePause();
int stateStart();
void stateGameOver();
void render();

int getStartState();
int getGameOverState();
void fullline(int x);

#endif  // GAME_H