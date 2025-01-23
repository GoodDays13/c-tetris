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

int create_window(struct Renderer* renderer, const char* title, int width, int height);

void destroy_window(struct Renderer* renderer);

void render_game(struct Renderer* renderer, struct GameState* game);

#endif
