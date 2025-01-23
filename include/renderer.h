#ifndef RENDERER_H
#define RENDERER_H

#include <SDL2/SDL.h>
#include "game.h"

struct Renderer {
	SDL_Window* window;
	SDL_Renderer* renderer;
	int width;
	int height;
};

extern const int numbers[10][5][3];

int create_window(struct Renderer* renderer, const char* title, int width, int height);

void destroy_window(struct Renderer* renderer);

void render_game(struct Renderer* renderer, struct GameState* game);

void render_digit(struct Renderer* renderer, int number, int x, int y, int block_size);
void render_number(struct Renderer* renderer, int number, int x, int y, int block_size);

#endif
