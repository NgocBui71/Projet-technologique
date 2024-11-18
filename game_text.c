#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "game.h"
#include "game_aux.h"
#include "game_ext.h"
#include "game_struct.h"
#include "game_tools.h"

int main(int argc, char* argv[]) {
  game g;
  if (argc > 1) {
    g = game_load(argv[1]);
  } else {
    g = game_default();
  }
  while (!game_won(g)) {
    game_print(g);
    for (uint i = 0; i < g->nb_rows; i++) {
      for (uint j = 0; j < g->nb_cols; j++) {
        if (game_get_status(g, i, j) == ERROR) {
          printf("erreur dans la case (%u %u)\n", i, j);
        }
      }
    }
    printf("Veuillez saisir un caractère : ");
    char c;
    // Check the return value of scanf
    if (scanf(" %c", &c) != 1) {
      // Handle input error
      fprintf(stderr, "Error reading character.\n");
      return EXIT_FAILURE;
    }
    if (c == 'h') {
      printf(
          "Pour commencer le jeu veuillez écrire 'b' pour jouer la couleur "
          "black dans une case, 'w' pour jouer la couleur white dans une "
          "case, "
          "'e' pour (empty) vider la case de sa couleur, 'r' pour (restart) "
          "réinitialiser la grille, 'z' pour (undo) annuler le dernier "
          "mouvement, 'y' pour (redo) "
          "rejouer le dernier mouvement, 's' pour (save) sauvegarder l'état "
          "courant du jeu dans un fichier, "
          "'h' pour (help) demander de l'aide, 'q' pour quitter le jeu. "
          "Amusez-vous bien !!!\n");
    } else if (c == 'r') {
      game_restart(g);
    } else if (c == 'z') {
      game_undo(g);
    } else if (c == 'y') {
      game_redo(g);
    } else if (c == 'q') {
      game_delete(g);
      printf("Shame\n");
      return EXIT_SUCCESS;
    } else if (c == 's') {
      char filename[100];
      printf("Veuillez entrer le nom du fichier pour sauvegarder :");
      if (scanf("%s", filename) != 1) {
        fprintf(stderr, "Error reading filename.\n");
        return EXIT_FAILURE;
      }
      game_save(g, filename);
      printf("Jeu sauvegardé dans le fichier %s\n", filename);
      return EXIT_SUCCESS;
    } else if (c == 'w' || c == 'b' || c == 'e') {
      printf("Veuillez entrer les coordonnées : ");
      uint i, j;
      if (scanf("%u %u", &i, &j) != 2) {
        fprintf(stderr, "Error reading coordinates.\n");
        return EXIT_FAILURE;
      }
      if (i < g->nb_rows && j < g->nb_cols) {
        if (c == 'w') {
          game_play_move(g, i, j, WHITE);
        } else if (c == 'b') {
          game_play_move(g, i, j, BLACK);
        } else if (c == 'e') {
          game_play_move(g, i, j, EMPTY);
        }
      }
    }
  }
  game_print(g);
  printf("Congratulations, you win ! :D\n");
  return EXIT_SUCCESS;
}