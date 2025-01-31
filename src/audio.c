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
	{octave5[2],  0.0, 1.0, ADSR_ATTACK, ADSR_DECAY, ADSR_SUSTAIN, ADSR_RELEASE, MELODY},
	{octave4[6],  1.0, 0.5, ADSR_ATTACK, ADSR_DECAY, ADSR_SUSTAIN, ADSR_RELEASE, MELODY},
	{octave5[0],  1.5, 0.5, ADSR_ATTACK, ADSR_DECAY, ADSR_SUSTAIN, ADSR_RELEASE, MELODY},
	{octave5[1],  2.0, 1.0, ADSR_ATTACK, ADSR_DECAY, ADSR_SUSTAIN, ADSR_RELEASE, MELODY},
	{octave5[0],  3.0, 0.5, ADSR_ATTACK, ADSR_DECAY, ADSR_SUSTAIN, ADSR_RELEASE, MELODY},
	{octave4[6],  3.5, 0.5, ADSR_ATTACK, ADSR_DECAY, ADSR_SUSTAIN, ADSR_RELEASE, MELODY},

	{octave4[5],  4.0, 1.0, ADSR_ATTACK, ADSR_DECAY, ADSR_SUSTAIN, ADSR_RELEASE, MELODY},
	{octave4[5],  5.0, 1.0, ADSR_ATTACK, ADSR_DECAY, ADSR_SUSTAIN, ADSR_RELEASE, MELODY},
	{octave5[0],  5.5, 1.0, ADSR_ATTACK, ADSR_DECAY, ADSR_SUSTAIN, ADSR_RELEASE, MELODY},
	{octave5[2],  6.0, 1.0, ADSR_ATTACK, ADSR_DECAY, ADSR_SUSTAIN, ADSR_RELEASE, MELODY},
	{octave5[1],  7.0, 0.5, ADSR_ATTACK, ADSR_DECAY, ADSR_SUSTAIN, ADSR_RELEASE, MELODY},
	{octave5[0],  7.5, 0.5, ADSR_ATTACK, ADSR_DECAY, ADSR_SUSTAIN, ADSR_RELEASE, MELODY},

	{octave4[6],  8.0, 1.5, ADSR_ATTACK, ADSR_DECAY, ADSR_SUSTAIN, ADSR_RELEASE, MELODY},
	{octave5[0],  9.5, 0.5, ADSR_ATTACK, ADSR_DECAY, ADSR_SUSTAIN, ADSR_RELEASE, MELODY},
	{octave5[1], 10.0, 1.0, ADSR_ATTACK, ADSR_DECAY, ADSR_SUSTAIN, ADSR_RELEASE, MELODY},
	{octave5[2], 11.0, 1.0, ADSR_ATTACK, ADSR_DECAY, ADSR_SUSTAIN, ADSR_RELEASE, MELODY},

	{octave5[0], 12.0, 1.0, ADSR_ATTACK, ADSR_DECAY, ADSR_SUSTAIN, ADSR_RELEASE, MELODY},
	{octave4[5], 13.0, 1.0, ADSR_ATTACK, ADSR_DECAY, ADSR_SUSTAIN, ADSR_RELEASE, MELODY},
	{octave4[5], 14.0, 1.0, ADSR_ATTACK, ADSR_DECAY, ADSR_SUSTAIN, ADSR_RELEASE, MELODY},

	{octave5[1], 16.5, 1.0, ADSR_ATTACK, ADSR_DECAY, ADSR_SUSTAIN, ADSR_RELEASE, MELODY},
	{octave5[3], 17.5, 0.5, ADSR_ATTACK, ADSR_DECAY, ADSR_SUSTAIN, ADSR_RELEASE, MELODY},
	{octave5[5], 18.0, 1.0, ADSR_ATTACK, ADSR_DECAY, ADSR_SUSTAIN, ADSR_RELEASE, MELODY},
	{octave5[4], 19.0, 0.5, ADSR_ATTACK, ADSR_DECAY, ADSR_SUSTAIN, ADSR_RELEASE, MELODY},
	{octave5[3], 19.5, 0.5, ADSR_ATTACK, ADSR_DECAY, ADSR_SUSTAIN, ADSR_RELEASE, MELODY},

	{octave5[2], 20.0, 1.5, ADSR_ATTACK, ADSR_DECAY, ADSR_SUSTAIN, ADSR_RELEASE, MELODY},
	{octave5[0], 21.5, 0.5, ADSR_ATTACK, ADSR_DECAY, ADSR_SUSTAIN, ADSR_RELEASE, MELODY},
	{octave5[2], 22.0, 1.0, ADSR_ATTACK, ADSR_DECAY, ADSR_SUSTAIN, ADSR_RELEASE, MELODY},
	{octave5[1], 23.0, 0.5, ADSR_ATTACK, ADSR_DECAY, ADSR_SUSTAIN, ADSR_RELEASE, MELODY},
	{octave5[0], 23.5, 0.5, ADSR_ATTACK, ADSR_DECAY, ADSR_SUSTAIN, ADSR_RELEASE, MELODY},

	{octave4[6], 24.0, 1.0, ADSR_ATTACK, ADSR_DECAY, ADSR_SUSTAIN, ADSR_RELEASE, MELODY},
	{octave4[6], 25.0, 0.5, ADSR_ATTACK, ADSR_DECAY, ADSR_SUSTAIN, ADSR_RELEASE, MELODY},
	{octave5[0], 25.5, 0.5, ADSR_ATTACK, ADSR_DECAY, ADSR_SUSTAIN, ADSR_RELEASE, MELODY},
	{octave5[1], 26.0, 1.0, ADSR_ATTACK, ADSR_DECAY, ADSR_SUSTAIN, ADSR_RELEASE, MELODY},
	{octave5[2], 27.0, 1.0, ADSR_ATTACK, ADSR_DECAY, ADSR_SUSTAIN, ADSR_RELEASE, MELODY},

	{octave5[0], 28.0, 1.0, ADSR_ATTACK, ADSR_DECAY, ADSR_SUSTAIN, ADSR_RELEASE, MELODY},
	{octave4[5], 29.0, 1.0, ADSR_ATTACK, ADSR_DECAY, ADSR_SUSTAIN, ADSR_RELEASE, MELODY},
	{octave4[5], 30.0, 1.0, ADSR_ATTACK, ADSR_DECAY, ADSR_SUSTAIN, ADSR_RELEASE, MELODY},
};

