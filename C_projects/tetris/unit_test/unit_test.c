#include "unit_test.h"

START_TEST(init_1) {
  initializeFigures();
  initializeGame();
  GameInfo_t test = updateCurrentState();
  ck_assert_int_eq(test.score, 0);
  ck_assert_int_eq(test.level, 1);
  for (int i = 0; i < 20; i++) {
    for (int j = 0; j < 10; j++) {
      ck_assert_int_eq(test.field[i][j], 0);
      ck_assert_int_eq(test.tempField[i][j], 0);
    }
  }
  ck_assert_int_eq(test.positionFigureY, -1);
  ck_assert_int_eq(test.positionFigureX, 3);
  finishBackend();
  freeMatrix(test.field, 20);
  freeMatrix(test.tempField, 20);
  freeMatrix(test.figure, 4);
  freeMatrix(test.next, 4);
}
END_TEST

START_TEST(spawn_1) {
  int flag = 0;
  initializeFigures();
  initializeGame();
  flag = spawnFigure();
  ck_assert_int_eq(flag, 0);
  finishBackend();
}
END_TEST

START_TEST(moving_left) {
  initializeFigures();
  initializeGame();
  spawnFigure();
  left();
  GameInfo_t test = updateCurrentState();
  ck_assert_int_eq(test.positionFigureX, 2);
  finishBackend();
  freeMatrix(test.field, 20);
  freeMatrix(test.tempField, 20);
  freeMatrix(test.figure, 4);
  freeMatrix(test.next, 4);
}
END_TEST

START_TEST(moving_right) {
  initializeFigures();
  initializeGame();
  spawnFigure();
  right();
  GameInfo_t test = updateCurrentState();
  ck_assert_int_eq(test.positionFigureX, 4);
  finishBackend();
  freeMatrix(test.field, 20);
  freeMatrix(test.tempField, 20);
  freeMatrix(test.figure, 4);
  freeMatrix(test.next, 4);
}
END_TEST

START_TEST(moving_down) {
  initializeFigures();
  initializeGame();
  spawnFigure();
  down();
  GameInfo_t test = updateCurrentState();
  ck_assert_int_eq(test.positionFigureY, 0);
  finishBackend();
  freeMatrix(test.field, 20);
  freeMatrix(test.tempField, 20);
  freeMatrix(test.figure, 4);
  freeMatrix(test.next, 4);
}
END_TEST

START_TEST(moving_up) {
  initializeFigures();
  initializeGame();
  spawnFigure();
  down();
  up();
  GameInfo_t test = updateCurrentState();
  ck_assert_int_eq(test.positionFigureY, -1);
  finishBackend();
  freeMatrix(test.field, 20);
  freeMatrix(test.tempField, 20);
  freeMatrix(test.figure, 4);
  freeMatrix(test.next, 4);
}
END_TEST

START_TEST(moving_action) {
  initializeFigures();
  initializeGame();
  GameInfo_t test = updateCurrentState();
  int testFigure[4][4];
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      testFigure[j][3 - i] = test.figure[i][j];
    }
  }
  spawnFigure();
  down();
  upend();
  freeMatrix(test.field, 20);
  freeMatrix(test.tempField, 20);
  freeMatrix(test.figure, 4);
  freeMatrix(test.next, 4);
  test = updateCurrentState();
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      ck_assert_int_eq(testFigure[i][j], test.figure[i][j]);
    }
  }
  finishBackend();
  freeMatrix(test.field, 20);
  freeMatrix(test.tempField, 20);
  freeMatrix(test.figure, 4);
  freeMatrix(test.next, 4);
}
END_TEST

START_TEST(gameover_1) {
  initializeFigures();
  initializeGame();
  while (!spawnFigure()) {
    while (!down()) {
    }
    copyField();
  }
  int test = getGameOverState();
  ck_assert_int_eq(test, 1);
  finishBackend();
}
END_TEST

START_TEST(drop_1) {
  initializeFigures();
  initializeGame();
  int test = speedDetection();
  ck_assert_int_eq(test, 1000000);
  finishBackend();
}
END_TEST

START_TEST(userInput_1) {
  initializeFigures();
  initializeGame();
  bool hold = false;
  UserAction_t action = Down;
  userInput(action, hold);
  GameInfo_t test = updateCurrentState();
  ck_assert_int_eq(test.positionFigureY, 0);
  finishBackend();
  freeMatrix(test.field, 20);
  freeMatrix(test.tempField, 20);
  freeMatrix(test.figure, 4);
  freeMatrix(test.next, 4);
}
END_TEST

START_TEST(line_1) {
  initializeFigures();
  initializeGame();
  fullline(1);
  lineCheck();
  GameInfo_t test = updateCurrentState();
  ck_assert_int_eq(test.score, 100);
  finishBackend();
  freeMatrix(test.field, 20);
  freeMatrix(test.tempField, 20);
  freeMatrix(test.figure, 4);
  freeMatrix(test.next, 4);
}
END_TEST

START_TEST(replacement) {
  initializeFigures();
  initializeGame();
  GameInfo_t test = updateCurrentState();
  int temp[4][4];
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      temp[i][j] = test.next[i][j];
    }
  }
  replacementFigure();
  freeMatrix(test.field, 20);
  freeMatrix(test.tempField, 20);
  freeMatrix(test.figure, 4);
  freeMatrix(test.next, 4);
  test = updateCurrentState();
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      ck_assert_int_eq(temp[i][j], test.figure[i][j]);
    }
  }
  finishBackend();
  freeMatrix(test.field, 20);
  freeMatrix(test.tempField, 20);
  freeMatrix(test.figure, 4);
  freeMatrix(test.next, 4);
}
END_TEST

START_TEST(gamestart_1) {
  initializeFigures();
  initializeGame();
  userInput(Start, false);
  int test = getStartState();
  ck_assert_int_eq(test, 1);
  finishBackend();
}
END_TEST

Suite *tetris_suite() {
  Suite *s = suite_create("tetris_suite");
  TCase *tc = tcase_create("tetris_tc");

  tcase_add_test(tc, init_1);
  tcase_add_test(tc, spawn_1);
  tcase_add_test(tc, moving_left);
  tcase_add_test(tc, moving_right);
  tcase_add_test(tc, moving_down);
  tcase_add_test(tc, moving_up);
  tcase_add_test(tc, moving_action);
  tcase_add_test(tc, gameover_1);
  tcase_add_test(tc, drop_1);
  tcase_add_test(tc, userInput_1);
  tcase_add_test(tc, line_1);
  tcase_add_test(tc, replacement);
  tcase_add_test(tc, gamestart_1);

  suite_add_tcase(s, tc);

  return s;
}

int main() {
  Suite *s = tetris_suite();
  SRunner *sr = srunner_create(s);
  int tf = 0;

  srunner_set_fork_status(sr, CK_NOFORK);
  srunner_run_all(sr, CK_VERBOSE);
  tf = srunner_ntests_failed(sr);
  srunner_free(sr);

  return tf > 0;
}
