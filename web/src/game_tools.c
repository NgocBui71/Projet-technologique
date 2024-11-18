/**
 * @file game_tools.c
 * @copyright University of Bordeaux. All rights reserved, 2023.
 **/

#include "game_tools.h"

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "game.h"
#include "game_aux.h"
#include "game_ext.h"
#include "game_private.h"
#include "game_struct.h"
#include "queue.h"

/* ************************************************************************** */
/*                               LOAD & SAVE                                  */
/* ************************************************************************** */

game game_load(char *filename)
{
  return NULL; // use your own implementation
}

/* ************************************************************************** */

void game_save(cgame g, char *filename)
{
  // use your own implementation
}

/* ************************************************************************** */
/*                                  SOLVER                                    */
/* ************************************************************************** */

bool game_solve(game g)
{
  // use your own implementation
  return false;
}

/* ************************************************************************** */

uint game_nb_solutions(cgame g)
{
  // use your own implementation
  return 0;
}

/* ************************************************************************** */
/*                             Random Game Generator                          */
/* ************************************************************************** */

game game_random(uint nb_rows, uint nb_cols, bool wrapping, neighbourhood neigh,
                 bool with_solution, float black_rate, float constraint_rate)
{
  assert(black_rate >= 0.0f && black_rate <= 1.0f);
  assert(constraint_rate >= 0.0f && constraint_rate <= 1.0f);
  game g = game_new_empty_ext(nb_rows, nb_cols, wrapping, neigh);
  assert(g);

  // fill the grid with random colors
  for (uint i = 0; i < nb_rows; i++)
  {
    for (uint j = 0; j < nb_cols; j++)
    {
      color c = (rand() < black_rate * (float)RAND_MAX) ? BLACK : WHITE;
      game_set_color(g, i, j, c);
    }
  }

  // fill the grid with actual constraint at random positions
  uint nb_squares = nb_rows * nb_cols;
  uint nb_constraints = constraint_rate * nb_squares;
  for (uint i = 0; i < nb_constraints; i++)
  {
    uint row = rand() % nb_rows;
    uint col = rand() % nb_cols;
    int nb_blacks = game_nb_neighbors(g, row, col, BLACK);
    game_set_constraint(g, row, col, nb_blacks);
  }

  // check solution
  if (!game_won(g))
  {
    game_delete(g);
    return NULL;
  }

  if (!with_solution)
    game_restart(g);
  return g;
}

/* ************************************************************************** */
