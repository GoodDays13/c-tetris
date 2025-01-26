#include "input.h"

void handle_events(struct Inputs *inputs) {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
			case SDL_QUIT:
				exit(0);
				break;
			case SDL_KEYDOWN:
				switch (event.key.keysym.sym) {
					case SDLK_ESCAPE:
						exit(0);
						break;
					case SDLK_LEFT:
						inputs->left = 1;
						break;
					case SDLK_RIGHT:
						inputs->right = 1;
						break;
					case SDLK_DOWN:
						inputs->down = 1;
						break;
					case SDLK_UP:
						inputs->rotate = 1;
						break;
					case SDLK_r:
						inputs->restart = 1;
						break;
				}
				break;
			case SDL_KEYUP:
				switch (event.key.keysym.sym) {
					case SDLK_DOWN:
						inputs->down = 0;
						break;
				}
				break;
		}
	}
}
