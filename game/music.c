#include "music.h"
#include "../timer/timer.h"

/* k=1/f'*f/n  k=f/(f'*n) k=25MHz/(f'*45) */
const int freqs[97]={33979, 32076, 30276, 28563, 26969, 25449, 24029, 22676, 21400, 20202, 19065, 17997, 16989, 16033, 15134, 14285, 13484, 12728, 12012, 11338, 10702, 10101, 9534, 8998, 8493, 8017, 7567, 7143, 6741, 6363, 6006, 5669, 5351, 5051, 4767, 4500, 4247, 4009, 3784, 3571, 3371, 3182, 3003, 2834, 2675, 2525, 2384, 2250, 2123, 2004, 1892, 1786, 1685, 1591, 1502, 1417, 1338, 1263, 1192, 1125, 1062, 1002, 946, 893, 843, 795, 751, 709, 669, 631, 596, 562, 531, 501, 473, 446, 421, 398, 375, 354, 334, 316, 298, 281, 265, 251, 236, 223, 211, 199, 188, 177, 167, 158, 149, 141, 133};
/*
	C0 - C8
	16.35,17.32,18.35,19.45,20.60,21.83,23.12,24.50,25.96,27.50,29.14,30.87,32.70,34.65,36.71,38.89,41.20,43.65,46.25,49.00,51.91,55.00,58.27,61.74,65.41,69.30,73.42,77.78,82.41,87.31,92.50,98.00,103.83,110.00,116.54,123.47,130.81,138.59,146.83,155.56,164.81,174.61,185.00,196.00,207.65,220.00,233.08,246.94,261.63,277.18,293.66,311.13,329.63,349.23,369.99,392.00,415.30,440.00,466.16,493.88,523.25,554.37,587.33,622.25,659.26,698.46,739.99,783.99,830.61,880.00,932.33,987.77,1046.50,1108.73,1174.66,1244.51,1318.51,1396.91,1479.98,1567.98,1661.22,1760.00,1864.66,1975.53,2093.00,2217.46,2349.32,2489.02,2637.02,2793.83,2959.96,3135.96,3322.44,3520.00,3729.31,3951.07,4186.01,
	
	0   C0
	1   C#0
	2   D0
	3   D#0
	4   E0
	5   F0
	6   F#0
	7   G0
	8   G#0
	9   A0
	10  A#0
	11  B0
	
	12  C1
	13  C#1
	14  D1
	15  D#1
	16  E1
	17  F1
	18  F#1
	19  G1
	20  G#1
	21  A1
	22  A#1
	23  B1
	
	24  C2
	25  C#2
	26  D2
	27  D#2
	28  E2
	29  F2
	30  F#2
	31  G2
	32  G#2
	33  A2
	34  A#2
	35  B2
	
	36  C3
	37  C#3
	38  D3
	39  D#3
	40  E3
	41  F3
	42  F#3
	43  G3
	44  G#3
	45  A3
	46  A#3
	47  B3
	
	48  C4
	49  C#4
	50  D4
	51  D#4
	52  E4
	53  F4
	54  F#4
	55  G4
	56  G#4
	57  A4
	58  A#4
	59  B4
	
	60  C5
	61  C#5
	62  D5
	63  D#5
	64  E5
	65  F5
	66  F#5
	67  G5
	68  G#5
	69  A5
	70  A#5
	71  B5
	
	72  C6
	73  C#6
	74  D6
	75  D#6
	76  E6
	77  F6
	78  F#6
	79  G6
	80  G#6
	81  A6
	82  A#6
	83  B6
	
	84  C7
	85  C#7
	86  D7
	87  D#7
	88  E7
	89  F7
	90  F#7
	91  G7
	92  G#7
	93  A7
	94  A#7
	95  B7
	
	96  C8
*/

const uint16_t SinTable[WAVE_SIZE] =                                       /* ÕýÏÒ±í                       */
{
   500, 570, 638, 703, 765, 821, 872, 915, 949,
	976, 992,1000, 997, 985, 964, 933, 894, 847,
	794, 735, 671, 604, 535, 465, 396, 329, 265,
	206, 153, 106, 67 , 36 , 15 , 3  , 0  , 8  ,
	24 , 51 , 85 , 128, 179, 235, 297, 362, 430
};

const uint16_t SqTable[WAVE_SIZE] =
{
	700, 700, 700, 700, 700, 700, 700, 700, 700,
	700, 700, 700, 700, 700, 700, 700, 700, 700,
	700, 700, 700, 700, 500, 300, 300, 300, 300,
	300, 300, 300, 300, 300, 300, 300, 300, 300,
	300, 300, 300, 300, 300, 300, 300, 300, 300, 
};

