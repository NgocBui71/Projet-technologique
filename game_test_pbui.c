#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "game.h"
#include "game_aux.h"
#include "game_ext.h"
#include "game_tools.h"

/* ********** ASSERT ********** */

#define ASSERT(expr)                                                          \
  do {                                                                        \
    if ((expr) == 0) {                                                        \
      fprintf(stderr, "[%s:%d] Assertion '%s' failed!\n", __FILE__, __LINE__, \
              #expr);                                                         \
      abort();                                                                \
    }                                                                         \
  } while (0)

/* ********** TEST DUMMY ********** */

int test_dummy() { return true; }

/* ********** USAGE ********** */

void usage(int argc, char *argv[]) {
  fprintf(stderr, "Usage: %s <testname> [<...>]\n", argv[0]);
  exit(EXIT_FAILURE);
}

/* ********** TEST GAME WON ********** */

bool test_game_won() {
  game g1 = game_default_solution();
  bool test1 = game_won(g1);
  ASSERT(test1);

  game g2 = game_default();
  bool test2 = !game_won(g2);
  ASSERT(test2);

  game_delete(g1);
  game_delete(g2);
  return test1 && test2;
}

/* ********** TEST GAME NB NEIGHBORS ********** */

bool test_game_nb_neighbors() {
  game g = game_new_empty();
  ASSERT(g != NULL);
  game_set_color(g, 0, 0, WHITE);
  game_set_color(g, 1, 1, WHITE);
  game_set_color(g, 1, 0, WHITE);
  int result = game_nb_neighbors(g, 0, 0, WHITE);
  ASSERT(result == 3);
  game g1 = game_new_empty_ext(DEFAULT_SIZE, DEFAULT_SIZE, false, ORTHO);
  game_set_color(g1, 0, 0, WHITE);
  game_set_color(g1, 1, 1, WHITE);
  game_set_color(g1, 1, 0, WHITE);
  game_set_color(g1, 0, 1, WHITE);
  int result1 = game_nb_neighbors(g1, 0, 0, WHITE);
  ASSERT(result1 == 3);
  game g2 = game_new_empty_ext(DEFAULT_SIZE, DEFAULT_SIZE, false, FULL_EXCLUDE);
  game_set_color(g2, 0, 0, WHITE);
  game_set_color(g2, 1, 0, WHITE);
  game_set_color(g2, 0, 1, WHITE);
  int result2 = game_nb_neighbors(g2, 0, 0, WHITE);
  ASSERT(result2 == 2);
  game_delete(g);
  game_delete(g1);
  game_delete(g2);
  return true;
}

/* ********** TEST GAME GET STATUS ********** */

// Version de prof
int test_game_get_status(void) {
  game g = game_new_empty();

  // test UNCONSTRAINED
  game_set_constraint(g, 2, 2, UNCONSTRAINED);
  game_set_color(g, 2, 2, WHITE);
  bool test1a = (game_get_status(g, 2, 2) == UNSATISFIED);
  game_set_color(g, 1, 1, BLACK);
  game_set_color(g, 1, 2, BLACK);
  game_set_color(g, 1, 3, BLACK);
  game_set_color(g, 2, 1, BLACK);
  game_set_color(g, 2, 3, BLACK);
  game_set_color(g, 3, 1, BLACK);
  game_set_color(g, 3, 2, BLACK);
  bool test1b = (game_get_status(g, 2, 2) == UNSATISFIED);
  game_set_color(g, 3, 3, BLACK);
  bool test1c = (game_get_status(g, 2, 2) == SATISFIED);

  // test constraint MAX
  game_set_constraint(g, 2, 2, 9);
  game_set_color(g, 2, 2, EMPTY);
  bool test2a = (game_get_status(g, 2, 2) == UNSATISFIED);
  game_set_color(g, 2, 2, BLACK);
  bool test2b = (game_get_status(g, 2, 2) == SATISFIED);
  game_set_color(g, 2, 2, WHITE);
  bool test2c = (game_get_status(g, 2, 2) == ERROR);

  game_delete(g);

  if (test1a && test1b && test1c && test2a && test2b && test2c) return true;

  return false;
}

/* ********** TEST GAME SET CONSTRAINT ********** */

bool test_game_set_constraint() {
  game g = game_default();
  game_set_constraint(g, 2, 2, 2);
  if (game_get_constraint(g, 2, 2) == 2) {
    game_delete(g);
    return true;
  }
  game_delete(g);
  return false;
}

/* ********** TEST GAME COPY ********** */

bool test_game_copy() {
  game g1 = game_new_empty_ext(5, 5, true, ORTHO);
  game g2 = game_copy(g1);
  bool result1 = game_is_wrapping(g2) == true;
  bool result2 = game_get_neighbourhood(g2) == ORTHO;
  game_delete(g1);
  game_delete(g2);
  return result1 && result2;
}

/* ********** TEST GAME NEW ********** */

bool test_game_new() {
  game g = game_default();
  if (g != NULL) {
    game_delete(g);
    return true;
  }
  game_delete(g);
  return false;
}

/* ********** TEST GAME NB ROWS ********** */

bool test_game_nb_rows() {
  game g = game_default();
  bool test = game_nb_rows(g);
  if (test) {
    game_delete(g);
    return true;
  }
  game_delete(g);
  return false;
}

/* ********** TEST GAME NB COLS ********** */

bool test_game_nb_cols() {
  game g = game_default();
  bool test = game_nb_cols(g);
  if (test) {
    game_delete(g);
    return true;
  }
  game_delete(g);
  return false;
}
/* ********** TEST GAME NB SOLUTIONS ********** */
bool test_game_nb_solutions() {
  game g = game_new_empty_ext(2, 2, false, FULL);
  game_set_constraint(g, 0, 0, 1);
  ASSERT(g);
  uint solutions = game_nb_solutions(g);
  ASSERT(solutions == 4);
  game_delete(g);

  game g2 = game_default();
  ASSERT(g2);
  uint solutions2 = game_nb_solutions(g2);
  ASSERT(solutions2 == 1);
  game_delete(g2);
  return true;
}
/* ********** MAIN ROUTE ********** */

int main(int argc, char *argv[]) {
  if (argc == 1) {
    usage(argc, argv);
  }
  // start test
  fprintf(stderr, "=> Start test \"%s\"\n", argv[1]);
  bool ok = false;
  if (strcmp("dummy", argv[1]) == 0) {
    ok = test_dummy();
  } else if (strcmp("game_new", argv[1]) == 0) {
    ok = test_game_new();
  } else if (strcmp("game_won", argv[1]) == 0) {
    ok = test_game_won();
  } else if (strcmp("game_get_status", argv[1]) == 0) {
    ok = test_game_get_status();
  } else if (strcmp("game_set_constraint", argv[1]) == 0) {
    ok = test_game_set_constraint();
  } else if (strcmp("game_copy", argv[1]) == 0) {
    ok = test_game_copy();
  } else if (strcmp("game_nb_neighbors", argv[1]) == 0) {
    ok = test_game_nb_neighbors();
  } else if (strcmp("game_nb_rows", argv[1]) == 0) {
    ok = test_game_nb_rows();
  } else if (strcmp("game_nb_cols", argv[1]) == 0) {
    ok = test_game_nb_cols();
  } else if (strcmp("game_nb_solutions", argv[1]) == 0) {
    ok = test_game_nb_solutions();
  } else {
    fprintf(stderr, "Error: test \"%s\" not found!\n", argv[1]);
    exit(EXIT_FAILURE);
  }

  // print test result
  if (ok) {
    fprintf(stderr, "Test \"%s\" finished: SUCCESS\n", argv[1]);
    return EXIT_SUCCESS;
  } else {
    fprintf(stderr, "Test \"%s\" finished: FAILURE\n", argv[1]);
    return EXIT_FAILURE;
  }
}