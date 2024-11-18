#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "game.h"
#include "game_aux.h"
#include "game_ext.h"
#include "game_struct.h"
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

/* ********** TEST GAME RESTART ********** */

bool test_game_restart() {
  game g = game_default();
  game_set_color(g, 2, 3, WHITE);
  game_set_color(g, 3, 3, BLACK);
  game_restart(g);
  for (uint i = 0; i < DEFAULT_SIZE; i++) {
    for (uint j = 0; j < DEFAULT_SIZE; j++) {
      if (game_get_color(g, i, j) != EMPTY) {
        game_delete(g);
        return false;
      }
    }
  }
  game_delete(g);
  return true;
}

/* ********** TEST GAME GET CONSTRAINT ********** */

bool test_game_get_constraint() {
  game g = game_default();
  game_set_constraint(g, 1, 1, 1);
  ASSERT(game_get_constraint(g, 1, 1) == 1);
  game_set_constraint(g, 1, 1, UNCONSTRAINED);
  ASSERT(game_get_constraint(g, 1, 1) == -1);
  game_delete(g);
  return true;
}

/* ********** TEST GAME DEFAULT SOLUTION ********** */

bool test_game_default_solution() {
  game g = game_default_solution();
  game_print(g);
  ASSERT(game_won(g));
  game_delete(g);
  return true;
}

/* ********** TEST GAME PRINT ********** */

bool test_game_print() {
  game g = game_default();
  game_print(g);
  game_delete(g);
  return true;
}

/* ********** TEST GAME EQUAL ********** */

bool test_game_equal() {
  game g1 = game_default_solution();
  game g2 = game_default_solution();
  ASSERT(game_equal(g1, g2));
  game empty = game_new_empty();
  ASSERT(!game_equal(g2, empty));
  game_delete(empty);
  game_delete(g1);
  game_delete(g2);
  return true;
}

/* ********** TEST GAME NEW EMPTY ********** */

bool test_game_new_empty() {
  game g = game_new_empty();
  for (uint i = 0; i < DEFAULT_SIZE; i++) {
    for (uint j = 0; j < DEFAULT_SIZE; j++) {
      if (game_get_constraint(g, i, j) != UNCONSTRAINED ||
          game_get_color(g, i, j) != EMPTY) {
        game_delete(g);
        return false;
      }
    }
  }
  game_delete(g);
  return true;
}

/* ********** TEST GAME NEW EMPTY EXT ********** */

bool test_game_new_empty_ext() {
  game g = game_new_empty_ext(3, 3, true, FULL);
  ASSERT(game_is_wrapping(g) == true);
  ASSERT(game_get_neighbourhood(g) == FULL);
  ASSERT(game_nb_rows(g) == 3);
  ASSERT(game_nb_cols(g) == 3);
  bool test = true;
  for (uint i = 0; i < 3; i++) {
    for (uint j = 0; j < 3; j++) {
      if (game_get_constraint(g, i, j) != UNCONSTRAINED ||
          game_get_color(g, i, j) != EMPTY) {
        test = false;
        break;
      }
    }
    if (!test) {
      break;
    }
  }
  game_delete(g);
  return test;
}

/* ********** TEST GAME GET NEIGHBOURHOOD ********** */

bool test_game_get_neighbourhood() {
  game g1 = game_new_empty_ext(3, 3, true, FULL);
  ASSERT(game_get_neighbourhood(g1) == FULL);
  game g2 = game_new_empty_ext(3, 3, true, ORTHO);
  ASSERT(game_get_neighbourhood(g2) == ORTHO);
  game_delete(g1);
  game_delete(g2);
  return true;
}

/* ********** TEST GAME UNDO ********** */

bool test_game_undo() {
  game g = game_new_empty_ext(DEFAULT_SIZE, DEFAULT_SIZE, true, FULL);
  game_play_move(g, 1, 1, BLACK);
  game_play_move(g, 2, 2, WHITE);
  game_play_move(g, 3, 3, BLACK);
  game_play_move(g, 3, 3, WHITE);
  game_undo(g);
  ASSERT(game_get_color(g, 1, 1) == BLACK);
  ASSERT(game_get_color(g, 2, 2) == WHITE);
  ASSERT(game_get_color(g, 3, 3) == BLACK);
  game_undo(g);
  ASSERT(game_get_color(g, 3, 3) == EMPTY);
  game_delete(g);
  return true;
}

/* ********** TEST GAME LOAD ********** */

