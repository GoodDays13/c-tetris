#include "renderer.h"
#include "game.h"
#include <SDL2/SDL_render.h>

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

const int numbers[10][5][3] = {
	{{1,1,1}, // 0
	 {1,0,1},
	 {1,0,1},
	 {1,0,1},
	 {1,1,1}},
	{{0,1,0}, // 1
	 {1,1,0},
	 {0,1,0},
	 {0,1,0},
	 {1,1,1}},
	{{1,1,1}, // 2
	 {0,0,1},
	 {1,1,1},
	 {1,0,0},
	 {1,1,1}},
	{{1,1,1}, // 3
	 {0,0,1},
	 {1,1,1},
	 {0,0,1},
	 {1,1,1}},
	{{1,0,1}, // 4
	 {1,0,1},
	 {1,1,1},
	 {0,0,1},
	 {0,0,1}},
	{{1,1,1}, // 5
	 {1,0,0},
	 {1,1,1},
	 {0,0,1},
	 {1,1,1}},
	{{1,1,1}, // 6
	 {1,0,0},
	 {1,1,1},
	 {1,0,1},
	 {1,1,1}},
	{{1,1,1}, // 7
	 {0,0,1},
	 {0,0,1},
	 {0,0,1},
	 {0,0,1}},
	{{1,1,1}, // 8
	 {1,0,1},
	 {1,1,1},
	 {1,0,1},
	 {1,1,1}},
	{{1,1,1}, // 9
	 {1,0,1},
	 {1,1,1},
	 {0,0,1},
	 {1,1,1}}
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
render_piece(struct Renderer *renderer, const struct Piece *piece, int x, int y, int block_size) {
	struct Color color = piece_colors[piece->type];
	SDL_Rect rect;
	rect.w = block_size;
	rect.h = block_size;
	for (int i = 0; i < 4; i++) {
		rect.x = x + piece->locations[i].x * block_size;
		rect.y = y + piece->locations[i].y * block_size;
		SDL_SetRenderDrawColor(renderer->renderer, color.r, color.g, color.b, color.a);
		SDL_RenderFillRect(renderer->renderer, &rect);
		if (block_size > 3) {
			SDL_SetRenderDrawColor(renderer->renderer, 0, 0, 0, 255);
			SDL_RenderDrawRect(renderer->renderer, &rect);
		}
	}
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
	int grid_right = grid_left + grid_size * GRID_WIDTH;
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

	// Draw current piece
	if (!gamestate->game_over)
		render_piece(renderer, &gamestate->current_piece, grid_left, grid_top, grid_size);

	SDL_Rect rect = {
		grid_left - 1,
		grid_top - 1,
		grid_size * GRID_WIDTH + 2,
		grid_size * GRID_HEIGHT + 2
	};
	SDL_SetRenderDrawColor(renderer->renderer, 255, 255, 255, 255);
	SDL_RenderDrawRect(renderer->renderer, &rect);

	// Render level
	render_digit(renderer, gamestate->level, grid_right + grid_size / 2, 2 * grid_size + 1, grid_size / 2);
	// Render score
	render_number(renderer, gamestate->score, grid_right + grid_size / 2, 5 * grid_size + 1, grid_size / 5);
	// Render next piece
	render_piece(renderer, &pieces[gamestate->bucket[gamestate->bucket_index]],
			  grid_right - grid_size * 2,
			  grid_bottom - grid_size * 5,
			  grid_size
			  );
	SDL_SetRenderDrawColor(renderer->renderer, 255, 255, 255, 255);
	rect.x = grid_right + grid_size - 1;
	rect.y = grid_bottom - grid_size * 5 - 1;
	rect.w = grid_size * 4 + 2;
	rect.h = grid_size * 4 + 2;
	SDL_RenderDrawRect(renderer->renderer, &rect);

	SDL_RenderPresent(renderer->renderer);
}

void
render_digit(struct Renderer *renderer, int number, int x, int y, int block_size) {
	SDL_SetRenderDrawColor(renderer->renderer, 255, 255, 255, 255);
	SDL_Rect rect;
	rect.w = block_size;
	rect.h = block_size;
	for (int y_block = 0; y_block < 5; y_block++) {
		for (int x_block = 0; x_block < 3; x_block++) {
			if (numbers[number][y_block][x_block]) {
				rect.x = x + x_block * block_size;
				rect.y = y + y_block * block_size;
				SDL_RenderFillRect(renderer->renderer, &rect);
			}
		}
	}
}

void render_number(struct Renderer *renderer, int number, int x, int y, int block_size) {
	if (number == 0) {
		render_digit(renderer, 0, x, y, block_size);
		return;
	}
	int digits[10];
	int digit_count = 0;
	while (number > 0) {
		digits[digit_count++] = number % 10;
		number /= 10;
	}
	for (int i = digit_count - 1; i >= 0; i--) {
		render_digit(renderer, digits[i], x + (digit_count - 1 - i) * block_size * 4, y, block_size);
	}
}
