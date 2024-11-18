// SDL2 Model by aurelien.esnard@u-bordeaux.fr

#ifndef MODEL_H
#define MODEL_H

#include <SDL.h>
#include <stdbool.h>
#include <stdio.h>

#include "game.h"
#include "game_aux.h"
#include "game_ext.h"
#include "game_tools.h"

struct Button_t {
  uint startX, startY;
  uint width, height;
  char *name;
};

struct Env_t {
  SDL_Texture *background;
  game g;
  uint square_size;
  uint startX, startY;
  struct Button_t nb_solutions, solve, undo, redo, restart;
};

typedef struct Env_t Env;

/* **************************************************************** */

#ifdef __ANDROID__
#define PRINT(STR, ...)          \
  do {                           \
    SDL_Log(STR, ##__VA_ARGS__); \
  } while (0)
#define ERROR(STR, ...)          \
  do {                           \
    SDL_Log(STR, ##__VA_ARGS__); \
    exit(EXIT_FAILURE);          \
  } while (0)
#else
#define PRINT(STR, ...)         \
  do {                          \
    printf(STR, ##__VA_ARGS__); \
  } while (0)
#define ERROR(STR, ...)                  \
  do {                                   \
    fprintf(stderr, STR, ##__VA_ARGS__); \
    exit(EXIT_FAILURE);                  \
  } while (0)
#endif

/* **************************************************************** */

#define APP_NAME "mosaic_A09 Demo"
#define SCREEN_WIDTH 600
#define SCREEN_HEIGHT 600
#define DELAY 100

/* **************************************************************** */

Env* init(SDL_Window* win, SDL_Renderer* ren, int argc, char* argv[]);
void render(SDL_Window* win, SDL_Renderer* ren, Env* env);
void clean(SDL_Window* win, SDL_Renderer* ren, Env* env);
bool process(SDL_Window* win, SDL_Renderer* ren, Env* env, SDL_Event* e);

/* **************************************************************** */

#endif
