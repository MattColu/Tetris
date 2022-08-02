#include "gamegfx.h"
#include "game.h"
#include "string.h"
#include "stdio.h"
#include "../GLCD/GLCD.h"

uint8_t blockShading[BLOCK_SIZE][BLOCK_SIZE] =
{
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3},
	{1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3},
	{1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 3},
	{1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3},
	{1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3},
	{1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3},
	{1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3},
	{1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3},
	{1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3},
	{1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3},
	{1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3},
	{1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3},
	{1, 1, 1, 4, 4, 4, 4, 4, 4, 4, 4, 4, 3, 3, 3},
	{1, 1, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 3, 3},
	{1, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 3},
};

uint8_t ghostShading[BLOCK_SIZE][BLOCK_SIZE] =
{
	{2, 2, 2, 2, 255, 255, 255, 255, 255, 255, 255, 2, 2, 2, 2},
	{2, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 2},
	{2, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 2},
	{2, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 2},
	{255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255},
	{255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255},
	{255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255},
	{255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255},
	{255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255},
	{255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255},
	{255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255},
	{2, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 2},
	{2, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 2},
	{2, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 2},
	{2, 2, 2, 2, 255, 255, 255, 255, 255, 255, 255, 2, 2, 2, 2},
};

uint8_t bgShading[BG_NUMBER][BLOCK_SIZE][BLOCK_SIZE] =
{
	{
		{4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4},
		{4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4},
		{4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4},
		{4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4},
		{4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4},
		{4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4},
		{4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4},
		{4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4},
		{4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4},
		{4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4},
		{4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4},
		{4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4},
		{4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4},
		{4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4},
		{4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4},
	},
	{
		{3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3},
		{3, 4, 4, 4, 4, 4, 4, 3, 4, 4, 4, 4, 4, 4, 3},
		{3, 4, 4, 4, 4, 4, 4, 3, 4, 4, 4, 4, 4, 4, 3},
		{3, 4, 4, 4, 4, 4, 4, 3, 4, 4, 4, 4, 4, 4, 3},
		{3, 4, 4, 4, 4, 4, 4, 3, 4, 4, 4, 4, 4, 4, 3},
		{3, 4, 4, 4, 4, 4, 4, 3, 4, 4, 4, 4, 4, 4, 3},
		{3, 4, 4, 4, 4, 4, 4, 3, 4, 4, 4, 4, 4, 4, 3},
		{3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3},
		{3, 4, 4, 4, 4, 4, 4, 3, 4, 4, 4, 4, 4, 4, 3},
		{3, 4, 4, 4, 4, 4, 4, 3, 4, 4, 4, 4, 4, 4, 3},
		{3, 4, 4, 4, 4, 4, 4, 3, 4, 4, 4, 4, 4, 4, 3},
		{3, 4, 4, 4, 4, 4, 4, 3, 4, 4, 4, 4, 4, 4, 3},
		{3, 4, 4, 4, 4, 4, 4, 3, 4, 4, 4, 4, 4, 4, 3},
		{3, 4, 4, 4, 4, 4, 4, 3, 4, 4, 4, 4, 4, 4, 3},
		{3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3},
	},
	{
		{3, 4, 4, 4, 4, 4, 4, 3, 4, 4, 4, 4, 4, 4, 3},
		{4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4},
		{4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4},
		{4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4},
		{4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4},
		{4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4},
		{4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4},
		{3, 4, 4, 4, 4, 4, 4, 3, 4, 4, 4, 4, 4, 4, 3},
		{4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4},
		{4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4},
		{4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4},
		{4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4},
		{4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4},
		{4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4},
		{3, 4, 4, 4, 4, 4, 4, 3, 4, 4, 4, 4, 4, 4, 3},
	},
	{
		{3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 3},
		{4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 3, 3},
		{4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 3, 3, 4},
		{4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 3, 3, 4, 4},
		{4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 3, 3, 4, 4, 4},
		{4, 4, 4, 4, 4, 4, 4, 4, 4, 3, 3, 4, 4, 4, 4},
		{4, 4, 4, 4, 4, 4, 4, 4, 3, 3, 4, 4, 4, 4, 4},
		{4, 4, 4, 4, 4, 4, 4, 3, 3, 4, 4, 4, 4, 4, 4},
		{4, 4, 4, 4, 4, 4, 3, 3, 4, 4, 4, 4, 4, 4, 4},
		{4, 4, 4, 4, 4, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4},
		{4, 4, 4, 4, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4},
		{4, 4, 4, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4},
		{4, 4, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4},
		{4, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4},
		{3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4},
	},
	{
		{4, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4},
		{3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4},
		{3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 3},
		{3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 3, 3},
		{3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 3, 3, 3},
		{3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 3, 3, 3, 3},
		{3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 3, 3, 3, 3, 3},
		{3, 3, 3, 3, 3, 3, 3, 4, 4, 3, 3, 3, 3, 3, 3},
		{3, 3, 3, 3, 3, 3, 4, 4, 3, 3, 3, 3, 3, 3, 3},
		{3, 3, 3, 3, 3, 4, 4, 3, 3, 3, 3, 3, 3, 3, 3},
		{3, 3, 3, 3, 4, 4, 3, 3, 3, 3, 3, 3, 3, 3, 3},
		{3, 3, 3, 4, 4, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3},
		{3, 3, 4, 4, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3},
		{3, 4, 4, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3},
		{4, 4, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3},
	},
	{
		{4, 3, 4, 3, 4, 3, 4, 3, 4, 3, 4, 3, 4, 3, 4},
		{3, 4, 3, 4, 3, 4, 3, 4, 3, 4, 3, 4, 3, 4, 3},
		{4, 3, 4, 3, 4, 3, 4, 3, 4, 3, 4, 3, 4, 3, 4},
		{3, 4, 3, 4, 3, 4, 3, 4, 3, 4, 3, 4, 3, 4, 3},
		{4, 3, 4, 3, 4, 3, 4, 3, 4, 3, 4, 3, 4, 3, 4},
		{3, 4, 3, 4, 3, 4, 3, 4, 3, 4, 3, 4, 3, 4, 3},
		{4, 3, 4, 3, 4, 3, 4, 3, 4, 3, 4, 3, 4, 3, 4},
		{3, 4, 3, 4, 3, 4, 3, 4, 3, 4, 3, 4, 3, 4, 3},
		{4, 3, 4, 3, 4, 3, 4, 3, 4, 3, 4, 3, 4, 3, 4},
		{3, 4, 3, 4, 3, 4, 3, 4, 3, 4, 3, 4, 3, 4, 3},
		{4, 3, 4, 3, 4, 3, 4, 3, 4, 3, 4, 3, 4, 3, 4},
		{3, 4, 3, 4, 3, 4, 3, 4, 3, 4, 3, 4, 3, 4, 3},
		{4, 3, 4, 3, 4, 3, 4, 3, 4, 3, 4, 3, 4, 3, 4},
		{3, 4, 3, 4, 3, 4, 3, 4, 3, 4, 3, 4, 3, 4, 3},
		{4, 3, 4, 3, 4, 3, 4, 3, 4, 3, 4, 3, 4, 3, 4},
	},
	{
		{4, 4, 4, 4, 4, 4, 4, 3, 4, 4, 4, 4, 4, 4, 4},
		{4, 4, 4, 4, 4, 4, 3, 3, 3, 4, 4, 4, 4, 4, 4},
		{4, 4, 4, 4, 4, 4, 3, 3, 3, 4, 4, 4, 4, 4, 4},
		{3, 4, 4, 4, 4, 3, 3, 3, 3, 3, 4, 4, 4, 4, 3},
		{3, 4, 4, 4, 4, 3, 3, 3, 3, 3, 4, 4, 4, 4, 3},
		{3, 3, 4, 4, 3, 3, 3, 3, 3, 3, 3, 4, 4, 3, 3},
		{3, 3, 4, 4, 3, 3, 3, 3, 3, 3, 3, 4, 4, 3, 3},
		{4, 4, 4, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4},
		{4, 4, 4, 3, 3, 4, 4, 4, 4, 4, 3, 3, 4, 4, 4},
		{4, 4, 3, 3, 3, 3, 4, 4, 4, 3, 3, 3, 3, 4, 4},
		{4, 4, 3, 3, 3, 3, 4, 4, 4, 3, 3, 3, 3, 4, 4},
		{4, 3, 3, 3, 3, 3, 3, 4, 3, 3, 3, 3, 3, 3, 4},
		{4, 3, 3, 3, 3, 3, 3, 4, 3, 3, 3, 3, 3, 3, 4},
		{3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3},
		{3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3},
	},
};

