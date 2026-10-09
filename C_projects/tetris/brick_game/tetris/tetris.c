#include "../../game.h"

static GameInfo_t game;
static Figures figures;
static gameState state;

GameInfo_t updateCurrentState() {
  GameInfo_t tempStruct;
  tempStruct.tempField = createField(20, 10);
  for (int i = 0; i < 20; i++) {
    for (int j = 0; j < 10; j++) {
      tempStruct.tempField[i][j] = game.tempField[i][j];
    }
  }
  tempStruct.field = createField(20, 10);
  for (int i = 0; i < 20; i++) {
    for (int j = 0; j < 10; j++) {
      tempStruct.field[i][j] = game.field[i][j];
    }
  }
  tempStruct.figure = createField(4, 4);
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      tempStruct.figure[i][j] = game.figure[i][j];
    }
  }
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      tempStruct.tempFigureUpend[i][j] = game.tempFigureUpend[i][j];
    }
  }
  tempStruct.positionFigureY = game.positionFigureY;
  tempStruct.positionFigureX = game.positionFigureX;
  tempStruct.next = createField(4, 4);
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      tempStruct.next[i][j] = game.next[i][j];
    }
  }
  tempStruct.score = game.score;
  tempStruct.high_score = game.high_score;
  tempStruct.level = game.level;
  return tempStruct;
}

void lineCheck() {
  int num = 1;
  int lines = 0;
  for (int i = 0; i <= 19; i++) {
    for (int j = 0; j <= 9; j++) {
      num = num * game.field[i][j];
    }
    if (num == 1) {
      deleteLine(i);
      lines++;
      i--;
    }
    num = 1;
  }
  switch (lines) {
    case 1:
      game.score += 100;
      break;
    case 2:
      game.score += 300;
      break;
    case 3:
      game.score += 500;
      break;
    case 4:
      game.score += 800;
      break;
    default:
      break;
  }
  switch (game.score) {
    case 1000:
      game.level = 2;
      break;
    case 2000:
      game.level = 3;
      break;
    case 3000:
      game.level = 4;
      break;
    case 4000:
      game.level = 5;
      break;
    case 5000:
      game.level = 6;
      break;
    case 6000:
      game.level = 7;
      break;
    case 7000:
      game.level = 8;
      break;
    case 8000:
      game.level = 9;
      break;
    case 9000:
      game.level = 10;
      break;
  }
}

void deleteLine(int line) {
  if (line == 0) {
    for (int j = 0; j <= 9; j++) {
      game.field[line][j] = 0;
    }
  } else {
    for (; line > 0; line--) {
      for (int j = 0; j <= 9; j++) {
        game.field[line][j] = game.field[line - 1][j];
      }
    }
    for (int j = 0; j <= 9; j++) {
      game.field[line][j] = 0;
    }
  }
}

void copyField() {
  for (int i = 0; i < 20; i++) {
    for (int j = 0; j < 10; j++) {
      game.tempField[i][j] = game.field[i][j];
    }
  }
}

void initializeGame() {
  FILE *file = fopen("high_score.txt", "r");
  game.field = createField(20, 10);
  game.tempField = createField(20, 10);
  game.figure = randFigure();
  game.next = randFigure();
  game.score = 0;
  fscanf(file, "%d", &game.high_score);
  game.level = 1;
  fclose(file);
  game.positionFigureY = -1;
  game.positionFigureX = 3;
}

int **createField(int lenght, int width) {
  int **field = (int **)calloc(lenght, sizeof(int *));
  for (int i = 0; i < lenght; i++) {
    field[i] = (int *)calloc(width, sizeof(int));
  }
  for (int i = 0; i < lenght; i++) {
    for (int j = 0; j < width; j++) {
      field[i][j] = 0;
    }
  }
  return field;
}

void freeMatrix(int **field, int length) {
  if (field == NULL) return;
  for (int i = 0; i < length; i++) {
    free(field[i]);
  }
  free(field);
}

