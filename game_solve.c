#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "game.h"
#include "game_aux.h"
#include "game_ext.h"
#include "game_struct.h"
#include "game_tools.h"

int main(int argc, char *argv[]) {
  if (argc < 3) {
    fprintf(stderr, "Number of arguments error\n");
    return EXIT_FAILURE;
  } else {
    char option = *argv[1];
    char *input_file = argv[2];
    char *output_file;
    if (argc == 4) {
      output_file = argv[3];
    } else {
      output_file = NULL;
    }

    game g = game_load(input_file);

    if (strcmp("-s", argv[1]) == 0) {
      game_solve(g);              // Appel à la fonction de résolution
      game_save(g, output_file);  // Sauvegarde de la solution
    } else if (strcmp("-c", argv[1]) == 0) {
      int nb = game_nb_solutions(g);  // Appel à la fonction de comptage
      FILE *f = fopen(output_file, "w");
      if (f == NULL) {
        fprintf(stderr, "Error opening file for writting: %s\n", output_file);
        exit(EXIT_FAILURE);
      }
      fprintf(f, "%d\n", nb);  // Enregistrement du résultat
      fclose(f);
    } else {
      fprintf(stderr, "Unrecognized option: %d\n", option);
      game_delete(g);
      return EXIT_FAILURE;
    }
    game_delete(g);
  }
  return EXIT_SUCCESS;
}