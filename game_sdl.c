#include "game_sdl.h"

#include <SDL.h>
#include <SDL_image.h>  // required to load transparent texture from PNG
#include <SDL_ttf.h>    // required to use TTF fonts
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "game.h"
#include "game_aux.h"
#include "game_ext.h"
#include "game_tools.h"
#include "game_random.h"

#define max(a, b) ((a) > (b) ? (a) : (b))
#define FONT "res/Arial.ttf"

/* **************************************************************** */

void checkNullPointer(void *ptr, const char *message) {
  if (ptr == NULL) {
    fprintf(stderr, "Error: %s\n", message);
    exit(EXIT_FAILURE);
  }
}

void SetButtonName(struct Button_t *button, const char *name) {
  button->name = malloc(strlen(name) + 1);
  checkNullPointer(button->name, "Memory allocation failed for button name");
  strcpy(button->name, name);
}

void SetButton(struct Button_t *button, const char *name, uint startX,
               uint startY, uint width, uint height) {
  SetButtonName(button, name);
  button->startX = startX;
  button->startY = startY;
  button->width = width;
  button->height = height;
}

void setCords(Env *env, int winW, int winH) {
  // Calculate square size and template size
  int maxRowsCols = max(game_nb_cols(env->g), game_nb_rows(env->g));
  int square_size = max(-1 * winW, -1 * winH) / (-2 * maxRowsCols);
  env->square_size = square_size;
  int template_size = square_size * maxRowsCols;

  // Calculate position to center the templatemessage
  int templateX = (winW - template_size) / 2;
  int templateY = ((winH - template_size) / 2) - square_size / 2;
  env->startX = templateX;
  env->startY = templateY;
}

// Initiates buttons
void initiateButtons(Env *env) {
  uint cols = game_nb_cols(env->g);
  uint rows = game_nb_rows(env->g);
  uint square_size = env->square_size;

  // Set buttpons
  SetButton(&env->nb_solutions, env->nb_solutions.name,
            env->startX + ((cols * square_size) / 2) - square_size / 2,
            env->startY - square_size / 2, square_size * 2, square_size / 2.5);

  SetButton(&env->solve, env->solve.name,
            env->startX + ((cols * square_size) / 2) + square_size * 1.5,
            env->startY - square_size / 2, square_size, square_size / 2.5);

  SetButton(
      &env->undo, env->undo.name,
      env->startX + ((cols * square_size) / 2) + square_size / 2,
      env->startY + rows * square_size + square_size / 3 + square_size / 1.2,
      square_size * 2, square_size / 1.2);

  SetButton(&env->redo, env->redo.name,
            env->startX + ((cols * square_size) / 2) + square_size / 2,
            env->startY + rows * square_size + square_size / 3, square_size * 2,
            square_size / 1.2);

  SetButton(&env->restart, env->restart.name,
            env->startX + ((cols * square_size) / 2) - square_size * 2.5,
            env->startY + (rows * square_size) + square_size / 1.55,
            square_size * 2, square_size);
}

Env *init(SDL_Window *win, SDL_Renderer *ren, int argc, char *argv[]) {
  Env *env = malloc(sizeof(struct Env_t));

  int winW, winH;
  SDL_GetWindowSize(win, &winW, &winH);

  /* Get the game to structure */
  if (argc > 1) {
    env->g = game_load(argv[1]);
  } else {
    // Seed the random number generator
    srand(time(NULL));
    // Generate a random game
    env->g = game_random(5, 5, false, FULL, false, 0.7, 0.7);
  }

  setCords(env, winW, winH);
  SetButtonName(&env->nb_solutions, "Nb Solutions");
  SetButtonName(&env->solve, "Solve");
  SetButtonName(&env->redo, "Redo");
  SetButtonName(&env->undo, "Undo");
  SetButtonName(&env->restart, "Restart");
  initiateButtons(env);

  // Load the background image
  SDL_Surface *surface = IMG_Load("res/background_clouds.jpeg");
  if (!surface) {
    fprintf(stderr, "Error loading background image: %s\n", IMG_GetError());
    exit(1);
  }
  env->background = SDL_CreateTextureFromSurface(ren, surface);
  SDL_FreeSurface(surface);

  return env;
}

/* **************************************************************** */

