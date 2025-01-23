#include "renderer.h"
#include "game.h"

struct Color {
		int r;
		int g;
		int b;
		int a;
};

struct Color piece_colors[] = {
	{0, 255, 0, 255}, // PIECE_Z
	{0, 255, 255, 255}, // PIECE_I
	{255, 170, 0, 255}, // PIECE_L
	{0, 0, 255, 255}, // PIECE_J
	{255, 0, 255, 255}, // PIECE_T
	{255, 0, 0, 255}, // PIECE_S
	{255, 255, 0, 255} // PIECE_O
};

int
create_window(struct Renderer *renderer, const char *title, int width, int height) {
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		fprintf(stderr, "SDL_Init failed: %s\n", SDL_GetError());
		return 1;
	}

	renderer->window = SDL_CreateWindow(
		title,
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		width,
		height,
		SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE
	);
	if (renderer->window == NULL) {
		fprintf(stderr, "SDL_CreateWindow failed: %s\n", SDL_GetError());
		return 1;
	}

	renderer->renderer = SDL_CreateRenderer(renderer->window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (renderer->renderer == NULL) {
		fprintf(stderr, "SDL_CreateRenderer failed: %s\n", SDL_GetError());
		return 1;
	}

	renderer->width = width;
	renderer->height = height;

	return 0;
}

void
destroy_window(struct Renderer *renderer) {
	if (renderer->renderer)
		SDL_DestroyRenderer(renderer->renderer);
	if (renderer->window)
		SDL_DestroyWindow(renderer->window);
	SDL_Quit();
}

void
render_game(struct Renderer *renderer, struct GameState *gamestate) {
	SDL_GetWindowSize(renderer->window, &renderer->width, &renderer->height);
	SDL_SetRenderDrawColor(renderer->renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer->renderer);

	// Render game
	// Draw grid
	int grid_top = 1;
	int grid_bottom = renderer->height - 1;
	int grid_size = (grid_bottom - grid_top) / GRID_HEIGHT;
	int grid_left = (renderer->width - grid_size * GRID_WIDTH) / 2;
	//int grid_right = grid_left + grid_size * GRID_WIDTH;
	for (int y = 0; y < GRID_HEIGHT; y++) {
		for (int x = 0; x < GRID_WIDTH; x++) {
			if (gamestate->grid[y][x]) {
				SDL_Rect rect = {
					grid_left + x * grid_size,
					grid_top + y * grid_size,
					grid_size,
					grid_size
				};
				struct Color color = piece_colors[gamestate->grid[y][x] - 1];
				SDL_SetRenderDrawColor(renderer->renderer, color.r, color.g, color.b, color.a);
				SDL_RenderFillRect(renderer->renderer, &rect);
				if (grid_size > 3) {
					SDL_SetRenderDrawColor(renderer->renderer, 0, 0, 0, 255);
					SDL_RenderDrawRect(renderer->renderer, &rect);
				}
			}
		}
	}
	for (int i = 0; i < 4; i++) {
			SDL_Rect rect = {
				grid_left + gamestate->current_piece.locations[i].x * grid_size,
				grid_top + gamestate->current_piece.locations[i].y * grid_size,
				grid_size,
				grid_size
			};
			struct Color color = piece_colors[gamestate->current_piece.type];
			SDL_SetRenderDrawColor(renderer->renderer, color.r, color.g, color.b, color.a);
			SDL_RenderFillRect(renderer->renderer, &rect);
			if (grid_size > 3) {
				SDL_SetRenderDrawColor(renderer->renderer, 0, 0, 0, 255);
				SDL_RenderDrawRect(renderer->renderer, &rect);
			}
	}

	SDL_Rect rect = {
		grid_left - 1,
		grid_top - 1,
		grid_size * GRID_WIDTH + 2,
		grid_size * GRID_HEIGHT + 2
	};
	SDL_SetRenderDrawColor(renderer->renderer, 255, 255, 255, 255);
	SDL_RenderDrawRect(renderer->renderer, &rect);

	SDL_RenderPresent(renderer->renderer);
}