const struct Note bass_notes[] = {
	{octave2[2], 0.0, 0.5, 0, 0, 1, 0, BASS},
	{octave3[2], 0.5, 0.5, 0, 0, 1, 0, BASS},
	{octave2[2], 1.0, 0.5, 0, 0, 1, 0, BASS},
	{octave3[2], 1.5, 0.5, 0, 0, 1, 0, BASS},
	{octave2[2], 2.0, 0.5, 0, 0, 1, 0, BASS},
	{octave3[2], 2.5, 0.5, 0, 0, 1, 0, BASS},
	{octave2[2], 3.0, 0.5, 0, 0, 1, 0, BASS},
	{octave3[2], 3.5, 0.5, 0, 0, 1, 0, BASS},

	{octave2[5], 4.0, 0.5, 0, 0, 1, 0, BASS},
	{octave3[5], 4.5, 0.5, 0, 0, 1, 0, BASS},
	{octave2[5], 5.0, 0.5, 0, 0, 1, 0, BASS},
	{octave3[5], 5.5, 0.5, 0, 0, 1, 0, BASS},
	{octave2[5], 6.0, 0.5, 0, 0, 1, 0, BASS},
	{octave3[5], 6.5, 0.5, 0, 0, 1, 0, BASS},
	{octave2[5], 7.0, 0.5, 0, 0, 1, 0, BASS},
	{octave3[5], 7.5, 0.5, 0, 0, 1, 0, BASS},

	{103.8, 8.0, 0.5, 0, 0, 1, 0, BASS},
	{207.7, 8.5, 0.5, 0, 0, 1, 0, BASS},
	{103.8, 9.0, 0.5, 0, 0, 1, 0, BASS},
	{207.7, 9.5, 0.5, 0, 0, 1, 0, BASS},

	{octave2[2], 10.0, 0.5, 0, 0, 1, 0, BASS},
	{octave3[2], 10.5, 0.5, 0, 0, 1, 0, BASS},
	{octave2[2], 11.0, 0.5, 0, 0, 1, 0, BASS},
	{octave3[2], 11.5, 0.5, 0, 0, 1, 0, BASS},

	{octave2[5], 12.0, 0.5, 0, 0, 1, 0, BASS},
	{octave3[5], 12.5, 0.5, 0, 0, 1, 0, BASS},
	{octave2[5], 13.0, 0.5, 0, 0, 1, 0, BASS},
	{octave3[5], 13.5, 0.5, 0, 0, 1, 0, BASS},
	{octave2[5], 14.0, 0.5, 0, 0, 1, 0, BASS},
	{octave3[5], 14.5, 0.5, 0, 0, 1, 0, BASS},

	{octave2[6], 15.0, 0.5, 0, 0, 1, 0, BASS},
	{octave3[0], 15.5, 0.5, 0, 0, 1, 0, BASS},
	{octave3[1], 16.0, 0.5, 0, 0, 1, 0, BASS},

	{octave2[1], 16.5, 0.5, 0, 0, 1, 0, BASS},

	{octave2[1], 17.5, 0.5, 0, 0, 1, 0, BASS},

	{octave2[1], 18.5, 0.5, 0, 0, 1, 0, BASS},
	{octave2[5], 19.0, 0.5, 0, 0, 1, 0, BASS},
	{octave2[3], 19.5, 0.5, 0, 0, 1, 0, BASS},
	{octave2[0], 20.0, 0.5, 0, 0, 1, 0, BASS},
	{octave3[0], 20.5, 0.5, 0, 0, 1, 0, BASS},

	{octave3[0], 21.5, 0.5, 0, 0, 1, 0, BASS},
	{octave2[0], 22.0, 0.5, 0, 0, 1, 0, BASS},
	{octave2[4], 22.5, 0.4, 0, 0, 1, 0, BASS},
	{octave2[4], 23.0, 0.5, 0, 0, 1, 0, BASS},

	{octave2[6], 24.0, 0.5, 0, 0, 1, 0, BASS},
	{octave3[6], 24.5, 0.5, 0, 0, 1, 0, BASS},

	{octave3[6], 25.5, 0.5, 0, 0, 1, 0, BASS},

	{octave3[2], 26.5, 0.5, 0, 0, 1, 0, BASS},

	{207.7,      27.5, 0.5, 0, 0, 1, 0, BASS},
	{octave2[5], 28.0, 0.5, 0, 0, 1, 0, BASS},
	{octave3[2], 28.5, 0.5, 0, 0, 1, 0, BASS},
	{octave2[5], 29.0, 0.5, 0, 0, 1, 0, BASS},
	{octave3[2], 29.5, 0.5, 0, 0, 1, 0, BASS},
	{octave2[5], 30.0, 1.0, 0, 0, 1, 0, BASS},
};