bool test_game_load() {
  FILE *fptr;
  fptr = fopen("test_file.txt", "w");
  if (fptr == NULL) {
    fprintf(stderr, "test_game_load: Failed to open the file test_file.txt\n");
    exit(EXIT_FAILURE);
  }
  fclose(fptr);

  game g1 = game_default_solution();
  game_save(g1, "test_file.txt");
  game_delete(g1);

  game loaded_game = game_load("test_file.txt");
  bool test = game_won(loaded_game);
  game_delete(loaded_game);
  remove("test_file.txt");  // delete test file

  return test;
}

/* ********** TEST GAME SAVE ********** */

bool test_game_save() {
  FILE *fptr;
  fptr = fopen("test_file_save.txt", "w");
  if (fptr == NULL) {
    fprintf(stderr,
            "test_game_load: Failed to open the file test_game_load.txt");
    exit(EXIT_FAILURE);
  }
  fclose(fptr);

  game g1 = game_default_solution();
  game_save(g1, "test_file_save.txt");

  game g2 = game_default_solution();
  bool test = game_won(g2);
  game_delete(g1);
  game_delete(g2);
  remove("test_file_save.txt");  // delete test file

  return test;
}

/* ********** TEST GAME SOLVE ********** */

bool test_game_solve() {
  game g = game_new_empty_ext(2, 2, false, FULL);
  ASSERT(g);
  game_set_constraint(g, 0, 0, 1);
  ASSERT(game_solve(g) == true);
  game_print(g);
  ASSERT(game_won(g));
  game_delete(g);

  game g2 = game_default();
  ASSERT(g2);
  ASSERT(game_solve(g2) == true);
  ASSERT(game_won(g2));
  game_delete(g2);

  game g3 = game_new_empty_ext(3, 3, false, ORTHO);
  ASSERT(g3);
  game_set_constraint(g3, 0, 0, 1);
  ASSERT(game_solve(g3) == true);
  ASSERT(game_won(g3));
  game_delete(g3);

  game g4 = game_new_empty_ext(3, 3, false, ORTHO_EXCLUDE);
  ASSERT(g4);
  game_set_constraint(g4, 0, 0, 1);
  ASSERT(game_solve(g4) == true);
  ASSERT(game_won(g4));
  game_delete(g4);

  game g5 = game_new_empty_ext(2, 4, false, FULL_EXCLUDE);
  ASSERT(g5);
  game_set_constraint(g5, 0, 0, 1);
  ASSERT(game_solve(g5) == true);
  ASSERT(game_won(g5));
  game_delete(g5);

  return true;
}

/* ********** USAGE ********** */

void usage(int argc, char *argv[]) {
  fprintf(stderr, "Usage: %s <testname> [<...>]\n", argv[0]);
  exit(EXIT_FAILURE);
}

/* ********** MAIN ROUTINE ********** */

int main(int argc, char *argv[]) {
  if (argc == 1) {
    usage(argc, argv);
  }
  // start test
  fprintf(stderr, "=> Start test \"%s\"\n", argv[1]);
  bool ok = false;
  if (strcmp("dummy", argv[1]) == 0) {
    ok = test_dummy();
  } else if (strcmp("game_restart", argv[1]) == 0) {
    ok = test_game_restart();
  } else if (strcmp("game_get_constraint", argv[1]) == 0) {
    ok = test_game_get_constraint();
  } else if (strcmp("game_default_solution", argv[1]) == 0) {
    ok = test_game_default_solution();
  } else if (strcmp("game_print", argv[1]) == 0) {
    ok = test_game_print();
  } else if (strcmp("game_equal", argv[1]) == 0) {
    ok = test_game_equal();
  } else if (strcmp("game_new_empty", argv[1]) == 0) {
    ok = test_game_new_empty();
  } else if (strcmp("game_new_empty_ext", argv[1]) == 0) {
    ok = test_game_new_empty_ext();
  } else if (strcmp("game_get_neighbourhood", argv[1]) == 0) {
    ok = test_game_get_neighbourhood();
  } else if (strcmp("game_undo", argv[1]) == 0) {
    ok = test_game_undo();
  } else if (strcmp("game_load", argv[1]) == 0) {
    ok = test_game_load();
  } else if (strcmp("game_save", argv[1]) == 0) {
    ok = test_game_save();
  } else if (strcmp("game_solve", argv[1]) == 0) {
    ok = test_game_solve();
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