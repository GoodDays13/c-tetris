#ifndef AUDIO_H
#define AUDIO_H

#include <SDL2/SDL.h>

enum instrument {
	MELODY,
	BASS,
	SNARE
};

struct Note {
	float frequency; // in Hz
	float start_time; // in beats
	float duration; // in beats
	float attack;
	float decay;
	float sustain;
	float release;
	enum instrument instrument;
};

struct PlayingNote {
	const struct Note *note;
	const struct Track * track;
	float time_in_note;
	float phase;
	struct PlayingNote *next;
	struct PlayingNote *prev;
};

struct Track {
	const struct Note * notes;
	int length; // in beats
	int note_count;
	int repeat; // 0 = no repeat, 1 = repeat
	double current_time;
	int next_note;
	float bpm;
};

struct PlayingTrack {
	struct Track *track;
	struct PlayingTrack *next;
	struct PlayingTrack *prev;
};

struct AudioState {
	struct PlayingTrack *playing_tracks;
	struct PlayingNote *playing_notes;
	int sample_rate;
	float low_pass_past_sample;
};

int init_audio(void);
void play_music(void);
void stop_music(void);
void cleanup_audio(void);

void play_track(struct AudioState *audio, struct Track *track);

#endif // AUDIO_H