const struct Note snare_timings[] = {
	{0, 0.5, 0.02, 0, 0, 1, 0, SNARE},
	{0, 1.5, 0.02, 0, 0, 1, 0, SNARE},
	{0, 2.5, 0.02, 0, 0, 1, 0, SNARE},
	{0, 2.75,0.02, 0, 0, 1, 0, SNARE},
	{0, 3.5, 0.02, 0, 0, 1, 0, SNARE},
	{0, 4.5, 0.02, 0, 0, 1, 0, SNARE},
	{0, 5.5, 0.02, 0, 0, 1, 0, SNARE},
	{0, 6.5, 0.02, 0, 0, 1, 0, SNARE},
	{0, 7.0, 0.02, 0, 0, 1, 0, SNARE},
	{0, 7.5, 0.02, 0, 0, 1, 0, SNARE},
};

void play_track(struct AudioState* audio, struct Track* track) {
	struct PlayingTrack* playing_track = malloc(sizeof(struct PlayingTrack));
	playing_track->track = track;
	playing_track->next = audio->playing_tracks;
	playing_track->prev = NULL;
	audio->playing_tracks = playing_track;
	if (playing_track->next)
		playing_track->next->prev = playing_track;
}

void play_note(struct AudioState* audio, const struct Track* track, const struct Note* note) {
	struct PlayingNote* playing_note = malloc(sizeof(struct PlayingNote));
	if (playing_note == NULL) {
		// Handle memory allocation failure
		return;
	}
	*playing_note = (struct PlayingNote){
		.note = note,
		.track = track,
		.time_in_note = 0,
		.phase = 0,
		.next = audio->playing_notes,
		.prev = NULL
	};
	if (audio->playing_notes != NULL) {
		audio->playing_notes->prev = playing_note;
	}
	audio->playing_notes = playing_note;
}

