#ifndef GAME_H
#define GAME_H

#define GRID_WIDTH 10
#define GRID_HEIGHT 20

struct Location {
	int x;
	int y;
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

struct Piece {
	struct Location locations[4];
	enum PieceType type;
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
	float speed;
	int level;
	int score;
	struct Piece current_piece;
	int game_over;
};

void init_game(struct GameState *game);

void update_game(struct GameState *game, struct Inputs *inputs, float delta_time);

void create_piece(struct GameState *game);

void handle_moves(struct GameState *game, struct Inputs *inputs);

void rotate_piece(struct GameState *game);

int check_rows(struct GameState *game);

void calculate_score(struct GameState *game, int rows_cleared);

extern const struct Piece pieces[];

#endif