// Renders the text given
void render_text(SDL_Renderer *renderer, TTF_Font *font, int x, int y,
                 const char *text, SDL_Color color) {
  SDL_Surface *surface;
  SDL_Texture *texture;
  SDL_Rect rect;

  // Render text onto surface
  surface = TTF_RenderText_Solid(font, text, color);
  if (!surface) {
    // Handle error
    printf("Error rendering text: %s\n", TTF_GetError());
    return;
  }

  // Create texture from surface
  texture = SDL_CreateTextureFromSurface(renderer, surface);
  if (!texture) {
    printf("Error creating texture: %s\n", SDL_GetError());
    SDL_FreeSurface(surface);
    return;
  }

  // Set rendering rectangle
  rect.x = x;
  rect.y = y;
  rect.w = surface->w;
  rect.h = surface->h;

  SDL_RenderCopy(renderer, texture, NULL, &rect);
  SDL_DestroyTexture(texture);
  SDL_FreeSurface(surface);
}

// Renders the constraints to the grid
void draw_constraint(SDL_Renderer *ren, int i, int j, Env *env) {
  int constraint = game_get_constraint(env->g, i, j);
  if (constraint != -1) {
    SDL_Color textColor;
    if (game_get_status(env->g, i, j) == ERROR) {
      textColor = (SDL_Color){250, 16, 16, 255};
    } else if (game_get_color(env->g, i, j) == BLACK) {
      textColor = (SDL_Color){247, 247, 247, 255};
    } else {
      textColor = (SDL_Color){36, 36, 36, 255};
    }
    char constraintStr[2];
    snprintf(constraintStr, sizeof(constraintStr), "%d", constraint);
    TTF_Font *font = TTF_OpenFont(FONT, env->square_size / 1.5);
    if (!font) ERROR("TTF_OpenFont: %s\n", FONT);

    // Calculate the position
    int textWidth, textHeight;
    TTF_SizeText(font, constraintStr, &textWidth, &textHeight);
    int x =
        env->startX + j * env->square_size + (env->square_size - textWidth) / 2;
    int y = env->startY + i * env->square_size +
            (env->square_size - textHeight) / 2;

    render_text(ren, font, x, y, constraintStr, textColor);
    TTF_CloseFont(font);
  }
}

// Renders the color and the constraints of each square of the game
void draw_col_con(SDL_Renderer *ren, Env *env) {
  for (int i = 0; i < game_nb_rows(env->g); ++i) {
    for (int j = 0; j < game_nb_cols(env->g); ++j) {
      color c = game_get_color(env->g, i, j);
      SDL_Rect rect = {env->startX + j * env->square_size,
                       env->startY + i * env->square_size, env->square_size,
                       env->square_size};
      switch (c) {
        case WHITE:
          SDL_SetRenderDrawColor(ren, 240, 240, 240, SDL_ALPHA_OPAQUE);
          break;
        case BLACK:
          SDL_SetRenderDrawColor(ren, 10, 10, 10, SDL_ALPHA_OPAQUE);
          break;
        case EMPTY:
          SDL_SetRenderDrawColor(ren, 165, 177, 199, SDL_ALPHA_OPAQUE);
          break;
      }
      SDL_RenderFillRect(ren, &rect);
      draw_constraint(ren, i, j, env);
    }
  }
}

// Renders the grid to the game
void draw_grid(SDL_Renderer *ren, Env *env) {
  SDL_SetRenderDrawColor(ren, 90, 90, 90, SDL_ALPHA_OPAQUE);
  for (int i = 0; i <= game_nb_rows(env->g); ++i) {
    int y = env->startY + i * env->square_size;
    SDL_RenderDrawLine(ren, env->startX, y,
                       env->startX + game_nb_cols(env->g) * env->square_size,
                       y);
  }
  for (int j = 0; j <= game_nb_cols(env->g); ++j) {
    int x = env->startX + j * env->square_size;
    SDL_RenderDrawLine(ren, x, env->startY, x,
                       env->startY + game_nb_rows(env->g) * env->square_size);
  }
}

// Draws the given button
void draw_button(SDL_Renderer *ren, int startX, int startY, int width,
                 int height, char *text) {
  SDL_Rect buttonRect = {startX, startY, width, height};
  // Colors
  SDL_SetRenderDrawColor(ren, 230, 230, 230, SDL_ALPHA_OPAQUE);
  SDL_RenderFillRect(ren, &buttonRect);
  SDL_SetRenderDrawColor(ren, 100, 100, 100, SDL_ALPHA_OPAQUE);
  SDL_RenderDrawRect(ren, &buttonRect);

  // Button
  int textWidth, textHeight;
  TTF_Font *font = TTF_OpenFont(FONT, (max(-1 * height, -1 * width) * -1) / 2);
  if (!font) ERROR("TTF_OpenFont: %s\n", FONT);
  TTF_SizeText(font, text, &textWidth, &textHeight);
  int textX = startX + (width - textWidth) / 2;
  int textY = startY + (height - textHeight) / 2;
  SDL_Color textColor = {30, 30, 30, SDL_ALPHA_OPAQUE};
  render_text(ren, font, textX, textY, text, textColor);
  TTF_CloseFont(font);
}

