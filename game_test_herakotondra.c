#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "game.h"
#include "game_aux.h"
#include "game_ext.h"
#include "game_struct.h"

/* ********** TEST DUMMY ********** */

bool test_dummy() { return true; }

/* ********** TEST GAME SET COLOR ********** */

bool test_game_set_color() {
  game g = game_default();
  game_set_color(g, 2, 2, WHITE);
  game_set_color(g, 4, 1, BLACK);
  game_set_color(g, 1, 3, WHITE);
  game_set_color(g, 1, 3, EMPTY);
  color test1 = game_get_color(g, 2, 2);
  color test2 = game_get_color(g, 4, 1);
  color test3 = game_get_color(g, 1, 3);
  if (test1 == WHITE && test2 == BLACK && test3 == EMPTY) {
    game_delete(g);
    return true;
  }
  game_delete(g);
  return false;
}

/* ********** TEST GAME DELETE ********** */

bool test_game_delete() {
  game g = game_default();
  game_delete(g);
  return true;
}

/* ********** TEST GAME GET COLOR  ********** */

bool test_game_get_color() {
  game g = game_default();
  color test1 = game_get_color(g, 2, 1);
  color test2 = game_get_color(g, 4, 3);
  if (test1 == BLACK || test1 == WHITE || test1 == EMPTY) {
    if (test2 == BLACK || test2 == WHITE || test2 == EMPTY) {
      game_delete(g);
      return true;
    }
  }
  game_delete(g);
  return false;
}

/* ********** TEST GAME GET NEXT SQUARE ********** */

bool test_game_get_next_square() {
  game g = game_default();
  uint x = 0;
  uint y = 0;
  unsigned int *pi_next = &x;
  unsigned int *pj_next = &y;
  bool test2 = game_get_next_square(g, 1, 2, UP, pi_next, pj_next);
  bool test1 = game_get_next_square(g, 4, 4, RIGHT, pi_next, pj_next);
  if (test1 == true && ((*pi_next < 0 || *pi_next >= DEFAULT_SIZE) ||
                        (*pj_next < 0 || *pj_next >= DEFAULT_SIZE))) {
    if (test2 == false && ((*pi_next > 0 && *pi_next < DEFAULT_SIZE) &&
                           (*pj_next > 0 || *pj_next < DEFAULT_SIZE))) {
      game_delete(g);
      return false;
    }
  }
  game_delete(g);
  return true;
}

/* ********** TEST GAME PLAY MOVE ********** */

bool test_game_play_move() {
  game g = game_default();
  game_play_move(g, 1, 3, WHITE);
  game_play_move(g, 2, 1, BLACK);
  game_play_move(g, 1, 1, WHITE);
  game_play_move(g, 1, 1, EMPTY);
  color test1 = game_get_color(g, 1, 3);
  color test2 = game_get_color(g, 2, 1);
  color test3 = game_get_color(g, 1, 1);
  if (test1 == WHITE && test2 == BLACK && test3 == EMPTY) {
    game_delete(g);
    return true;
  }
  game_delete(g);
  return false;
}

/* ********** TEST GAME DEFAULT ********** */

bool test_game_default() {
  game g = game_default();
  constraint x[DEFAULT_SIZE * DEFAULT_SIZE] = {
      0,  -1, -1, 3,  -1, -1, 5,  -1, -1, -1, -1, -1, 4,
      -1, 1,  6,  -1, 6,  3,  -1, -1, -1, -1, -1, -1};
  color y[DEFAULT_SIZE * DEFAULT_SIZE] = {
      EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
      EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
      EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY};
  game g1 = game_new(x, y);
  bool equal = game_equal(g, g1);
  game_delete(g);
  game_delete(g1);
  return equal;
}

/* ********** TEST GAME REDO ********** */

bool test_game_redo() {
  game g = game_default();
  game_play_move(g, 0, 0, BLACK);
  game_undo(g);
  game_redo(g);
  if (game_get_color(g, 0, 0) != BLACK || game_get_status(g, 0, 0) != ERROR) {
    game_delete(g);
    return false;
  }
  game_delete(g);
  return true;
}

/* ********** TEST GAME IS WRAPPING ********** */

