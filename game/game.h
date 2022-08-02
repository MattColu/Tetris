#include "LPC17xx.H"
#include <stdlib.h>
#include <string.h>
#include "../GLCD/GLCD.h"
#include "../can/s_can.h"

#ifndef _GAME_H
#define _GAME_H

#define PLAYFIELD_X 10
#define PLAYFIELD_Y 20

#define JOY_HOLD_TIME 10

#define START_X 3
#define START_Y 0

#define TETRA_SIZE 4
#define PIECE_TYPE_NUMBER 7

#define PAUSE_OPTION_NUMBER 5
#define SETTINGS_OPTION_NUMBER 6
#define MAIN_MENU_OPTION_NUMBER 5
#define CONTROLS_OPTION_NUMBER 14
#define CREDITS_OPTION_NUMBER 12
#define LOBBY_OPTION_NUMBER 8

#define OPTION_STRING_LEN 25

#define CONSOLE_STRING_LEN 27
#define CONSOLE_LINES 8

#define MAX_PLAYERS 7
#define PLAYER_NAME_LEN 11

#define MAX_GARBAGE_SIZE (MAX_PLAYERS - 1) * TETRA_SIZE

typedef enum {
	Boot,
	LoadingMainMenu,
	MainMenu,
	LoadingSinglePlayer,
	SinglePlayer,
	SinglePlayerGameOver,
	OpeningPause,
	Pause,
	ClosingPause,
	OpeningLobby,
	Lobby,
	//ClosingLobby,
	LoadingMultiPlayer,
	MultiPlayer,
	MultiPlayerGameOver,
	OpeningSettings,
	Settings,
	OpeningControls,
	Controls,
	OpeningCredits,
	Credits,
} status;

typedef enum {
	Up,
	Down,
	Left,
	Right,
} direction;

typedef enum {
	NONE,
	KEY1,
	KEY2,
	INT0,
	JUP,
	JDOWN,
	JLEFT,
	JRIGHT,
	JSELECT,
} input;

typedef enum {
	I,
	L,
	J,
	T,
	S,
	Z,
	Q
} piecetype;

typedef enum {
	Current,
	Next,
	Bagged
} piecegameplaytype;

typedef enum {
	Value,
	Swatch,
	BgSwatch,
	OnOff,
	JustLabel,
	Button,
} optioncontroltype;

typedef struct {
	optioncontroltype type;
	uint8_t sizeX;
	uint8_t sizeY;
} optioncontrol;

typedef struct {
	uint8_t string[OPTION_STRING_LEN];
	optioncontrol *controller;
	void *option;
	uint8_t min;
	uint8_t max;
} menuEntry;

typedef struct {
	piecetype type;
	uint8_t typeN;
	uint8_t shape[TETRA_SIZE][TETRA_SIZE][TETRA_SIZE];
} pieceArchetype;

typedef struct {
	uint8_t x;
	uint8_t y;
	uint8_t oldX;
	uint8_t oldY;
	uint8_t rotation;
	uint8_t oldRotation;
	pieceArchetype *archetype;
} piece;

typedef struct {
	uint8_t id;
	char name[PLAYER_NAME_LEN];
	uint8_t colorIndex;
	uint8_t ko;
	uint8_t target;
	int8_t playfield[PLAYFIELD_Y][PLAYFIELD_X];
	uint8_t playfieldX;
	uint16_t playfieldY;
	uint8_t playfieldSize;
	int8_t cleared[TETRA_SIZE];
	int8_t garbageQueue[MAX_GARBAGE_SIZE];
	uint8_t garbageSize;
} player;

extern status gameStatus;
extern input inputStatus;

extern uint8_t fallCounter;
extern uint8_t fallThreshold;
extern uint8_t thresholdDecreaseAmt;
extern uint8_t minThreshold;
extern uint8_t hasBaggedPiece;
extern uint8_t canBag;
extern uint8_t clearedLines;
extern uint8_t clearStreak;
extern uint8_t clearedTotal;
extern uint8_t clearsForSpeedup;

extern uint8_t nextX;
extern uint8_t nextY;
extern uint8_t baggedX;
extern uint8_t baggedY;

extern uint8_t ghostEnabled;

extern menuEntry pauseOptions[PAUSE_OPTION_NUMBER];
extern menuEntry mainMenuOptions[MAIN_MENU_OPTION_NUMBER];
extern menuEntry settingOptions[SETTINGS_OPTION_NUMBER];
extern menuEntry controlsOptions[CONTROLS_OPTION_NUMBER];
extern menuEntry creditsOptions[CREDITS_OPTION_NUMBER];
extern menuEntry lobbyOptions[LOBBY_OPTION_NUMBER];

extern piece currentPiece;
extern piece ghostPiece;
extern piece nextPiece;
extern piece baggedPiece;

extern pieceArchetype pieceI;
extern pieceArchetype pieceL;
extern pieceArchetype pieceJ;
extern pieceArchetype pieceT;
extern pieceArchetype pieceS;
extern pieceArchetype pieceZ;
extern pieceArchetype pieceQ;

extern player *self;
extern player players[MAX_PLAYERS];
extern uint8_t nPlayers;
extern uint8_t alivePlayers;
extern uint8_t winner;

extern pieceArchetype *archetypeArray[PIECE_TYPE_NUMBER];
extern piecetype typeArray[PIECE_TYPE_NUMBER];

void init_variables(void);
void init_game_variables(void);
void init_playfield(int8_t playfield[PLAYFIELD_Y][PLAYFIELD_X]);
void init_all_players(void);
void init_player(uint8_t i);
void init_archetypes(void);
void init_piece(piece *piece);
void init_ghost(piece *ghost, piece *piece);

void save_piece_old(piece *piece);
void copy_piece(piece *from, piece *to);
void bag_piece(piece *bagged, piece *p, piece *next);
uint8_t move_piece(piece *piece, direction direction);
uint8_t rotate_piece(piece *piece);
void project_ghost(piece *ghost, piece *piece);

void add_to_playfield(piece *piece, int8_t playfield[PLAYFIELD_Y][PLAYFIELD_X]);
uint8_t check_playfield(int8_t playfield[PLAYFIELD_Y][PLAYFIELD_X], int8_t cleared[TETRA_SIZE]);
void clear_lines(int8_t playfield[PLAYFIELD_Y][PLAYFIELD_X], int8_t cleared[TETRA_SIZE]);

void add_garbage_to_queue(uint8_t hole, player *p);
void apply_garbage_line(uint8_t hole, player *p);
uint8_t apply_garbage_queue(player *p);

void decrease_fall_threshold(void);
uint8_t check_cleared_total(void);
uint8_t check_game_over(void);

void cycle_next_target(void);

void increase_option_value(uint8_t *option, uint8_t max);
void decrease_option_value(uint8_t *option, uint8_t max);

void send_game_start_to_all(void);
void send_piece_to_all(piece *p);
void send_lines_to_target(uint8_t n, uint8_t target);
void send_ko_to_all(void);
void send_game_end_to_all(void);

void game_logic(void);

#endif
