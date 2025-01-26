#include <stdlib.h>
#include <time.h>
#include "game.h"
#include "audio.h"

const struct Piece pieces[] = {
	{{{5, 0}, {5, 1}, {4, 1}, {4, 2}}, PIECE_Z}, // Z
	{{{3, 0}, {4, 0}, {5, 0}, {6, 0}}, PIECE_I}, // I
	{{{4, 0}, {4, 1}, {4, 2}, {5, 2}}, PIECE_L}, // L
	{{{5, 0}, {5, 1}, {5, 2}, {4, 2}}, PIECE_J}, // J
	{{{4, 0}, {4, 1}, {5, 1}, {3, 1}}, PIECE_T}, // T
	{{{4, 0}, {4, 1}, {5, 1}, {5, 2}}, PIECE_S}, // S
	{{{4, 0}, {4, 1}, {5, 0}, {5, 1}}, PIECE_O}  // O
};

void
init_game(struct GameState *game) {
	game->game_over = 0;
	for (int y = 0; y < GRID_HEIGHT; y++) {
		for (int x = 0; x < GRID_WIDTH; x++) {
			game->grid[y][x] = 0;
		}
	}
	srand(time(NULL));
	game->bucket[0] = PIECE_Z;
	game->bucket[1] = PIECE_I;
	game->bucket[2] = PIECE_L;
	game->bucket[3] = PIECE_J;
	game->bucket[4] = PIECE_T;
	game->bucket[5] = PIECE_S;
	game->bucket[6] = PIECE_O;

	shuffle_bucket(game);
	game->bucket_index = 0;
	create_piece(game);
	game->level = 1;
	game->score = 0;
	game->speed = 1.0;
	game->time_till_drop = 1.0;

	play_music();
}

void
update_game(struct GameState *game, struct Inputs *inputs, float delta_time) {
	if (inputs->restart) {
		inputs->restart = 0;
		if (game->game_over) {
			init_game(game);
		}
	}

	if (game->game_over)
		return;

	handle_moves(game, inputs);

	game->time_till_drop -= delta_time;
	if (inputs->down && game->time_till_drop > 0.1) {
		game->time_till_drop = 0.1;
	}
	if (game->time_till_drop <= 0) {
		game->time_till_drop += 1.0 / game->level;
		// Check if piece can move down
		for (int i = 0; i < 4; i++) {
			if (game->current_piece.locations[i].y + 1 >= GRID_HEIGHT ||
				game->grid[game->current_piece.locations[i].y + 1][game->current_piece.locations[i].x]) {
				// Piece can't move down
				// Place piece
				for (int i = 0; i < 4; i++) {
					game->grid[game->current_piece.locations[i].y][game->current_piece.locations[i].x] = game->current_piece.type + 1;
				}
				// Check for full rows
				int rows = check_rows(game);
				calculate_score(game, rows);
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
	int num = game->bucket[game->bucket_index++];
	if (game->bucket_index == 7) {
		shuffle_bucket(game);
		game->bucket_index = 0;
	}

	game->current_piece = pieces[num];
	for (int i = 0; i < 4; i++) {
		if (game->grid[game->current_piece.locations[i].y][game->current_piece.locations[i].x]) {
			game->bucket_index--;
			game->game_over = 1;
			stop_music();
		}
	}
}

void
shuffle_bucket(struct GameState *game) {
	for (int i = 6; i > 0; i--) {
		int j = rand() % (i + 1);
		enum PieceType temp = game->bucket[i];
		game->bucket[i] = game->bucket[j];
		game->bucket[j] = temp;
	}
}

void
rotate_piece(struct GameState *game) {
	if (game->current_piece.type == PIECE_O)
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

void
calculate_score(struct GameState *game, int rows_cleared) {
	int gained_score = 0;
	switch (rows_cleared) {
		case 0:
			gained_score = 0;
			break;
		case 1:
			gained_score = 100 * game->level;
			break;
		case 2:
			gained_score = 300 * game->level;
			break;
		case 3:
			gained_score = 500 * game->level;
			break;
		case 4:
			gained_score = 800 * game->level;
			break;
	}
	game->score += gained_score;
	while (game->score >= 250 * game->level * (game->level + 1)) {
		game->level++;
		game->speed /= 1.1;
	}
}