bool test_game_is_wrapping() {
  game g1 = game_new_empty_ext(DEFAULT_SIZE, DEFAULT_SIZE, true, FULL);
  game g2 = game_new_empty_ext(DEFAULT_SIZE, DEFAULT_SIZE, false, FULL);
  bool test1 = game_is_wrapping(g1) == true;
  bool test2 = game_is_wrapping(g2) == false;
  game_delete(g1);
  game_delete(g2);
  return test1 && test2;
}

/* ********** TEST GAME NEW EXT ********** */

bool test_game_new_ext() {
  constraint constraints[DEFAULT_SIZE * DEFAULT_SIZE] = {1,
                                                         UNCONSTRAINED,
                                                         2,
                                                         UNCONSTRAINED,
                                                         UNCONSTRAINED,
                                                         UNCONSTRAINED,
                                                         3,
                                                         UNCONSTRAINED,
                                                         4,
                                                         UNCONSTRAINED,
                                                         UNCONSTRAINED,
                                                         UNCONSTRAINED,
                                                         5,
                                                         UNCONSTRAINED,
                                                         UNCONSTRAINED,
                                                         UNCONSTRAINED,
                                                         6,
                                                         UNCONSTRAINED,
                                                         7,
                                                         UNCONSTRAINED,
                                                         UNCONSTRAINED,
                                                         UNCONSTRAINED,
                                                         8,
                                                         UNCONSTRAINED,
                                                         9};
  color colors[DEFAULT_SIZE * DEFAULT_SIZE] = {
      WHITE, EMPTY, BLACK, EMPTY, EMPTY, EMPTY, BLACK, EMPTY, WHITE,
      EMPTY, EMPTY, EMPTY, BLACK, EMPTY, EMPTY, EMPTY, WHITE, EMPTY,
      BLACK, EMPTY, EMPTY, EMPTY, BLACK, EMPTY, WHITE};
  game g =
      game_new_ext(DEFAULT_SIZE, DEFAULT_SIZE, constraints, colors, true, FULL);
  bool test1 = game_is_wrapping(g) == true;
  bool test2 = game_get_neighbourhood(g) == FULL;
  bool test3 = game_nb_rows(g) == DEFAULT_SIZE;
  bool test4 = game_nb_cols(g) == DEFAULT_SIZE;
  game_delete(g);
  return test1 && test2 && test3 && test4;
}

/* ********** USAGE ********** */

void usage(int argc, char *argv[]) {
  fprintf(stderr, "Usage: %s <testname> [<...>]\n", argv[0]);
  exit(EXIT_FAILURE);
}

/* ********** MAIN ROUTE ********** */

int main(int argc, char *argv[]) {
  if (argc == 1) usage(argc, argv);

  fprintf(stderr, "=> Start test \"%s\"\n", argv[1]);
  bool ok = false;
  if (strcmp("dummy", argv[1]) == 0) {
    ok = test_dummy();
  } else if (strcmp("game_set_color", argv[1]) == 0) {
    ok = test_game_set_color();
  } else if (strcmp("game_delete", argv[1]) == 0) {
    ok = test_game_delete();
  } else if (strcmp("game_get_color", argv[1]) == 0) {
    ok = test_game_get_color();
  } else if (strcmp("game_default", argv[1]) == 0) {
    ok = test_game_default();
  } else if (strcmp("game_get_next_square", argv[1]) == 0) {
    ok = test_game_get_next_square();
  } else if (strcmp("game_play_move", argv[1]) == 0) {
    ok = test_game_play_move();
  } else if (strcmp("game_redo", argv[1]) == 0) {
    ok = test_game_redo();
  } else if (strcmp("game_is_wrapping", argv[1]) == 0) {
    ok = test_game_is_wrapping();
  } else if (strcmp("game_new_ext", argv[1]) == 0) {
    ok = test_game_new_ext();
  } else {
    fprintf(stderr, "error argument !\n");
    return EXIT_FAILURE;
  }

  if (ok) {
    fprintf(stderr, "Test \"%s\" finished: SUCCESS\n", argv[1]);
    return EXIT_SUCCESS;
  } else {
    fprintf(stderr, "Test \"%s\" finished: FAILURE\n", argv[1]);
    return EXIT_FAILURE;
  }
}