void render(SDL_Window *win, SDL_Renderer *ren, Env *env) {
  // Render the background
  SDL_RenderCopy(ren, env->background, NULL, NULL);

  // Center the game
  int winW, winH;
  SDL_GetWindowSize(win, &winW, &winH);
  setCords(env, winW, winH);
  initiateButtons(env);

  // Draw the game + buttons
  draw_col_con(ren, env);
  draw_grid(ren, env);
  draw_button(ren, env->nb_solutions.startX, env->nb_solutions.startY,
              env->nb_solutions.width, env->nb_solutions.height,
              env->nb_solutions.name);
  draw_button(ren, env->solve.startX, env->solve.startY, env->solve.width,
              env->solve.height, env->solve.name);
  draw_button(ren, env->undo.startX, env->undo.startY, env->undo.width,
              env->undo.height, env->undo.name);
  draw_button(ren, env->redo.startX, env->redo.startY, env->redo.width,
              env->redo.height, env->redo.name);
  draw_button(ren, env->restart.startX, env->restart.startY, env->restart.width,
              env->restart.height, env->restart.name);
}

/* **************************************************************** */

bool isInsideButton(SDL_Point mouse, struct Button_t button) {
  return (mouse.x >= button.startX && mouse.x <= button.startX + button.width &&
          mouse.y >= button.startY && mouse.y <= button.startY + button.height);
}

void updateButtonText(Env *env, struct Button_t *button, const char *newText,
                      SDL_Window *win, SDL_Renderer *ren) {
  free(button->name);
  SetButtonName(button, newText);
  render(win, ren, env);
}

bool process(SDL_Window *win, SDL_Renderer *ren, Env *env, SDL_Event *e) {
  if (e->type == SDL_QUIT) {
    return true;
  } else if (e->type == SDL_MOUSEBUTTONDOWN) {
    SDL_Point mouse;
    SDL_GetMouseState(&mouse.x, &mouse.y);

    if (env->square_size != 0) {
      uint gridY = (mouse.x - env->startX) / env->square_size;
      uint gridX = (mouse.y - env->startY) / env->square_size;

      if (gridX >= 0 && gridX < game_nb_rows(env->g) && gridY >= 0 &&
          gridY < game_nb_cols(env->g)) {
        switch (game_get_color(env->g, gridX, gridY)) {
          case EMPTY:
            game_play_move(env->g, gridX, gridY, WHITE);
            break;
          case WHITE:
            game_play_move(env->g, gridX, gridY, BLACK);
            break;
          case BLACK:
            game_play_move(env->g, gridX, gridY, EMPTY);
            break;
        }
      } else if (isInsideButton(mouse, env->nb_solutions)) {
        updateButtonText(env, &env->nb_solutions, "Calculating...", win, ren);
        SDL_RenderPresent(ren);
        int solutions = game_nb_solutions(env->g);
        char solutionText[4];
        snprintf(solutionText, sizeof(solutionText), "%d", solutions);
        updateButtonText(env, &env->nb_solutions, solutionText, win, ren);
        SDL_RenderPresent(ren);
      } else if (isInsideButton(mouse, env->solve)) {
        updateButtonText(env, &env->solve, "Solving...", win, ren);
        SDL_RenderPresent(ren);
        if (!game_solve(env->g)) {
          updateButtonText(env, &env->solve, "NotFound", win, ren);
        } else {
          updateButtonText(env, &env->solve, "Solve", win, ren);
        }
        SDL_RenderPresent(ren);
      } else if (isInsideButton(mouse, env->redo)) {
        game_redo(env->g);
      } else if (isInsideButton(mouse, env->restart)) {
        game_restart(env->g);
      } else if (isInsideButton(mouse, env->undo)) {
        game_undo(env->g);
      }
    }
  }
  return false;
}

/* **************************************************************** */

void clean(SDL_Window *win, SDL_Renderer *ren, Env *env) {
  free(env->nb_solutions.name);
  free(env->solve.name);
  free(env->undo.name);
  free(env->redo.name);
  free(env->restart.name);
  SDL_DestroyTexture(env->background);
  free(env);
}

/* **************************************************************** */