void remove_track(struct AudioState* audio, struct PlayingTrack* track_node) {
	if (track_node->prev) {
		track_node->prev->next = track_node->next;
	} else {
		audio->playing_tracks = track_node->next;
	}
	if (track_node->next) {
		track_node->next->prev = track_node->prev;
	}

	free(track_node->track);
	free(track_node);
}

void remove_note(struct AudioState* audio, struct PlayingNote* note_node) {
	if (note_node->prev) {
		note_node->prev->next = note_node->next;
	} else {
		audio->playing_notes = note_node->next;
	}
	if (note_node->next) {
		note_node->next->prev = note_node->prev;
	}

	free(note_node);
}

// This should update the current time then add the current note to the playing notes
void update_track(struct PlayingTrack* track_node, double dt) {
	struct Track* track = track_node->track;

	// Update current note
	track->current_time += dt;
	if (track->current_time >= track->length * 60 / track->bpm) {
		track->current_time -= track->length * 60 / track->bpm;
		track->next_note = 0;
		if (!track->repeat) {
			remove_track(&audio_state, track_node);
			return;
		}
	}
	if (track->next_note >= track->note_count) {
		return;
	}
	while (track->current_time >= track->notes[track->next_note].start_time * 60 / track->bpm) {
		const struct Note *note = &track->notes[track->next_note++];
		play_note(&audio_state, track, note);

		if (track->next_note >= track->note_count) {
			return;
		}
	}
}

void update_note(struct PlayingNote* note_node, float dt) {
	note_node->time_in_note += dt;
	if (note_node->time_in_note >= note_node->note->duration * 60 / note_node->track->bpm) {
		remove_note(&audio_state, note_node);
	}
}

void audio_callback(void* userdata, Uint8* stream, int len) {
	struct AudioState* state = (struct AudioState*)userdata;
	float volume = 0.1;

	// Cast stream for audio format AUDIO_F32LSB
	float* stream_f32 = (float*)stream;
	// Get the length of the stream
	int len_f32 = len / sizeof(float);

	for (int i = 0; i < len_f32; i++) {
		struct PlayingTrack *song = state->playing_tracks;
		while (song) {
			update_track(song, 1.0 / state->sample_rate);
			song = song->next;
		}

		float wave = 0;

		struct PlayingNote *note = state->playing_notes;
		while (note) {
			// Update phase
			note->phase += note->note->frequency / state->sample_rate;
			if (note->phase >= 1) {
				note->phase -= 1;
			}

			float duration_of_note = note->note->duration * 60 / note->track->bpm;

			switch (note->note->instrument) {
				case MELODY: {
					float envelope = 1;
					// Generate envelope
					if (note->time_in_note < note->note->attack) {
						envelope = note->time_in_note / note->note->attack;
					} else if (note->time_in_note < note->note->attack + note->note->decay) {
						float decay = (note->time_in_note - note->note->attack) / note->note->decay;
						envelope = 1 - decay * (1 - note->note->sustain);
					} else if (note->time_in_note > duration_of_note - note->note->release) {
						float release = (note->time_in_note - (duration_of_note - note->note->release)) / note->note->release;
						envelope = (1 - release) * note->note->sustain;
					} else {
						envelope = note->note->sustain;
					}

					float melody = (note->phase < 0.5) ? 1 : -1;
					envelope *= 0.7;
					melody *= envelope;
					wave += melody;
					break;
				}
				case BASS: {
					float basswave = (note->phase < 0.25) ? 1.0 : -0.33;
					// Add basic low-pass filter
					float cutoff = 0.1; // Adjust between 0-1 to change filter frequency
					basswave = audio_state.low_pass_past_sample + cutoff * (basswave - audio_state.low_pass_past_sample);
					audio_state.low_pass_past_sample = basswave;
					wave += basswave;
					break;
				}
				case SNARE:
					wave += rand() / (float)RAND_MAX * 1 - 0.5;
					break;
			}

			note->time_in_note += 1.0 / state->sample_rate;
			if (note->time_in_note >= duration_of_note) {
				struct PlayingNote *next = note->next;
				remove_note(state, note);
				note = next;
			} else {
				note = note->next;
			}

			/*if (state->snare.time_in_note >= 0 && state->snare.time_in_note < 0.02) {*/
			/*	wave += rand() / (float)RAND_MAX * 1 - 0.5;*/
			/*}*/
		}
		wave *= volume;
		stream_f32[i] = wave;
		/*stream[i] = (wave + 1) * 127;*/
	}
};

