#include "audio.h"
#include <SDL2/SDL.h>
#include <stdlib.h>

#define ADSR_ATTACK 0
#define ADSR_DECAY 0.5
#define ADSR_SUSTAIN 0
#define ADSR_RELEASE 0

struct AudioState audio_state = {0};

const float octave2[] = {
	65.4, // C2
	73.4, // D2
	82.4, // E2
	87.3, // F2
	98.0, // G2
	110.0, // A2
	123.5, // B2
};

const float octave3[] = {
	130.8, // C3
	146.8, // D3
	164.8, // E3
	174.6, // F3
	196.0, // G3
	220.0, // A3
	246.9, // B3
};

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

const struct Note melody_notes[] = {
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

const struct Note bass_notes[] = {
	{octave2[2], 0.5},
	{octave3[2], 0.5},
	{octave2[2], 0.5},
	{octave3[2], 0.5},
	{octave2[2], 0.5},
	{octave3[2], 0.5},
	{octave2[2], 0.5},
	{octave3[2], 0.5},

	{octave2[5], 0.5},
	{octave3[5], 0.5},
	{octave2[5], 0.5},
	{octave3[5], 0.5},
	{octave2[5], 0.5},
	{octave3[5], 0.5},
	{octave2[5], 0.5},
	{octave3[5], 0.5},

	{103.8, 0.5},
	{207.7, 0.5},
	{103.8, 0.5},
	{207.7, 0.5},

	{octave2[2], 0.5},
	{octave3[2], 0.5},
	{octave2[2], 0.5},
	{octave3[2], 0.5},

	{octave2[5], 0.5},
	{octave3[5], 0.5},
	{octave2[5], 0.5},
	{octave3[5], 0.5},
	{octave2[5], 0.5},
	{octave3[5], 0.5},

	{octave2[6], 0.5},
	{octave3[0], 0.5},
	{octave3[1], 0.5},

	{octave2[1], 0.5},
	{0, 0.5},
	{octave2[1], 0.5},
	{0, 0.5},
	{octave2[1], 0.5},
	{octave2[5], 0.5},
	{octave2[3], 0.5},
	{octave2[0], 0.5},
	{octave3[0], 0.5},
	{0, 0.5},
	{octave3[0], 0.5},
	{octave2[0], 0.5},
	{octave2[4], 0.4},
	{0, 0.1},
	{octave2[4], 0.5},
	{0, 0.5},
	{octave2[6], 0.5},
	{octave3[6], 0.5},
	{0, 0.5},
	{octave3[6], 0.5},
	{0, 0.5},
	{octave3[2], 0.5},
	{0, 0.5},
	{207.7, 0.5},
	{octave2[5], 0.5},
	{octave3[2], 0.5},
	{octave2[5], 0.5},
	{octave3[2], 0.5},
	{octave2[5], 1},
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
	struct Song* melody = &state->melody;
	struct Song* bass = &state->bass;
	float volume = 0.1;

	for (int i = 0; i < len; i++) {
		const struct Note current_note = melody->notes[melody->current_note];
		const struct Note current_bass_note = bass->notes[bass->current_note];

		// Update current note
		update_song(melody, 1.0 / state->sample_rate);
		update_song(bass, 1.0 / state->sample_rate);
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


		// Update phase
		melody->phase += current_note.frequency / state->sample_rate;
		if (melody->phase >= 1) {
			melody->phase -= 1;
		}

		bass->phase += current_bass_note.frequency / state->sample_rate;
		if (bass->phase >= 1) {
			bass->phase -= 1;
		}

		// Generate wave
		float wave = (melody->phase < 0.5) ? 1 : -1;
		envelope *= 0.7;
		wave *= envelope;

		float basswave = (bass->phase < 0.25) ? 1.0 : -0.33;

		// Add basic low-pass filter
		float cutoff = 0.1; // Adjust between 0-1 to change filter frequency
		basswave = audio_state.low_pass_past_sample + cutoff * (basswave - audio_state.low_pass_past_sample);
		audio_state.low_pass_past_sample = basswave;
		wave += basswave;

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
	audio_state.low_pass_past_sample = 0;

	audio_state.melody.notes = melody_notes;
	audio_state.melody.length = sizeof(melody_notes) / sizeof(melody_notes[0]);
	audio_state.melody.time_in_note = 0;
	audio_state.melody.current_note = 0;
	audio_state.melody.phase = 0;

	audio_state.bass.notes = bass_notes;
	audio_state.bass.length = sizeof(bass_notes) / sizeof(bass_notes[0]);
	audio_state.bass.time_in_note = 0;
	audio_state.bass.current_note = 0;
	audio_state.bass.phase = 0;

	audio_state.snare.notes = snare_timings;
	audio_state.snare.length = sizeof(snare_timings) / sizeof(snare_timings[0]);
	audio_state.snare.time_in_note = -0.5 * 60 / audio_state.bpm;
	audio_state.snare.current_note = 0;

	SDL_PauseAudio(0);
}

void cleanup_audio() {
	SDL_CloseAudio();
}