void initializeFigures() {
  int I[4][4] = {{0, 0, 0, 0}, {1, 1, 1, 1}, {0, 0, 0, 0}, {0, 0, 0, 0}};
  memcpy(figures.I_figure, I, sizeof(I));
  int L1[4][4] = {{0, 0, 0, 0}, {0, 0, 2, 0}, {2, 2, 2, 0}, {0, 0, 0, 0}};
  memcpy(figures.L1_figure, L1, sizeof(L1));
  int L2[4][4] = {{0, 0, 0, 0}, {3, 0, 0, 0}, {3, 3, 3, 0}, {0, 0, 0, 0}};
  memcpy(figures.L2_figure, L2, sizeof(L2));
  int O[4][4] = {{0, 0, 0, 0}, {0, 4, 4, 0}, {0, 4, 4, 0}, {0, 0, 0, 0}};
  memcpy(figures.O_figure, O, sizeof(O));
  int Z1[4][4] = {{0, 0, 0, 0}, {0, 5, 5, 0}, {5, 5, 0, 0}, {0, 0, 0, 0}};
  memcpy(figures.Z1_figure, Z1, sizeof(Z1));
  int Z2[4][4] = {{0, 0, 0, 0}, {6, 6, 0, 0}, {0, 6, 6, 0}, {0, 0, 0, 0}};
  memcpy(figures.Z2_figure, Z2, sizeof(Z2));
  int T[4][4] = {{0, 0, 0, 0}, {7, 7, 7, 0}, {0, 7, 0, 0}, {0, 0, 0, 0}};
  memcpy(figures.T_figure, T, sizeof(T));
}

int **randFigure() {
  int **temp = (int **)calloc(4, sizeof(int *));
  for (int i = 0; i < 4; i++) {
    temp[i] = (int *)calloc(4, sizeof(int));
  }
  int randNum = (rand() % 7) + 1;
  switch (randNum) {
    case 1:
      for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
          temp[i][j] = figures.I_figure[i][j];
        }
      }
      break;
    case 2:
      for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
          temp[i][j] = figures.L1_figure[i][j];
        }
      }
      break;
    case 3:
      for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
          temp[i][j] = figures.L2_figure[i][j];
        }
      }
      break;
    case 4:
      for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
          temp[i][j] = figures.O_figure[i][j];
        }
      }
      break;
    case 5:
      for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
          temp[i][j] = figures.Z1_figure[i][j];
        }
      }
      break;
    case 6:
      for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
          temp[i][j] = figures.Z2_figure[i][j];
        }
      }
      break;
    case 7:
      for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
          temp[i][j] = figures.T_figure[i][j];
        }
      }
      break;
  }
  return temp;
}

int spawnFigure() {
  int flag = 0;
  game.positionFigureY = -1;
  game.positionFigureX = 3;
  if (collision() == 1) {
    flag = 1;
    state.gameOver = 1;
  } else {
    for (int i = 0; i < 4; i++) {
      for (int j = 0; j < 4; j++) {
        if (game.figure[i][j] != 0) {
          game.field[i + game.positionFigureY][j + game.positionFigureX] =
              game.figure[i][j];
        }
      }
    }
  }
  return flag;
}

int collision() {
  int flag = 0;
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      if (game.figure[i][j] != 0) {
        int newY = i + game.positionFigureY;
        int newX = j + game.positionFigureX;
        if (newY < 0 || newY > 19 || newX < 0 || newX > 9) {
          flag = 1;
        } else if (game.tempField[newY][newX] != 0) {
          flag = 1;
        }
      }
    }
  }
  return flag;
}

int collisionUpend() {
  int flag = 0;
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      if (game.tempFigureUpend[i][j] != 0) {
        int newY = i + game.positionFigureY;
        int newX = j + game.positionFigureX;
        if (newY < 0 || newY > 19 || newX < 0 || newX > 9) {
          flag = 1;
        } else if (game.tempField[newY][newX] != 0) {
          flag = 1;
        }
      }
    }
  }
  return flag;
}

int down() {
  int flag = 0;
  game.positionFigureY++;
  if (collision() == 1) {
    game.positionFigureY--;
    flag = 1;
  } else {
    cleanField(game.positionFigureX, game.positionFigureY - 1);
    drop();
  }
  return flag;
}

int left() {
  int flag = 0;
  game.positionFigureX--;
  if (collision() == 1) {
    game.positionFigureX++;
    flag = 1;
  } else {
    cleanField(game.positionFigureX + 1, game.positionFigureY);
    drop();
  }
  return flag;
}

int right() {
  int flag = 0;
  game.positionFigureX++;
  if (collision() == 1) {
    game.positionFigureX--;
    flag = 1;
  } else {
    cleanField(game.positionFigureX - 1, game.positionFigureY);
    drop();
  }
  return flag;
}