const uint16_t TriTable[WAVE_SIZE] = {
	0  , 43 , 87 , 130, 174, 217, 261, 304, 348,
	391, 435, 478, 522, 565, 609, 652, 696, 739,
	783, 826, 870, 913, 957, 1000,955, 909, 864,
	818, 773, 727, 682, 636, 591, 545, 500, 455,
	409, 364, 318, 273, 227, 182, 136, 91 , 45
};

const uint16_t SawTable[WAVE_SIZE] = {
	0  , 22 , 44 , 67 , 89 , 111, 133, 156, 178,
	200, 222, 244, 267, 289, 311, 333, 356, 378,
	400, 422, 444, 467, 489, 511, 533, 556, 578,
	600, 622, 644, 667, 689, 711, 733, 756, 778,
	800, 822, 844, 867, 889, 911, 933, 956, 978
};

uint8_t masterVolume = 25;
uint8_t musicTheme = 1;

uint8_t tempoCounter = 0;
uint8_t currentSong = 0;
audioChannel channels[CHANNEL_N] = {{NULL, 0, 0, 0, 50, Square}, {NULL, 0, 0, 0, 50, Noise}};

void parse_line(uint8_t songN) {
	uint8_t i, n, halt = 0, loop = 0;
	uint16_t tProgress;
	song *s = &songs[songN];
	musicEvent *e;
	musicEvent line[CHANNEL_N];
	tProgress = s->songProgress;
	for (i = 0; i < CHANNEL_N; i++) {
		line[i] = s->eventList[tProgress][i];
		e = &line[i];
		switch (e->effect) {
			case Vo:
				channels[i].current.volume = e->eValue.v;
			break;
			case Ch:
				channels[i].channelVolume = e->eValue.v;
			break;
			case Dc:
				channels[i].decay = e->eValue.v;
			break;
			case Wf:
				channels[i].wf = e->eValue.w;
			break;
			case Lo:
				loop = 1;
				s->repeatCounter = 0;
				s->songProgress = e->eValue.v;
			break;
			case Rp:
				if (s->repeatCounter >= (e->eValue.v & 0x0F)) {
					loop = 0;
					s->repeatCounter = 0;
				} else {
					loop = 1;
					s->songProgress = ((e->eValue.v & 0xF0) >> 4);
					s->repeatCounter++;
				}
			break;
			case St:
				halt = 1;
			break;
			case _:
			break;
		}
		if (halt) {
			switch (i) {
				case 0:
					n = 0;
				break;
				case 1:
					n = 3;
				break;
			}
			disable_timer(n);
		}
		if (line[i].note != __) {
			channels[i].current.pitch = line[i].note;
			channels[i].noteVolume = channels[i].current.volume;
			switch (i) {
				case 0:
					n = 0;
				break;
				case 1:
					n = 3;
				break;
			}
			disable_timer(n);
			reset_timer(n);
			init_timer(n, freqs[channels[i].current.pitch]);
			enable_timer(n);
		}
	}
	if (!halt && !loop) s->songProgress++;
}

int16_t generate_sound(uint8_t ch) {
	uint16_t ret;
	float finalVol;
	audioChannel *c = &channels[ch];
	switch (c->wf) {
		case Square:
			ret = SqTable[c->waveCounter];
		break;
		case Triangle:
			ret = TriTable[c->waveCounter];
		break;
		case Sine:
			ret = SinTable[c->waveCounter];
		break;
		case Sawtooth:
			ret = SawTable[c->waveCounter];
		break;
		case Noise:
			ret = rand()%100;
		break;
		default:
			ret = SinTable[c->waveCounter];
		break;
	}
	c->waveCounter = (c->waveCounter + 1) % WAVE_SIZE;
	finalVol = (c->noteVolume * c->channelVolume * masterVolume) / (float)1000000;
	return ret * finalVol;
}

void apply_decay(void) {
	uint8_t i;
	for (i = 0; i < CHANNEL_N; i++) {
		audioChannel *c = &channels[i];
		if (c->noteVolume > c->decay) {
			c->noteVolume -= c->decay;
		} else {
			c->noteVolume = 0;
		}
	}
}

void start_song(uint8_t n) {
	currentSong = n;
	songs[n].songProgress = 0;
	songs[n].repeatCounter = 0;
}

void resume_song(uint8_t n) {
	uint8_t i;
	currentSong = n;
	for (i = 0; i < CHANNEL_N; i++) {
		channels[i].noteVolume = channels[i].current.volume;
	}
}

void audio_engine(void) {
	if (tempoCounter >= songs[currentSong].tempo) {
		parse_line(currentSong);
		tempoCounter = 0;
	} else {
		tempoCounter++;
	}
	
	apply_decay();
}
