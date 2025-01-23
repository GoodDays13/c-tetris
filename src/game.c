#include <stdlib.h>
#include <time.h>
#include "game.h"

struct Piece pieces[] = {
	{{{4, 0}, {4, 1}, {5, 1}, {5, 2}}}, // Z
	{{{4, 0}, {4, 1}, {4, 2}, {4, 3}}}, // I
	{{{4, 0}, {4, 1}, {4, 2}, {5, 2}}}, // L
	{{{4, 0}, {4, 1}, {4, 2}, {3, 2}}}, // J
	{{{4, 0}, {4, 1}, {5, 1}, {3, 1}}}, // T
	{{{5, 0}, {5, 1}, {4, 1}, {4, 2}}}, // S
	{{{4, 0}, {4, 1}, {5, 0}, {5, 1}}}  // O
};

enum PieceType {
	PIECE_Z,
	PIECE_I,
	PIECE_L,
	PIECE_J,
	PIECE_T,
	PIECE_S,
	PIECE_O
};

void
init_game(struct GameState *game) {
	for (int y = 0; y < GRID_HEIGHT; y++) {
		for (int x = 0; x < GRID_WIDTH; x++) {
			game->grid[y][x] = 0;
		}
	}
	srand(time(NULL));
	create_piece(game);
}

void
update_game(struct GameState *game, struct Inputs *inputs, float delta_time) {
	handle_moves(game, inputs);

	game->time_till_drop -= delta_time;
	if (inputs->down && game->time_till_drop > 0.1) {
		game->time_till_drop = 0.1;
	}
	if (game->time_till_drop <= 0) {
		if (inputs->down) {
			game->time_till_drop = 0.1;
		} else {
			game->time_till_drop += 1;
		}
		// Check if piece can move down
		for (int i = 0; i < 4; i++) {
			if (game->current_piece.locations[i].y + 1 >= GRID_HEIGHT ||
				game->grid[game->current_piece.locations[i].y + 1][game->current_piece.locations[i].x]) {
				// Piece can't move down
				// Place piece
				for (int i = 0; i < 4; i++) {
					game->grid[game->current_piece.locations[i].y][game->current_piece.locations[i].x] = 1;
				}
				// Check for full rows
				check_rows(game);
				// Spawn new piece
				create_piece(game);
				return;
			}
		}
		// Drop piece
		for (int i = 0; i < 4; i++) {
			game->current_piece.locations[i].y++;
		}
	}
}

void
handle_moves(struct GameState *game, struct Inputs *inputs) {
	if (inputs->rotate) {
		inputs->rotate = 0;
		rotate_piece(game);
	}

	if (inputs->left && inputs->right) {
		inputs->left = 0;
		inputs->right = 0;
		return;
	}

	if (inputs->left) {
		inputs->left = 0;
		// Check if piece can move left
		for (int i = 0; i < 4; i++) {
			if (game->current_piece.locations[i].x - 1 < 0 ||
				game->grid[game->current_piece.locations[i].y][game->current_piece.locations[i].x - 1]) {
				return;
			}
		}
		// Move piece left
		for (int i = 0; i < 4; i++) {
			game->current_piece.locations[i].x--;
		}
	}
	if (inputs->right) {
		inputs->right = 0;
		// Check if piece can move right
		for (int i = 0; i < 4; i++) {
			if (game->current_piece.locations[i].x + 1 >= GRID_WIDTH ||
				game->grid[game->current_piece.locations[i].y][game->current_piece.locations[i].x + 1]) {
				return;
			}
		}
		// Move piece right
		for (int i = 0; i < 4; i++) {
			game->current_piece.locations[i].x++;
		}
	}
}

void
create_piece(struct GameState *game) {
	int num = rand() % 7;
	game->current_piece = pieces[num];
	game->piece_index = num;
}

void
rotate_piece(struct GameState *game) {
	if (game->piece_index == PIECE_O)
		return;

	struct Location center = game->current_piece.locations[1];
	for (int i = 0; i < 4; i++) {
		int x = game->current_piece.locations[i].x;
		int y = game->current_piece.locations[i].y;
		int new_x = center.x + center.y - y;
		int new_y = center.y - center.x + x;
		if (new_x < 0 || new_x >= GRID_WIDTH || new_y < 0 || new_y >= GRID_HEIGHT ||
			game->grid[new_y][new_x]) {
			return;
		}
	}
	for (int i = 0; i < 4; i++) {
		int x = game->current_piece.locations[i].x;
		int y = game->current_piece.locations[i].y;
		int new_x = center.x + center.y - y;
		int new_y = center.y - center.x + x;
		game->current_piece.locations[i].x = new_x;
		game->current_piece.locations[i].y = new_y;
	}
}

int
check_rows(struct GameState *game) {
	int full_rows = 0;
	for (int y = 0; y < GRID_HEIGHT; y++) {
		int full = 1;
		for (int x = 0; x < GRID_WIDTH; x++) {
			if (!game->grid[y][x]) {
				full = 0;
				break;
			}
		}
		if (full) {
			full_rows++;
			for (int i = y; i > 0; i--) {
				for (int x = 0; x < GRID_WIDTH; x++) {
					game->grid[i][x] = game->grid[i - 1][x];
				}
			}
			for (int x = 0; x < GRID_WIDTH; x++) {
				game->grid[0][x] = 0;
			}
		}
	}
	return full_rows;
}
