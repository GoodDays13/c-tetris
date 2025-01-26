#ifndef AUDIO_H
#define AUDIO_H

#include <SDL2/SDL.h>

struct Note {
	float frequency; // in Hz
	float duration; // in beats
};

struct Song {
	const struct Note *notes;
	int length;
	float time_in_note;
	int current_note;
	float phase;
};

struct AudioState {
	struct Song melody;
	struct Song bass;
	struct Song snare;
	int sample_rate;
	float bpm;
	float low_pass_past_sample;
};

void init_audio(void);
void play_music(void);
void stop_music(void);
void cleanup_audio(void);

#endif // AUDIO_H