uint16_t LBluePalette[PALETTE_SIZE] = {RGB565CONVERT(179,255,255), RGB565CONVERT(77,255,255), RGB565CONVERT(0,255,255), RGB565CONVERT(0,178,178), RGB565CONVERT(0,76,76)};
uint16_t OrangePalette[PALETTE_SIZE] = {RGB565CONVERT(255,214,279), RGB565CONVERT(155,160,77), RGB565CONVERT(255,119,0), RGB565CONVERT(178,83,0), RGB565CONVERT(76,35,0)};
uint16_t BluePalette[PALETTE_SIZE] = {RGB565CONVERT(179,179,230), RGB565CONVERT(77,77,196), RGB565CONVERT(0,0,170), RGB565CONVERT(0,0,119), RGB565CONVERT(0,0,51)};
uint16_t PurplePalette[PALETTE_SIZE] = {RGB565CONVERT(240,179,240), RGB565CONVERT(219,77,219), RGB565CONVERT(204,0,204), RGB565CONVERT(142,0,142), RGB565CONVERT(61,0,61)};
uint16_t GreenPalette[PALETTE_SIZE] = {RGB565CONVERT(179,255,179), RGB565CONVERT(77,255,77), RGB565CONVERT(0,255,0), RGB565CONVERT(0,178,0), RGB565CONVERT(0,76,0)};
uint16_t RedPalette[PALETTE_SIZE] = {RGB565CONVERT(255,179,179), RGB565CONVERT(255,77,77), RGB565CONVERT(255,0,0), RGB565CONVERT(178,0,0), RGB565CONVERT(76,0,0)};
uint16_t YellowPalette[PALETTE_SIZE] = {RGB565CONVERT(255,255,179), RGB565CONVERT(255,255,77), RGB565CONVERT(255,255,0), RGB565CONVERT(178,178,0), RGB565CONVERT(76,76,0)};
uint16_t GreyPalette[PALETTE_SIZE] = {RGB565CONVERT(214,214,214), RGB565CONVERT(160,160,160), RGB565CONVERT(119,119,119), RGB565CONVERT(83,83,83), RGB565CONVERT(35,35,35)};
uint16_t BlackPalette[PALETTE_SIZE] = {RGB565CONVERT(80,80,80), RGB565CONVERT(60,60,60), RGB565CONVERT(40,40,40), RGB565CONVERT(20,20,20), RGB565CONVERT(0,0,0)};

