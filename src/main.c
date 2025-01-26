#include "renderer.h"
#include "game.h"
#include "audio.h"

int main(int argc, char *argv[]) {
	(void)argc;
	(void)argv;
	struct Renderer renderer;

	int block_size = 24;
	if (create_window(&renderer, "New Window", (GRID_WIDTH + 6 * 2) * block_size, GRID_HEIGHT * block_size + 2))
		return 1;

	struct GameState gamestate = {0};
	struct Inputs inputs = {0};
	init_audio();
	init_game(&gamestate);

	static unsigned int last_time = 0;
	int running = 1;
	while (running) {
		// Calculate delta time
		unsigned int current_time = SDL_GetTicks();
		float delta_time = (current_time - last_time) / 1000.0;
		last_time = current_time;

		// Handle events
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
				case SDL_QUIT:
					running = 0;
					break;
				case SDL_KEYDOWN:
					switch (event.key.keysym.sym) {
						case SDLK_ESCAPE:
							running = 0;
							break;
						case SDLK_LEFT:
							inputs.left = 1;
							break;
						case SDLK_RIGHT:
							inputs.right = 1;
							break;
						case SDLK_DOWN:
							inputs.down = 1;
							break;
						case SDLK_UP:
							inputs.rotate = 1;
							break;
						case SDLK_r:
							if (gamestate.game_over) {
								init_game(&gamestate);
							}
							break;
					}
					break;
				case SDL_KEYUP:
					switch (event.key.keysym.sym) {
						case SDLK_DOWN:
							inputs.down = 0;
							break;
					}
					break;
			}
		}

		// Update
		update_game(&gamestate, &inputs, delta_time);

		// Render
		render_game(&renderer, &gamestate);
	}
	cleanup_audio();
	destroy_window(&renderer);

	return 0;
}
