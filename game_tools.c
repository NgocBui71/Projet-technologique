#include "game_tools.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "game.h"
#include "game_aux.h"
#include "game_ext.h"

/* ************************************************************************** */
/* ********** CONVERTERS ********** */
// Convert constraint value to character
char constraint_to_char(int constraint) {
  if (constraint == UNCONSTRAINED) {
    return '-';
  } else {
    return '0' + constraint;
  }
}
// Convert character to constraint value
int char_to_constraint(char c) {
  if (c == '-') {
    return UNCONSTRAINED;
  } else if (c >= '0' && c <= '9') {
    return (int)c - 48;
  } else {
    fprintf(stderr, "Invalid constraint character: %c\n", c);
    exit(EXIT_FAILURE);
  }
}
// Convert color enum to character
char color_to_char(color c) {
  switch (c) {
    case EMPTY:
      return 'e';
    case WHITE:
      return 'w';
    case BLACK:
      return 'b';
    default:
      fprintf(stderr, "Invalid color character: %c\n", c);
      exit(EXIT_FAILURE);
  }
}
// Convert character to color enum
color char_to_color(char c) {
  switch (c) {
    case 'e':
      return EMPTY;
    case 'w':
      return WHITE;
    case 'b':
      return BLACK;
    default:
      fprintf(stderr, "Invalid color character: %c\n", c);
      exit(EXIT_FAILURE);
  }
}
/* ************************************************************************** */
// Verify file pointer
void verify_pointer(FILE *f, const char *filename) {
  if (f == NULL) {
    fprintf(stderr, "Error opening file for reading: %s\n", filename);
    exit(EXIT_FAILURE);
  }
}
// Verify return value
void verify_ret(int ret, int target, char *filename) {
  if (ret != target) {
    fprintf(stderr, "Error reading file header: %s\n", filename);
    exit(EXIT_FAILURE);
  }
}
// Verify malloc result
void verify_malloc(game *tab) {
  if (tab == NULL) {
    fprintf(stderr, "Not enough memory\n");
    exit(EXIT_FAILURE);
  }
}
// Free game table
void freeGameTab(game *tab, uint cmpt) {
  for (uint i = 0; i < cmpt; i++) {
    game_delete(tab[i]);
  }
  free(tab);
}
/* ************************************************************************** */

// Load game from file
game game_load(char *filename) {
  FILE *file = fopen(filename, "r");
  verify_pointer(file, filename);

  // Read game information from the file
  int nb_rows, nb_cols, x, y;
  int ret = fscanf(file, "%d %d %d %d", &nb_rows, &nb_cols, &x, &y);
  verify_ret(ret, 4, filename);

  // Determine neighborhood
  bool wrapping = (x != 0);
  neighbourhood neigh;
  switch (y) {
    case 0:
      neigh = FULL;
      break;
    case 1:
      neigh = ORTHO;
      break;
    case 2:
      neigh = FULL_EXCLUDE;
      break;
    case 3:
      neigh = ORTHO_EXCLUDE;
      break;
    default:
      fprintf(stderr, "Invalid neighborhood value in file: %s\n", filename);
      exit(EXIT_FAILURE);
  }

  // Create a new game with the specified parameters
  game g = game_new_empty_ext(nb_rows, nb_cols, wrapping, neigh);

  // Read constraints and colors from the file and set them in the game
  for (int i = 0; i < nb_rows; i++) {
    for (int j = 0; j < nb_cols; j++) {
      char constraint;
      char color;
      do {
        int ret = fscanf(file, "%c", &constraint);
        verify_ret(ret, 1, filename);
      } while (constraint == '\n');

      int ret = fscanf(file, "%c", &color);
      verify_ret(ret, 1, filename);
      game_set_constraint(g, i, j, char_to_constraint(constraint));
      game_set_color(g, i, j, char_to_color(color));
    }
  }

  fclose(file);
  return g;
}

/* ************************************************************************** */

// Save current game to a file
void game_save(cgame g, char *filename) {
  FILE *f = fopen(filename, "w");
  verify_pointer(f, filename);

  // Write game information to the file
  fprintf(f, "%d %d %d %d\n", game_nb_rows(g), game_nb_cols(g),
          game_is_wrapping(g), game_get_neighbourhood(g));

  // Write constraints and colors to the file
  for (int i = 0; i < game_nb_rows(g); i++) {
    for (int j = 0; j < game_nb_cols(g); j++) {
      int constraint = constraint_to_char(game_get_constraint(g, i, j));
      char color = color_to_char(game_get_color(g, i, j));
      fprintf(f, "%c%c", constraint, color);
    }
    fprintf(f, "\n");
  }

  fclose(f);
}

/* ************************************************************************** */

uint genMoves(int pos, game g, uint cmpt, game **tab) {
  if (pos == game_nb_cols(g) * game_nb_rows(g)) {
    if (game_won(g)) {
      game_print(g);
      *tab = realloc(*tab, (cmpt + 1) * sizeof(game));
      (*tab)[cmpt] = game_copy(g);

      return cmpt + 1;
    }
    return cmpt;
  }

  // Calculate the position
  uint y = pos % game_nb_cols(g);
  uint x = pos / game_nb_cols(g);

  // Play the moves WHITE + BLACK
  for (color c = WHITE; c <= BLACK; c++) {
    game_set_color(g, x, y, c);
    if (game_get_status(g, x, y) != ERROR) {
      cmpt = genMoves(pos + 1, g, cmpt, tab);
    }
    game_set_color(g, x, y, EMPTY);
  }

  return cmpt;
}

// Solve the game
bool game_solve(game g) {
  game *tab = NULL;
  game gg = game_copy(g);
  game_restart(gg);

  int nb_sol = genMoves(0, gg, 0, &tab);
  if (nb_sol > 0) {
    for (uint i = 0; i < game_nb_rows(g); i++) {
      for (uint j = 0; j < game_nb_cols(g); j++) {
        game_set_constraint(g, i, j, game_get_constraint(tab[0], i, j));
        game_set_color(g, i, j, game_get_color(tab[0], i, j));
      }
    }
  }
  game_delete(gg);
  freeGameTab(tab, nb_sol);
  return nb_sol > 0;
}

// Count the number of solutions
uint game_nb_solutions(cgame g) {
  game g_copy = game_copy(g);
  game *tab = NULL;
  uint nb_solutions = genMoves(0, g_copy, 0, &tab);
  game_delete(g_copy);
  freeGameTab(tab, nb_solutions);
  return nb_solutions;
}