void up() {
  game.positionFigureY--;
  if (collision() == 1) {
    game.positionFigureY++;
  }
}
void upend() {
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      game.tempFigureUpend[i][j] = 0;
    }
  }
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      if (game.figure[i][j]) {
        game.tempFigureUpend[j][3 - i] = game.figure[i][j];
      }
    }
  }
  if (collisionUpend() == 0) {
    cleanField(game.positionFigureX, game.positionFigureY);
    for (int i = 0; i < 4; i++) {
      for (int j = 0; j < 4; j++) {
        game.figure[i][j] = game.tempFigureUpend[i][j];
      }
    }
    drop();
  }
}

void drop() {
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      if (game.figure[i][j] != 0) {
        game.field[i + game.positionFigureY][j + game.positionFigureX] = 1;
      }
    }
  }
}

void cleanField(int positionX, int positionY) {
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      if (game.figure[i][j] != 0) {
        game.field[i + positionY][j + positionX] = 0;
      }
    }
  }
}
void finishBackend() {
  freeMatrix(game.field, 20);
  freeMatrix(game.tempField, 20);
  freeMatrix(game.figure, 4);
  freeMatrix(game.next, 4);
}

void replacementFigure() {
  freeMatrix(game.figure, 4);
  game.figure = game.next;
  game.next = randFigure();
}

int speedDetection() {
  int temp = 0;
  switch (game.level) {
    case 1:
      temp = 1000000;
      break;
    case 2:
      temp = 900000;
      break;
    case 3:
      temp = 800000;
      break;
    case 4:
      temp = 700000;
      break;
    case 5:
      temp = 600000;
      break;
    case 6:
      temp = 500000;
      break;
    case 7:
      temp = 400000;
      break;
    case 8:
      temp = 300000;
      break;
    case 9:
      temp = 200000;
      break;
    case 10:
      temp = 100000;
      break;
    default:
      break;
  }
  return temp;
}

void process_input() {
  int ch = getch();
  if (ch != ERR) {
    bool hold = false;
    UserAction_t action = None;

    switch (ch) {
      case KEY_LEFT:
        action = Left;
        break;
      case KEY_RIGHT:
        action = Right;
        break;
      case KEY_DOWN:
        action = Down;
        break;
      case KEY_UP:
        action = Up;
        break;
      case ' ':
        action = Action;
        break;
      case 'p':
        action = Pause;
        break;
      case 'q':
        action = Terminate;
        break;
      case 's':
        action = Start;
        break;
      default:
        break;
    }

    if (action != None) {
      userInput(action, hold);
    }
  }
}

void updateHighScore() {
  if (game.score > game.high_score) {
    FILE *file = fopen("high_score.txt", "w");
    fprintf(file, "%d\n", game.score);
    fclose(file);
  }
}
void game_loop() {
  int flag = 0;
  setlocale(LC_ALL, "");
  initscr();
  cbreak();
  noecho();
  nodelay(stdscr, TRUE);
  keypad(stdscr, TRUE);
  curs_set(0);
  initializeFigures();
  initializeGame();
  initState();
  init_colors();
  if (stateStart()) {
    flag = 1;
  }
  clear();
  if (!flag) {
    draw_static_info();
    calculate();
  }
  updateHighScore();
  finishBackend();
  finishGame();
  stateGameOver();
  nodelay(stdscr, FALSE);
  echo();
  nocbreak();
  curs_set(1);
  endwin();
}

void initState() {
  state.gameOver = 0;
  state.start = 0;
}

void calculate() {
  render();
  while (!state.gameOver) {
    spawnFigure();
    render();
    fallingFigure();
    lineCheck();
    copyField();
    replacementFigure();
  }
}

void fallingFigure() {
  int temp = 0;
  clock_t x = 0;
  clock_t last_drop = clock();
  x = speedDetection();
  while (!temp && !state.gameOver) {
    process_input();
    render();
    if (clock() - last_drop > x) {
      temp = down();
      render();
      last_drop = clock();
    }
    refresh();
  }
}

void userInput(UserAction_t action, bool hold) {
  if (hold == false) {
    switch (action) {
      case Start:
        state.start = 1;
        break;
      case Pause:
        if (statePause()) {
          state.gameOver = 1;
        };
        break;
      case Terminate:
        state.gameOver = 1;
        state.start = 0;
        break;
      case Left:
        left();
        break;
      case Right:
        right();
        break;
      case Up:
        break;
      case Down:
        down();
        break;
      case Action:
        upend();
        break;
    }
  }
}

int getGameOverState() {
  int temp = state.gameOver;
  return temp;
}

int getStartState() {
  int temp = state.start;
  return temp;
}

void fullline(int x) {
  for (int i = 0; i < x; i++) {
    for (int j = 0; j < 10; j++) {
      game.field[i][j] = 1;
    }
  }
}