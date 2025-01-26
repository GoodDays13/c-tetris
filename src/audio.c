#include "audio.h"
#include <SDL2/SDL.h>
#include <stdlib.h>

#define ADSR_ATTACK 0
#define ADSR_DECAY 0.5
#define ADSR_SUSTAIN 0
#define ADSR_RELEASE 0

struct AudioState audio_state = {0};

const float octave4[] = {
	261.6, // C4
	293.7, // D4
	329.6, // E4
	349.2, // F4
	392.0, // G4
	440.0, // A4
	493.9, // B4
};

const float octave5[] = {
	523.3, // C5
	587.3, // D5
	659.3, // E5
	698.5, // F5
	783.9, // G5
	880.0, // A5
	987.8, // B5
};

const struct Note melody[] = {
	{octave5[2], 1},
	{octave4[6], 0.5},
	{octave5[0], 0.5},
	{octave5[1], 1},
	{octave5[0], 0.5},
	{octave4[6], 0.5},
	{octave4[5], 1},
	{octave4[5], 0.5},
	{octave5[0], 0.5},
	{octave5[2], 1},
	{octave5[1], 0.5},
	{octave5[0], 0.5},
	{octave4[6], 1.5},
	{octave5[0], 0.5},
	{octave5[1], 1},
	{octave5[2], 1},
	{octave5[0], 1},
	{octave4[5], 1},
	{octave4[5], 1},
	{0, 1.5},
	{octave5[1], 1},
	{octave5[3], 0.5},
	{octave5[5], 1},
	{octave5[4], 0.5},
	{octave5[3], 0.5},
	{octave5[2], 1.5},
	{octave5[0], 0.5},
	{octave5[2], 1},
	{octave5[1], 0.5},
	{octave5[0], 0.5},
	{octave4[6], 1},
	{octave4[6], 0.5},
	{octave5[0], 0.5},
	{octave5[1], 1},
	{octave5[2], 1},
	{octave5[0], 1},
	{octave4[5], 1},
	{octave4[5], 1},
	{0, 1},
};

const struct Note snare_timings[] = {
	{0, 1},
	{0, 1},
	{0, 0.25},
	{0, 0.75},
	{0, 1},
	{0, 1},
	{0, 1},
	{0, 0.5},
	{0, 0.5},
	{0, 1},
};

void update_song(struct Song* song, float dt) {
	const struct Note current_note = song->notes[song->current_note];
	// Update current note
	song->time_in_note += dt;
	if (song->time_in_note >= current_note.duration * 60 / audio_state.bpm) {
		song->current_note++;
		song->time_in_note = 0;
		if (song->current_note >= song->length) {
			song->current_note = 0;
		}
	}
}

void audio_callback(void* userdata, Uint8* stream, int len) {
	struct AudioState* state = (struct AudioState*)userdata;
	float volume = 0.1;

	for (int i = 0; i < len; i++) {
		struct Song* melody = &state->melody;
		const struct Note current_note = melody->notes[melody->current_note];
		// Update current note
		update_song(melody, 1.0 / state->sample_rate);
		update_song(&state->snare, 1.0 / state->sample_rate);
		float duration_of_note = current_note.duration * 60 / state->bpm;

		// Generate envelope
		float envelope = 1;
		if (melody->time_in_note < ADSR_ATTACK) {
			/*envelope = song->time_in_note / ADSR_ATTACK;*/
			envelope = 0;
		} else if (melody->time_in_note < ADSR_ATTACK + ADSR_DECAY) {
			float decay = (melody->time_in_note - ADSR_ATTACK) / ADSR_DECAY;
			envelope = 1 - decay * (1 - ADSR_SUSTAIN);
		} else if (melody->time_in_note > duration_of_note - ADSR_RELEASE) {
			/*float release = (song->time_in_note - (duration_of_note - ADSR_RELEASE)) / ADSR_RELEASE;*/
			float release = 0;
			envelope = (1 - release) * ADSR_SUSTAIN;
		} else {
			envelope = ADSR_SUSTAIN;
		}
		if (current_note.frequency == 0) {
			envelope = 0;
		}


		// Generate sawtooth wave
		melody->phase += current_note.frequency / state->sample_rate;
		if (melody->phase >= 1) {
			melody->phase -= 1;
		}

		// Turn sawtooth wave into square wave
		float wave = (melody->phase < 0.5) ? 1 : -1;
		wave *= envelope;
		if (state->snare.time_in_note >= 0 && state->snare.time_in_note < 0.02) {
			wave += rand() / (float)RAND_MAX * 1 - 0.5;
		}
		wave *= volume;
		stream[i] = (wave + 1) * 127;
	}
};

void init_audio() {
	SDL_AudioSpec want, have;
	want.freq = 44100;
	want.format = AUDIO_U8;
	want.channels = 1;
	want.samples = 4096;
	want.callback = audio_callback;
	want.userdata = &audio_state;

	if (SDL_OpenAudio(&want, &have) < 0) {
		SDL_Log("Failed to open audio: %s\n", SDL_GetError());
		exit(1);
	}

	audio_state.bpm = 150;
	audio_state.sample_rate = have.freq;

	audio_state.melody.notes = melody;
	audio_state.melody.length = sizeof(melody) / sizeof(melody[0]);
	audio_state.melody.time_in_note = 0;
	audio_state.melody.current_note = 0;
	audio_state.melody.phase = 0;

	audio_state.snare.notes = snare_timings;
	audio_state.snare.length = sizeof(snare_timings) / sizeof(snare_timings[0]);
	audio_state.snare.time_in_note = -0.5 * 60 / audio_state.bpm;
	audio_state.snare.current_note = 0;

	SDL_PauseAudio(0);
}

void cleanup_audio() {
	SDL_CloseAudio();
}