int init_audio() {
	SDL_AudioSpec want, have;
	want.freq = 44100;
	want.format = AUDIO_F32LSB;
	want.channels = 1;
	want.samples = 4096;
	want.callback = audio_callback;
	want.userdata = &audio_state;

	if (SDL_OpenAudio(&want, &have) < 0) {
		SDL_Log("Failed to open audio: %s\n", SDL_GetError());
		exit(1);
	}
	// print AUDIO_F32LSB first
	/*printf("Audio format: %b\n", AUDIO_F32LSB);*/
	/*printf("Audio format: %b\n", have.format);*/
	/*printf("Audio format signed: %s\n", SDL_AUDIO_ISSIGNED(have.format) ? "true" : "false");*/
	/*printf("Audio format big endian: %s\n", SDL_AUDIO_ISBIGENDIAN(have.format) ? "true" : "false");*/
	/*printf("Audio format is float: %s\n", SDL_AUDIO_ISFLOAT(have.format) ? "true" : "false");*/
	/*printf("Audio format bit size: %d\n", SDL_AUDIO_BITSIZE(have.format));*/

	if (have.format != want.format) {
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to get desired audio format\n");
		SDL_CloseAudio();
		return -1;
	}

	audio_state.sample_rate = have.freq;
	audio_state.low_pass_past_sample = 0;

	return 0;
}

void play_music() {
	struct Track *melody = malloc(sizeof(struct Track));
	if (melody == NULL) {
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to allocate memory for melody track\n");
		return;
	}
	melody->notes = melody_notes;
	melody->length = 32;
	melody->note_count = sizeof(melody_notes) / sizeof(melody_notes[0]);
	melody->repeat = 1;
	melody->current_time = 0;
	melody->next_note = 0;
	melody->bpm = 150;

	struct Track *bass = malloc(sizeof(struct Track));
	if (bass == NULL) {
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to allocate memory for bass track\n");
		return;
	}
	bass->notes = bass_notes;
	bass->length = 32;
	bass->note_count = sizeof(bass_notes) / sizeof(bass_notes[0]);
	bass->repeat = 1;
	bass->current_time = 0;
	bass->next_note = 0;
	bass->bpm = 150;

	struct Track *snare = malloc(sizeof(struct Track));
	if (snare == NULL) {
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to allocate memory for snare track\n");
		return;
	}
	snare->notes = snare_timings;
	snare->length = 8;
	snare->note_count = sizeof(snare_timings) / sizeof(snare_timings[0]);
	snare->repeat = 1;
	snare->current_time = 0;
	snare->next_note = 0;
	snare->bpm = 150;

	play_track(&audio_state, melody);
	play_track(&audio_state, bass);
	play_track(&audio_state, snare);

	SDL_PauseAudio(0);
}

void stop_music() {
	while (audio_state.playing_tracks) {
		remove_track(&audio_state, audio_state.playing_tracks);
	}
}

void cleanup_audio() {
	SDL_CloseAudio();
}
