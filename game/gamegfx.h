#include "LPC17xx.H"
#include "game.h"
#include "BootImage.h"

#ifndef _GAMEGFX_H
#define _GAMEGFX_H

#define BLOCK_SIZE 15
#define PALETTE_SIZE 5
#define PALETTE_NUMBER 9
#define BG_NUMBER 5

#define NEXT_BG_SIZE_X 3
#define NEXT_BG_SIZE_Y 4

#define SELECTION_MAX_SIZE_X 240
#define SELECTION_MAX_SIZE_Y 32

#define SCREEN_OFFSET_X 10
#define SCREEN_OFFSET_Y 10

#define MENU_OFFSET_X 20
#define MENU_OFFSET_Y 60

#define CONSOLE_OFFSET_X 20
#define CONSOLE_OFFSET_Y 20

#define MINI_PLAYFIELD_OFFSET_X 170
#define MINI_PLAYFIELD_OFFSET_Y 180

typedef struct {
	uint16_t x;
	uint16_t y;
	uint16_t sizeX;
	uint16_t sizeY;
	uint16_t previousPixels[SELECTION_MAX_SIZE_Y][SELECTION_MAX_SIZE_X];
} selection;

typedef enum{
	AlignLeft,
	//AlignRight,
	AlignCenter,
} layoutAlignment;

typedef enum {
	Invert,
	Darken,
} highlightStyle;

typedef struct {
	uint8_t offsetXL;
	uint8_t offsetXR;
	uint8_t offsetYU;
	uint8_t offsetYD;
	menuEntry *options;
	uint8_t optionNumber;
	uint8_t selected;
	uint8_t highlightSelected;
	uint8_t textSize;
	layoutAlignment alignment;
	highlightStyle style;
} choiceScreen;

extern uint16_t LBluePalette[PALETTE_SIZE];
extern uint16_t OrangePalette[PALETTE_SIZE];
extern uint16_t BluePalette[PALETTE_SIZE];
extern uint16_t PurplePalette[PALETTE_SIZE];
extern uint16_t GreenPalette[PALETTE_SIZE];
extern uint16_t RedPalette[PALETTE_SIZE];
extern uint16_t YellowPalette[PALETTE_SIZE];
extern uint16_t GreyPalette[PALETTE_SIZE];
extern uint16_t BlackPalette[PALETTE_SIZE];

extern uint16_t *PaletteArray[PALETTE_NUMBER];

extern uint8_t blockShading[BLOCK_SIZE][BLOCK_SIZE];
extern uint8_t bgShading[BG_NUMBER][BLOCK_SIZE][BLOCK_SIZE];

extern uint8_t bgType;
extern uint8_t bgPalette;

extern uint8_t garbagePalette;

extern uint8_t frameThickness;

extern optioncontrol ValueCtrl;
extern optioncontrol SwatchCtrl;
extern optioncontrol BgSwatchCtrl;
extern optioncontrol OnOffCtrl;
extern optioncontrol JustLabelCtrl;
extern optioncontrol ButtonCtrl;

extern choiceScreen pauseMenu;
extern choiceScreen mainMenu;

extern selection menuSelection;

void draw_piece(piece *p, piecegameplaytype type);
void draw_ghost_piece(piece *p, uint8_t withOffset);
void draw_playfield(int8_t playfield[PLAYFIELD_Y][PLAYFIELD_X]);
void draw_playfield_frame(uint16_t color);

void configure_mini_playfields(void);
void draw_mini_playfield(player *p);
void draw_mini_playfield_frame(player *p);
void draw_all_mini_playfields(void);
void ko_mini_playfield(player *p);

void draw_background_next (void);
void draw_bagged_next (void);
void patch_piece(piece *p, piecegameplaytype type, uint8_t force);

void draw_option_screen(choiceScreen *screen);
void draw_option(choiceScreen *screen, uint8_t i);
void reset_selection(choiceScreen *screen);
void manage_option_input(choiceScreen *screen);

void draw_boot_image(void);
void console_print(char *str);
void console_refresh(void);
void console_push(char *str);
void console_clear(void);

#endif
