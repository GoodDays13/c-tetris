#ifndef GAME_H
#define GAME_H

#define GRID_WIDTH 10
#define GRID_HEIGHT 20

struct Location {
	int x;
	int y;
};

struct Piece {
	struct Location locations[4];
};

struct Inputs {
	int left;
	int right;
	int down;
	int rotate;
};

struct GameState {
	int grid[GRID_HEIGHT][GRID_WIDTH];
	float time_till_drop;
	struct Piece current_piece;
	int piece_index;
};

void init_game(struct GameState *game);

void update_game(struct GameState *game, struct Inputs *inputs, float delta_time);

void create_piece(struct GameState *game);

void handle_moves(struct GameState *game, struct Inputs *inputs);

void rotate_piece(struct GameState *game);

int check_rows(struct GameState *game);

extern struct Piece pieces[];

#endif