uint16_t *PaletteArray[PALETTE_NUMBER] = {LBluePalette, OrangePalette, BluePalette, PurplePalette, GreenPalette, RedPalette, YellowPalette, GreyPalette, BlackPalette};

uint8_t bgType;
uint8_t bgPalette;

uint8_t garbagePalette = 7;

uint8_t frameThickness = 3;
uint8_t frameBlinkingStatus = 1;
uint8_t frameBlinkingTimer = 0;
uint8_t frameBlinkingSpeed = 20;

optioncontrol ValueCtrl = {Value, 56, 16};
optioncontrol SwatchCtrl = {Swatch, 3 * BLOCK_SIZE, BLOCK_SIZE + 1};
optioncontrol BgSwatchCtrl = {BgSwatch, 3 * BLOCK_SIZE, BLOCK_SIZE};
optioncontrol OnOffCtrl = {OnOff, 16, 16};
optioncontrol JustLabelCtrl = {JustLabel, 0, 0};
optioncontrol ButtonCtrl = {Button, 0, 0};

//offsetXL, offsetXR, offsetYU, offsetYD, *options, optionNumber, selected, highlightSelected, textSize, alignment, style
choiceScreen pauseMenu = {20, 20, 100, 30, pauseOptions, PAUSE_OPTION_NUMBER, 0, PAUSE_OPTION_NUMBER, 1, AlignLeft, Invert};
choiceScreen mainMenu = {20, 20, 30, 30, mainMenuOptions, MAIN_MENU_OPTION_NUMBER, 0, MAIN_MENU_OPTION_NUMBER, 2, AlignCenter, Darken};
choiceScreen creditsMenu = {10, 10, 60, 20, creditsOptions, CREDITS_OPTION_NUMBER, 0, CREDITS_OPTION_NUMBER, 1, AlignCenter, Darken};
choiceScreen settingsMenu = {20, 20, 100, 30, settingOptions, SETTINGS_OPTION_NUMBER, 0, PAUSE_OPTION_NUMBER, 1, AlignLeft, Invert};
choiceScreen controlsMenu = {20, 20, 60, 20, controlsOptions, CONTROLS_OPTION_NUMBER, 0, CONTROLS_OPTION_NUMBER, 1, AlignCenter, Darken};
choiceScreen lobbyMenu = {20, 20, 55, 155, lobbyOptions, LOBBY_OPTION_NUMBER, 0, LOBBY_OPTION_NUMBER, 1, AlignCenter, Darken};

