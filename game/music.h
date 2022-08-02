#include "lpc17xx.h"
#include "stdlib.h"

#define __ 255

#define WAVE_SIZE 45
#define SONG_LENGTH 300

#define CHANNEL_N 2

typedef enum {
	Square,
	Triangle,
	Sine,
	Sawtooth,
	Noise
} waveform;

typedef struct {
	uint8_t pitch;
	uint8_t volume;
} note;

typedef enum {
	Vo,//Volume
	Ch,//Channel volume
	Dc,//Decay
	Wf,//Waveform
	Lo,//Loop
	Rp,//Repeat
	St,//Stop
	_, //Nop
} effect;

typedef struct {
	uint8_t note;
	effect effect;
	union {
		uint8_t v;
		waveform w;
	} eValue;
} musicEvent;

typedef struct {
	musicEvent eventList[SONG_LENGTH][CHANNEL_N];
	uint16_t songProgress;
	uint8_t tempo;
	uint8_t repeatCounter;
} song;

typedef struct {
	note current;
	uint8_t noteVolume;
	uint8_t decay;
	uint16_t waveCounter;
	uint8_t channelVolume;
	waveform wf;
} audioChannel;

extern song songs[];
extern uint8_t masterVolume;
extern uint8_t musicTheme;
extern uint8_t currentSong;

void parse_line(uint8_t songN);
int16_t generate_sound(uint8_t ch);
void apply_decay(void);
void start_song(uint8_t n);
void resume_song(uint8_t n);
void audio_engine(void); 
