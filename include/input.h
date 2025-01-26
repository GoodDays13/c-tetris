#ifndef INPUT_H
#define INPUT_H

#include <SDL2/SDL.h>

struct Inputs {
	int left;
	int right;
	int down;
	int rotate;
	int restart;
};

void handle_events(struct Inputs *inputs);

#endif