selection theSelection;

uint8_t console[CONSOLE_LINES][CONSOLE_STRING_LEN];

void draw_piece(piece *p, piecegameplaytype type) {
	uint8_t i, j;
	for (i = 0; i < TETRA_SIZE; i++) {
		for (j = 0; j < TETRA_SIZE; j++) {
			if (p->archetype->shape[p->rotation][i][j]) {
				switch (type) {
					case Current:
						LCD_DrawBlock(SCREEN_OFFSET_X + BLOCK_SIZE * p->x + BLOCK_SIZE * j, SCREEN_OFFSET_Y + BLOCK_SIZE * p->y + BLOCK_SIZE * i, blockShading, PaletteArray[p->archetype->typeN]);
					break;
					case Next:
						LCD_DrawBlock(/*SCREEN_OFFSET_X*/ + BLOCK_SIZE * nextX + BLOCK_SIZE * j, SCREEN_OFFSET_Y + BLOCK_SIZE * nextY + BLOCK_SIZE * i, blockShading, PaletteArray[p->archetype->typeN]);
					break;
					case Bagged:
						LCD_DrawBlock(/*SCREEN_OFFSET_X*/ + BLOCK_SIZE * baggedX + BLOCK_SIZE * j, SCREEN_OFFSET_Y + BLOCK_SIZE * baggedY + BLOCK_SIZE * i, blockShading, PaletteArray[p->archetype->typeN]);
					break;
				}
			}
		}
	}
	return;
}

void draw_ghost_piece(piece *p, uint8_t withOffset) {
	uint8_t i, j;
	for (i = 0; i < TETRA_SIZE; i++) {
		for (j = 0; j < TETRA_SIZE; j++) {
			if (p->archetype->shape[p->rotation][i][j]) {
				if (withOffset) {
					LCD_DrawGhostBlock(SCREEN_OFFSET_X + BLOCK_SIZE * p->x + BLOCK_SIZE * j, SCREEN_OFFSET_Y + BLOCK_SIZE * p->y + BLOCK_SIZE * i, ghostShading, PaletteArray[p->archetype->typeN]);
				} else {
					LCD_DrawGhostBlock(BLOCK_SIZE * p->x + BLOCK_SIZE * j, BLOCK_SIZE * p->y + BLOCK_SIZE * i, ghostShading, PaletteArray[p->archetype->typeN]);
				}
			}
		}
	}
	return;
}

void draw_playfield(int8_t playfield[PLAYFIELD_Y][PLAYFIELD_X]) {
	uint8_t i, j;
	for (i = 0; i < PLAYFIELD_Y; i++) {
		for (j = 0; j < PLAYFIELD_X; j++) {
			if (playfield[i][j] != -1) {
				LCD_DrawBlock(SCREEN_OFFSET_X + BLOCK_SIZE * j, SCREEN_OFFSET_Y + BLOCK_SIZE * i, blockShading, PaletteArray[playfield[i][j]]);
			} else {
				LCD_DrawBlock(SCREEN_OFFSET_X + BLOCK_SIZE * j, SCREEN_OFFSET_Y + BLOCK_SIZE * i, bgShading[bgType], PaletteArray[bgPalette]);
			}
		}
	}
	return;
}

void draw_playfield_frame(uint16_t color) {
	LCD_DrawHollowRectangle(
			SCREEN_OFFSET_X,
			SCREEN_OFFSET_Y,
			BLOCK_SIZE * PLAYFIELD_X,
			BLOCK_SIZE * PLAYFIELD_Y,
			frameThickness,
			color);
	return;
}

void configure_mini_playfields(void) {
	uint8_t i, j, k = 0, size, row[3], offsetX, offsetY, oneOffX;
	switch (nPlayers) {
		case 1:
			size = 0;
			oneOffX = 0;
			offsetX = 0;
			offsetY = 0;
			row[0] = 0;
			row[1] = 0;
			row[2] = 0;
		break;
		case 2:
			size = 4;
			oneOffX = 14;
			offsetX = 0;
			offsetY = 0;
			row[0] = 1;
			row[1] = 0;
			row[2] = 0;
		break;
		case 3:
			size = 3;
			oneOffX = 17;
			offsetX = 0;
			offsetY = 10;
			row[0] = 1;
			row[1] = 1;
			row[2] = 0;
		break;
		case 4:
			size = 3;
			oneOffX = 0;
			offsetX = 5;
			offsetY = 10;
			row[0] = 2;
			row[1] = 1;
			row[2] = 0;
		break;
		case 5:
			size = 3;
			oneOffX = 0;
			offsetX = 5;
			offsetY = 10;
			row[0] = 2;
			row[1] = 2;
			row[2] = 0;
		break;
		default:
			size = 2;
			oneOffX = 0;
			offsetX = 60 - size * 2 * PLAYFIELD_X;
			offsetY = 5;
			row[0] = 2;
			row[1] = 2;
			row[2] = nPlayers - 4;
		break;
	}
	for (i = 0; i < 3; i++) {
		for (j = 0; j < row[i]; j++) {
			if (k == self->id) k++;
			players[k].playfieldX = MINI_PLAYFIELD_OFFSET_X + oneOffX + j * size * PLAYFIELD_X + j * offsetX;
			players[k].playfieldY = MINI_PLAYFIELD_OFFSET_Y + i * size * PLAYFIELD_Y + i * offsetY;
			players[k].playfieldSize = size;
			k++;
		}
	}
	return;
}

void draw_mini_playfield(player *p) {
	uint8_t i, j;
	uint8_t size = p->playfieldSize;
	for (i = 0; i < size * PLAYFIELD_Y; i++) {
		for (j = 0; j < size * PLAYFIELD_X; j++) {
			if (p->playfield[i/size][j/size] != -1) {
				LCD_SetPoint(p->playfieldX + j, p->playfieldY + i, PaletteArray[p->playfield[i/size][j/size]][PALETTE_SIZE/2]);
			} else {
				LCD_SetPoint(p->playfieldX + j, p->playfieldY + i, PaletteArray[bgPalette][PALETTE_SIZE - 1]);
			}
		}
	}
	return;
}

void draw_all_mini_playfields(void) {
	uint8_t i = 0;
	for (i = 0; i < nPlayers; i++) {
		if (i != self->id) {
			draw_mini_playfield_frame(&players[i]);
			draw_mini_playfield(&players[i]);
		}
	}
	return;
}

void draw_mini_playfield_frame(player *p) {
	LCD_DrawHollowRectangle(
			p->playfieldX,
			p->playfieldY,
			p->playfieldSize * PLAYFIELD_X,
			p->playfieldSize * PLAYFIELD_Y,
			p->playfieldSize - 1,
			PaletteArray[p->colorIndex][PALETTE_SIZE/2]);
	return;
}

void clear_mini_playfield_frame(player *p) {
	LCD_DrawHollowRectangle(
			p->playfieldX,
			p->playfieldY,
			p->playfieldSize * PLAYFIELD_X,
			p->playfieldSize * PLAYFIELD_Y,
			p->playfieldSize - 1,
			PaletteArray[p->colorIndex][PALETTE_SIZE-1]);
	return;
}

void ko_mini_playfield(player *p) {
	uint16_t x, y, size;
	size = p->playfieldSize - 1;
	x = p->playfieldX + (p->playfieldSize * PLAYFIELD_X)/2 - 8 * size;
	y = p->playfieldY + (p->playfieldSize * PLAYFIELD_Y)/2 - 8 * size;
	GUI_LegacyExtraText(x, y, (uint8_t*)"KO", size, White, Black);
	return;
}

void draw_background_next (void) {
	uint8_t i, j;
	GUI_Text(BLOCK_SIZE * nextX + 7, SCREEN_OFFSET_Y + BLOCK_SIZE * (nextY - 1), (uint8_t *)"NEXT", White, Black);
	for (i = 0; i < NEXT_BG_SIZE_Y; i++) {
		for (j = 0; j < NEXT_BG_SIZE_X; j++) {
			LCD_DrawBlock(BLOCK_SIZE * nextX + BLOCK_SIZE * j, SCREEN_OFFSET_Y + BLOCK_SIZE * nextY + BLOCK_SIZE * i, bgShading[bgType], PaletteArray[bgPalette]);
		}
	}
}

void draw_bagged_next (void) {
	uint8_t i, j;
	GUI_Text(BLOCK_SIZE * baggedX + 7, SCREEN_OFFSET_Y + BLOCK_SIZE * (baggedY - 1), (uint8_t *)"HOLD", White, Black);
	for (i = 0; i < NEXT_BG_SIZE_Y; i++) {
		for (j = 0; j < NEXT_BG_SIZE_X; j++) {
			LCD_DrawBlock(BLOCK_SIZE * baggedX + BLOCK_SIZE * j, SCREEN_OFFSET_Y + BLOCK_SIZE * baggedY + BLOCK_SIZE * i, bgShading[bgType], PaletteArray[bgPalette]);
		}
	}
}

void patch_piece(piece *p, piecegameplaytype type, uint8_t force) {
	uint8_t i, j;
	if (!force) {
		if (p->x == p->oldX && p->y == p->oldY && p->rotation == p->oldRotation) return;
	}
	for (i = 0; i < TETRA_SIZE; i++) {
		for (j = 0; j < TETRA_SIZE; j++) {
			if (p->archetype->shape[p->oldRotation][i][j]) {
				switch (type) {
					case Current:
						LCD_DrawBlock(SCREEN_OFFSET_X + BLOCK_SIZE * p->oldX + BLOCK_SIZE * j, SCREEN_OFFSET_Y + BLOCK_SIZE * p->oldY + BLOCK_SIZE * i, bgShading[bgType], PaletteArray[bgPalette]);
					break;
					case Next:
						LCD_DrawBlock(BLOCK_SIZE * nextX + BLOCK_SIZE * j, SCREEN_OFFSET_Y + BLOCK_SIZE * nextY + BLOCK_SIZE * i, bgShading[bgType], PaletteArray[bgPalette]);
					break;
					case Bagged:
						LCD_DrawBlock(BLOCK_SIZE * baggedX + BLOCK_SIZE * j, SCREEN_OFFSET_Y + BLOCK_SIZE * baggedY + BLOCK_SIZE * i, bgShading[bgType], PaletteArray[bgPalette]);
					break;
				}
			}
		}
	}
}

void draw_option_screen(choiceScreen *screen) {
	uint8_t i;
	for (i = 0; i < screen->optionNumber; i++) {
		draw_option(screen, i);
	}
	
}

void draw_option(choiceScreen *screen, uint8_t i) {
	uint8_t tstr[8] = "\x8C     \x8D";
	uint8_t j;
	uint16_t posx, posy, vpadding, ctrlposx;
	vpadding = (MAX_Y - screen->offsetYU - screen->offsetYD - screen->optionNumber * screen->textSize * 16)/(screen->optionNumber - 1);
	posy = screen->offsetYU + i * (screen->textSize * 16 + vpadding);
	switch (screen->alignment) {
		case AlignLeft:
			posx = screen->offsetXL;
			ctrlposx = MAX_X - screen->offsetXR - screen->options[i].controller->sizeX;
		break;
		case AlignCenter:
			if (screen->options[i].controller->type == JustLabel || screen->options[i].controller->type == Button) {
				posx = screen->offsetXL + (MAX_X - screen->offsetXL - screen->offsetXR - strlen((char*)screen->options[i].string) * screen->textSize * 8)/2;
			} else {
				posx = screen->offsetXL + ((MAX_X - screen->offsetXL - screen->offsetXR)/2 - strlen((char*)screen->options[i].string)) * screen->textSize * 8/2;
			}
		break;
		/*case AlignRight:
		break;*/
	}
	GUI_ExtraText(posx, posy, screen->options[i].string, screen->textSize, White, Black);
	switch (screen->options[i].controller->type) {
		case JustLabel:
		break;
		case Button:
		break;
		case Value:
			GUI_Text(ctrlposx, posy, tstr, White, Black);
			sprintf((char *)tstr, " %d ", *(uint8_t *)(screen->options[i].option));
			ctrlposx = ctrlposx + (8 * (7 - strlen((char *)tstr)))/2;
			GUI_LegacyText(ctrlposx, posy, tstr, White, Black);
		break;
		case Swatch:
			LCD_DrawRectangle(ctrlposx, posy, screen->options[i].controller->sizeX, screen->options[i].controller->sizeY, PaletteArray[*((uint8_t *)(screen->options[i].option))][PALETTE_SIZE-2]);
		break;
		case BgSwatch:
			for (j = 0; j < 3; j++) {
				LCD_DrawBlock(ctrlposx + j * BLOCK_SIZE, posy, bgShading[bgType], PaletteArray[bgPalette]);
			}
		break;
		case OnOff:
			if (*((uint8_t *)(screen->options[i].option))) {
				tstr[0] = 97 + 32;
				tstr[1] = 98 + 32;
			} else {
				tstr[0] = 95 + 32;
				tstr[1] = 96 + 32;
			}
			tstr[2] = 0;
			GUI_LegacyText(ctrlposx, posy, tstr, White, Black);
		break;
	}
	if (i == screen->selected) {
		if (screen->highlightSelected != screen->selected) {
			if (screen->highlightSelected != screen->optionNumber) {
				restore_selection_pixels(&theSelection);
			}
			theSelection.x = screen->offsetXL;
			theSelection.y = posy;
			theSelection.sizeX = MAX_X - screen->offsetXL - screen->offsetXR;
			theSelection.sizeY = 16 * screen->textSize;
			load_selection_pixels(&theSelection);
			switch (screen->style) {
				case Invert:
					smart_invert_selection_pixels(&theSelection);
				break;
				case Darken:
					smart_darken_selection_pixels(&theSelection);
				break;
			}
			screen->highlightSelected  = i;
		}
	}
}

void reset_selection(choiceScreen *screen) {
	screen->selected = 0;
	screen->highlightSelected = screen->optionNumber;
	return;
}

void manage_option_input(choiceScreen *screen) {
	uint8_t i = screen->selected;
	uint8_t changed = 1;
	switch(inputStatus) {
		case JSELECT:
		case KEY1:
			switch(screen->options[i].controller->type) {
				case Button:
					*((status *)(screen->options[i].option)) = (status)screen->options[i].max;
				break;
				case OnOff:
					*((uint8_t*)screen->options[i].option) = !(*((uint8_t*)screen->options[i].option));
				break;
				default:
					changed = 0;
				break;
			}
		break;
		case JUP:
			decrease_option_value(&(screen->selected), screen->optionNumber);
		break;
		case JDOWN:
			increase_option_value(&(screen->selected), screen->optionNumber);
		break;
		case JLEFT:
			switch(screen->options[i].controller->type){
				case Value:
				case Swatch:
				case BgSwatch:
					decrease_option_value(screen->options[i].option, screen->options[i].max);
				break;
				default:
					changed = 0;
				break;
			}
		break;
		case JRIGHT:
			switch(screen->options[i].controller->type){
				case Value:
				case Swatch:
				case BgSwatch:
					increase_option_value(screen->options[i].option, screen->options[i].max);
				break;
				default:
					changed = 0;
				break;
			}
		break;	
		default:
			changed = 0;
		break;
	}
	if (changed) {
		restore_selection_pixels(&theSelection);
		screen->highlightSelected = screen->optionNumber;
		draw_option_screen(screen);
	}
}

void draw_boot_image(void) {
	uint8_t i, b1, b2, b3;
	uint8_t off = 0x36U;
	uint16_t palette[25];
	for (i = 0; i < 25; i++) {
		b3 = imageFileData[off + 4*i];
		b2 = imageFileData[off + 4*i + 1];
		b1 = imageFileData[off + 4*i + 2];
		palette[i] = RGB565CONVERT(b1, b2, b3);
	}
	LCD_StreamFromFile(imageFileData, imageFileData[0x0A], palette);
	return;
}

void console_print(char *str) {
	console_push(str);
	console_refresh();
	return;
}

void console_refresh(void) {
	uint8_t i;
	uint8_t spaces[CONSOLE_STRING_LEN];
	memset(spaces, ' ', CONSOLE_STRING_LEN-1);
	spaces[CONSOLE_STRING_LEN - 1] = 0;
	
	if (gameStatus != Lobby && gameStatus != OpeningLobby) return;
	for (i = 0; i < CONSOLE_LINES; i++) {
		if (console[i][0] == 0) continue;
		GUI_LegacyText(CONSOLE_OFFSET_X, CONSOLE_OFFSET_Y + (CONSOLE_LINES - 1 - i) * 16, spaces, White, Black);
		GUI_Text(CONSOLE_OFFSET_X, CONSOLE_OFFSET_Y + (CONSOLE_LINES - 1 - i) * 16, console[i], White, Black);
	}
	return;
}

void console_push(char *str) {
	int8_t i;
	for (i = CONSOLE_LINES - 2; i >= 0; i--) {
		strcpy((char *)console[i+1], (char *)console[i]);
	}
	strcpy((char *)console[0], str);
	return;
}

void console_clear(void) {
	memset(console, 0, CONSOLE_LINES * CONSOLE_STRING_LEN);
	return;
}
