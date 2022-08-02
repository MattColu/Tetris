#include "game.h"
#include "gamegfx.h"
#include "music.h"

status gameStatus;
input inputStatus;

uint8_t fallCounter;
uint8_t fallThreshold;
uint8_t thresholdDecreaseAmt;
uint8_t minThreshold;
uint8_t hasBaggedPiece;
uint8_t canBag;
uint8_t clearedLines;
uint8_t clearStreak;
uint8_t clearedTotal;
uint8_t clearsForSpeedup;

uint8_t nextX;
uint8_t nextY;
uint8_t baggedX;
uint8_t baggedY;

uint8_t ghostEnabled;

player *self;
player players[MAX_PLAYERS];
uint8_t nPlayers;
uint8_t alivePlayers;
uint8_t winner;

menuEntry pauseOptions[PAUSE_OPTION_NUMBER] = {
	{"Background style", &BgSwatchCtrl, &bgType, 0, BG_NUMBER},
	{"Background color", &SwatchCtrl, &bgPalette, 0, PALETTE_NUMBER},
	{"Enable ghost piece", &OnOffCtrl, &ghostEnabled, 0, 1},
	{"Main volume", &ValueCtrl, &masterVolume, 0, 101},
	{"Quit", &ButtonCtrl, &gameStatus, 0, LoadingMainMenu},
};

menuEntry settingOptions[SETTINGS_OPTION_NUMBER] = {
	{"Background style", &BgSwatchCtrl, &bgType, 0, BG_NUMBER},
	{"Background color", &SwatchCtrl, &bgPalette, 0, PALETTE_NUMBER},
	{"Enable ghost piece", &OnOffCtrl, &ghostEnabled, 0, 1},
	{"Main volume", &ValueCtrl, &masterVolume, 0, 101},
	{"Music theme", &ValueCtrl, &musicTheme, 0, 3},
	{"Back to Main Menu", &ButtonCtrl, &gameStatus, 0, LoadingMainMenu},
};

menuEntry mainMenuOptions[MAIN_MENU_OPTION_NUMBER] = {
	{"Singleplayer", &ButtonCtrl, &gameStatus, 0, LoadingSinglePlayer},
	{"Multiplayer", &ButtonCtrl, &gameStatus, 0, OpeningLobby},
	{"Settings", &ButtonCtrl, &gameStatus, 0, OpeningSettings},
	{"Controls", &ButtonCtrl, &gameStatus, 0, OpeningControls},
	{"Credits", &ButtonCtrl, &gameStatus, 0, OpeningCredits},
};

menuEntry controlsOptions[CONTROLS_OPTION_NUMBER] = {
	{" ", &JustLabelCtrl, NULL, 0, 0},
	{"Move Piece*       \x8C\x8E\x8F\x8D", &JustLabelCtrl, NULL, 0, 0},
	{"Fast Drop*         \x8E\x8F\x8B", &JustLabelCtrl, NULL, 0, 0},
	{"Instant Drop        \x90\x91", &JustLabelCtrl, NULL, 0, 0},
	{"Rotate Piece        \x84\x85", &JustLabelCtrl, NULL, 0, 0},
	{"Hold Piece          \x86\x87", &JustLabelCtrl, NULL, 0, 0},
	{"(SingleP)Options    \x88\x89", &JustLabelCtrl, NULL, 0, 0},
	{"(MultiP)Next Target \x88\x89", &JustLabelCtrl, NULL, 0, 0},
	{" ", &JustLabelCtrl, NULL, 0, 0},
	{"Menu Navigate       \x8E\x8F", &JustLabelCtrl, NULL, 0, 0},
	{"Menu Accept      \x84\x85/\x90\x91", &JustLabelCtrl, NULL, 0, 0},
	{"Menu Back           \x88\x89", &JustLabelCtrl, NULL, 0, 0},
	{" ", &JustLabelCtrl, NULL, 0, 0},
	{"* Tip: You can hold \x8E\x8F", &JustLabelCtrl, NULL, 0, 0},
};

menuEntry creditsOptions[CREDITS_OPTION_NUMBER] = {
	{"This was created as", &JustLabelCtrl, NULL, 0, 0},
	{"Special Project for", &JustLabelCtrl, NULL, 0, 0},
	{"the ASE 2021/2022", &JustLabelCtrl, NULL, 0, 0},
	{"course, held by", &JustLabelCtrl, NULL, 0, 0},
	{"Prof. Paolo Bernardi.", &JustLabelCtrl, NULL, 0, 0},
	{" ", &JustLabelCtrl, NULL, 0, 0},
	{"TETRIS belongs to", &JustLabelCtrl, NULL, 0, 0 },
	{"The Tetris Company", &JustLabelCtrl, NULL, 0, 0},
	{"and I'm in no way", &JustLabelCtrl, NULL, 0, 0},
	{"affiliated to it.", &JustLabelCtrl, NULL, 0, 0},
	{" ", &JustLabelCtrl, NULL, 0, 0},
	{"\x83 Matteo Colucci 2022", &JustLabelCtrl, NULL, 0, 0},
};

menuEntry lobbyOptions[LOBBY_OPTION_NUMBER] = {
	{"Clear lines to send", &JustLabelCtrl, NULL, 0, 0},
	{"garbage to an opponent", &JustLabelCtrl, NULL, 0, 0},
	{"(cycle with \x88\x89)", &JustLabelCtrl, NULL, 0, 0},
	{"1 cleared \x8D 0 garbage", &JustLabelCtrl, NULL, 0, 0},
	{"2 cleared \x8D 1 garbage", &JustLabelCtrl, NULL, 0, 0},
	{"3 cleared \x8D 2 garbage", &JustLabelCtrl, NULL, 0, 0},
	{"4 cleared \x8D 4 garbage", &JustLabelCtrl, NULL, 0, 0},
	{"Up to 7 people can play!", &JustLabelCtrl, NULL, 0, 0},
};

piece currentPiece;
piece ghostPiece;
piece nextPiece;
piece baggedPiece;

pieceArchetype pieceI;
pieceArchetype pieceL;
pieceArchetype pieceJ;
pieceArchetype pieceT;
pieceArchetype pieceS;
pieceArchetype pieceZ;
pieceArchetype pieceQ;

uint8_t shapeArray[PIECE_TYPE_NUMBER][TETRA_SIZE][TETRA_SIZE][TETRA_SIZE] = 
{
	{
		{
			{1, 0, 0, 0},
			{1, 0, 0, 0},
			{1, 0, 0, 0},
			{1, 0, 0, 0},
		},
		{
			{1, 1, 1, 1},
			{0, 0, 0, 0},
			{0, 0, 0, 0},
			{0, 0, 0, 0},
		},
		{
			{1, 0, 0, 0},
			{1, 0, 0, 0},
			{1, 0, 0, 0},
			{1, 0, 0, 0},
		},
		{
			{1, 1, 1, 1},
			{0, 0, 0, 0},
			{0, 0, 0, 0},
			{0, 0, 0, 0},
		},
	},
	{
		{
			{1, 0, 0, 0},
			{1, 0, 0, 0},
			{1, 1, 0, 0},
			{0, 0, 0, 0},
		},
		{
			{1, 1, 1, 0},
			{1, 0, 0, 0},
			{0, 0, 0, 0},
			{0, 0, 0, 0},
		},
		{
			{1, 1, 0, 0},
			{0, 1, 0, 0},
			{0, 1, 0, 0},
			{0, 0, 0, 0},
		},
		{
			{0, 0, 1, 0},
			{1, 1, 1, 0},
			{0, 0, 0, 0},
			{0, 0, 0, 0},
		},
	},
	{
		{
			{0, 1, 0, 0},
			{0, 1, 0, 0},
			{1, 1, 0, 0},
			{0, 0, 0, 0},
		},
		{
			{1, 0, 0, 0},
			{1, 1, 1, 0},
			{0, 0, 0, 0},
			{0, 0, 0, 0},
		},
		{
			{1, 1, 0, 0},
			{1, 0, 0, 0},
			{1, 0, 0, 0},
			{0, 0, 0, 0},
		},
		{
			{1, 1, 1, 0},
			{0, 0, 1, 0},
			{0, 0, 0, 0},
			{0, 0, 0, 0},
		},
	},
	{
		{
			{0, 1, 0, 0},
			{1, 1, 1, 0},
			{0, 0, 0, 0},
			{0, 0, 0, 0},
		},
		{
			{1, 0, 0, 0},
			{1, 1, 0, 0},
			{1, 0, 0, 0},
			{0, 0, 0, 0},
		},
		{
			{1, 1, 1, 0},
			{0, 1, 0, 0},
			{0, 0, 0, 0},
			{0, 0, 0, 0},
		},
		{
			{0, 1, 0, 0},
			{1, 1, 0, 0},
			{0, 1, 0, 0},
			{0, 0, 0, 0},
		},
	},
	{
		{
			{0, 1, 1, 0},
			{1, 1, 0, 0},
			{0, 0, 0, 0},
			{0, 0, 0, 0},
		},
		{
			{1, 0, 0, 0},
			{1, 1, 0, 0},
			{0, 1, 0, 0},
			{0, 0, 0, 0},
		},
		{
			{0, 1, 1, 0},
			{1, 1, 0, 0},
			{0, 0, 0, 0},
			{0, 0, 0, 0},
		},
		{
			{1, 0, 0, 0},
			{1, 1, 0, 0},
			{0, 1, 0, 0},
			{0, 0, 0, 0},
		},
	},
	{
		{
			{1, 1, 0, 0},
			{0, 1, 1, 0},
			{0, 0, 0, 0},
			{0, 0, 0, 0},
		},
		{
			{0, 1, 0, 0},
			{1, 1, 0, 0},
			{1, 0, 0, 0},
			{0, 0, 0, 0},
		},
		{
			{1, 1, 0, 0},
			{0, 1, 1, 0},
			{0, 0, 0, 0},
			{0, 0, 0, 0},
		},
		{
			{0, 1, 0, 0},
			{1, 1, 0, 0},
			{1, 0, 0, 0},
			{0, 0, 0, 0},
		},
	},
	{
		{
			{1, 1, 0, 0},
			{1, 1, 0, 0},
			{0, 0, 0, 0},
			{0, 0, 0, 0},
		},
		{
			{1, 1, 0, 0},
			{1, 1, 0, 0},
			{0, 0, 0, 0},
			{0, 0, 0, 0},
		},
		{
			{1, 1, 0, 0},
			{1, 1, 0, 0},
			{0, 0, 0, 0},
			{0, 0, 0, 0},
		},
		{
			{1, 1, 0, 0},
			{1, 1, 0, 0},
			{0, 0, 0, 0},
			{0, 0, 0, 0},
		},
	}
};
pieceArchetype *archetypeArray[PIECE_TYPE_NUMBER] = {&pieceI, &pieceL, &pieceJ, &pieceT, &pieceS, &pieceZ, &pieceQ};
piecetype typeArray[PIECE_TYPE_NUMBER] = {I, L, J, T, S, Z, Q};

void init_variables(void) {
	bgType = 1;
	bgPalette = 8;
	ghostEnabled = 1;
	self = &players[0];
	init_all_players();
	nPlayers = 1;
}

void init_game_variables(void) {
	fallCounter = 0;
	fallThreshold = 60;
	thresholdDecreaseAmt = 5;
	minThreshold = 20;
	canBag = 1;
	nextX = 12;
	nextY = 1;
	baggedX = 12;
	baggedY = 6;
	clearedLines = 0;
	clearStreak = 0;
	clearedTotal = 0;
	clearsForSpeedup = 10;
	if (gameStatus == LoadingMultiPlayer) {
		init_all_players();
		self->target = rand()%nPlayers;
		cycle_next_target();
		alivePlayers = nPlayers - 1;
		winner = 0;
	}
}

void init_archetypes(void) {
	uint8_t n, i, j, k;
	for (n = 0; n < PIECE_TYPE_NUMBER; n++) {
		archetypeArray[n]->type = typeArray[n];
		archetypeArray[n]->typeN = n;
		for (i = 0; i < TETRA_SIZE; i++) {
			for (j = 0; j < TETRA_SIZE; j++) {
				for (k = 0; k < TETRA_SIZE; k++) {
					archetypeArray[n]->shape[i][j][k] = shapeArray[n][i][j][k];
				}
			}
		}
	}
}

void init_piece(piece *piece) {
	piece->x = START_X;
	piece->y = START_Y;
	piece->oldX = START_X;
	piece->oldY = START_Y;
	piece->rotation = 0;
	piece->oldRotation = 0;
	piece->archetype = archetypeArray[rand()%PIECE_TYPE_NUMBER];
}

void init_ghost(piece *ghost, piece *piece) {
	ghost->x = START_X;
	ghost->y = START_Y;
	ghost->oldX = START_X;
	ghost->oldY = START_Y;
	ghost->rotation = piece->rotation;
	ghost->oldRotation = 0;
	ghost->archetype = piece->archetype;
}

void init_playfield(int8_t playfield[PLAYFIELD_Y][PLAYFIELD_X]) {
	uint8_t i, j;
	for (i = 0; i < PLAYFIELD_Y; i++) {
		for (j = 0; j < PLAYFIELD_X; j++) {
			playfield[i][j] = -1;
		}
	}
	return;
}

void init_player(uint8_t i) {
	char str[PLAYER_NAME_LEN];
	
	players[i].id = i;
	init_playfield(players[i].playfield);
		
	sprintf(str, "Player %d", i);
	strcpy(players[i].name, str);
		
	players[i].ko = 0;
	players[i].colorIndex = players[i].id;
	players[i].garbageSize = 0;
	return;
}

void init_all_players(void) {
	uint8_t i;
	
	for (i = 0; i < MAX_PLAYERS; i++) {
		init_player(i);
	}
	return;
}

void save_piece_old(piece *piece) {
	piece->oldX = piece->x;
	piece->oldY = piece->y;
	piece->oldRotation = piece->rotation;
}

void copy_piece(piece *from, piece *to) {
	to->x = from->x;
	to->y = from->y;
	to->oldX = from->oldX;
	to->oldY = from->oldY;
	to->rotation = from->rotation;
	to->oldRotation = from->oldRotation;
	to->archetype = from->archetype;
}

void bag_piece(piece *bagged, piece *p, piece *next) {
	piece temp;
	if (!canBag) return;
	if (!hasBaggedPiece) {
		hasBaggedPiece = 1;
		copy_piece(p, bagged);
		copy_piece(next, p);
	} else {
		copy_piece(bagged, &temp);
		copy_piece(p, bagged);
		copy_piece(&temp, p);
	}
	canBag = 0;
	p->x = START_X;
	p->y = START_Y;
	p->oldX = START_X;
	p->oldY = START_Y;
	bagged->x = baggedX;
	bagged->y = baggedY;
	bagged->oldX = baggedX;
	bagged->oldY = baggedY;
	bagged->rotation = 0;
	bagged->oldRotation = 0;
	return;
}

uint8_t move_piece(piece *piece, direction direction) {
	uint8_t i, j;
	for (i = 0; i < TETRA_SIZE; i++) {
		for (j = 0; j < TETRA_SIZE; j++) {
			if (piece->archetype->shape[piece->rotation][i][j]) {
				switch (direction) {
					case Up:
					break;
					case Down:
						if (piece->y + i + 1 >= PLAYFIELD_Y) return 0;
						if (self->playfield[piece->y + i + 1][piece->x + j] != -1) return 0;
					break;
					case Left:
						if (piece->x + j - 1 < 0) return 0;
						if (self->playfield[piece->y + i][piece->x + j - 1] != -1) return 0;
					break;
					case Right:
						if (piece->x + j + 1 >= PLAYFIELD_X) return 0;
						if (self->playfield[piece->y + i][piece->x + j + 1] != -1) return 0;
					break;
				}
			}
		}
	}
	switch(direction) {
		case Up:
		break;
		case Down:
			piece->y++;
		break;
		case Left:
			piece->x--;
		break;
		case Right:
			piece->x++;
		break;
	}
	return 1;
}

/*uint8_t rotate_piece(piece *piece) {
	uint8_t i, j;
	for (i = 0; i < TETRA_SIZE; i++) {
		for (j = 0; j < TETRA_SIZE; j++) {
			if (piece->archetype->shape[(piece->rotation + 1)%TETRA_SIZE][i][j]) {
				if (piece->x + j >= PLAYFIELD_X) {
					return 0;
				}
				if (playfield[piece->y + i][piece->x + j] != 0) {
					return 0;
				}
			}
		}
	}
	piece->rotation = (piece->rotation + 1)%TETRA_SIZE;	
	return 1;
}*/

uint8_t rotate_piece(piece *piece) {
	uint8_t i, j, out = 0, moveleft = 0;
	for (i = 0; i < TETRA_SIZE && !out; i++) {
		for (j = 0; j < TETRA_SIZE && !out; j++) {
			if (piece->archetype->shape[(piece->rotation + 1)%TETRA_SIZE][i][j]) {
				if (piece->x + j >= PLAYFIELD_X) {
					out = 1;
				}
				if (self->playfield[piece->y + i][piece->x + j] != -1) {
					out = 1;
				}
			}
		}
	}
	if (out) {
		if (move_piece(piece, Left)) {
			moveleft++;
			if (moveleft > 3) return 0;
			if (rotate_piece(piece)) {	
				return 1;
			}
		}
		if (moveleft) {
			move_piece(piece, Right);
		}
		return 0;
	}
	piece->rotation = (piece->rotation + 1)%TETRA_SIZE;	
	return 1;
}

void project_ghost(piece *ghost, piece *piece) {
	ghost->x = piece->x;
	ghost->y = piece->y;
	ghost->rotation = piece->rotation;
	while(move_piece(ghost, Down));
	return;
}

void add_to_playfield(piece *piece, int8_t playfield[PLAYFIELD_Y][PLAYFIELD_X]) {
	int i, j;
	for (i = 0; i < TETRA_SIZE; i++) {
		for (j = 0; j < TETRA_SIZE; j++) {
			if (piece->archetype->shape[piece->rotation][i][j]) {
				playfield[piece->y + i][piece->x + j] = piece->archetype->typeN;
			}
		}
	}
	return;
}

uint8_t check_playfield(int8_t playfield[PLAYFIELD_Y][PLAYFIELD_X], int8_t cleared[TETRA_SIZE]) {
	uint8_t i, j, n = 0;
	uint8_t rowFull;
	for (i = 0; i < PLAYFIELD_Y; i++) {
		rowFull = 1;
		for (j = 0; j < PLAYFIELD_X; j++) {
			if (playfield[i][j] == -1) {
				rowFull = 0;
				break;
			}
		}
		if (rowFull) {
			cleared[n] = i;
			n++;
		}
	}
	for (i = n; i < TETRA_SIZE; i++) {
		cleared[i] = -1;
	}
	return n;
}

void clear_lines(int8_t playfield[PLAYFIELD_Y][PLAYFIELD_X], int8_t cleared[TETRA_SIZE]){
	uint8_t j, n;
	int8_t i;
	for (n = 0; n < TETRA_SIZE; n++) {
		if (cleared[n] == -1) return;
		for (i = cleared[n]; i > 0; i--) {
			for (j = 0; j < PLAYFIELD_X; j++) {
				playfield[i][j] = playfield[i-1][j];
			}
		}			
	}
	return;
}

void add_garbage_to_queue(uint8_t hole, player *p) {
	if (p->garbageSize == MAX_GARBAGE_SIZE) return;
	p->garbageQueue[p->garbageSize] = hole;
	p->garbageSize++;
	return;
}

uint8_t apply_garbage_queue(player *p) {
	uint8_t i, ret = p->garbageSize;
	if (ret == 0) return ret;
	for (i = 0; i < p->garbageSize; i++) {
		apply_garbage_line(p->garbageQueue[i], p);
	}
	p->garbageSize = 0;
	return ret;
}

void apply_garbage_line(uint8_t hole, player *p) {
	uint8_t i, j;
	for (i = 1; i < PLAYFIELD_Y; i++) {
		for (j = 0; j < PLAYFIELD_X; j++) {
			p->playfield[i-1][j] = p->playfield[i][j];
		}
	}
	i = PLAYFIELD_Y - 1;
	for (j = 0; j < PLAYFIELD_X; j++) {
		if (j == hole) {
			p->playfield[i][j] = -1;
		} else {
			p->playfield[i][j] = garbagePalette;
		}
	}
	return;
}

void cycle_next_target(void) {
	if (!frameBlinkingStatus) draw_mini_playfield_frame(&players[self->target]);
	do {
		self->target = (self->target + 1) % nPlayers;
	} while (players[self->target].ko || self->target == self->id);
	return;
}

void decrease_fall_threshold(void) {
	if (fallThreshold > minThreshold) {
		fallThreshold -= thresholdDecreaseAmt;
	}
	return;
}

uint8_t check_cleared_total(void) {
	if (clearedTotal > clearsForSpeedup) {
		clearedTotal = clearedTotal % clearsForSpeedup;
		return 1;
	}
	return 0;
}

uint8_t check_game_over(void) {
	uint8_t i, j;
	for (i = 0; i < TETRA_SIZE; i++) {
		for (j = 0; j < TETRA_SIZE; j++) {
			if (currentPiece.archetype->shape[currentPiece.rotation][i][j] && self->playfield[currentPiece.y + i][currentPiece.x + j] != -1) {
				return 1;
			}
		}
	}
	return 0;
}

void increase_option_value(uint8_t *option, uint8_t max) {
	*option = (*option + 1) % max;
	return;
}

void decrease_option_value(uint8_t *option, uint8_t max) {
	if (*option == 0) {
		*option = max - 1;
	} else {
		*option = (*option - 1) % max;
	}
	return;
}

void send_game_start_to_all(void) {
	CAN_MSG data;
	SCANFrame scanData;
	uint8_t tbody[8] = {0,0,0,0,0,0,0,0};
	SCANcreate(M, &scanData, SCAN_BROADCAST, tbody);
	SCANstructure(&scanData, &data);
	CAN_Send(LPC_CAN1, &data);
	return;
}

void send_piece_to_all(piece *p) {
	CAN_MSG data;
	SCANFrame scanData;
	uint8_t tbody[8] = {0,0,0,0,0,0,0,0};
	tbody[0] = 1;	//Piece placed
	tbody[1] = p->archetype->typeN;
	tbody[2] = p->x; 
	tbody[3] = p->y;
	tbody[4] = p->rotation;
	SCANcreate(M, &scanData, SCAN_BROADCAST, tbody);
	SCANstructure(&scanData, &data);
	CAN_Send(LPC_CAN1, &data);
	return;
}

void send_lines_to_target(uint8_t n, uint8_t target) {
	uint8_t hole, i;
	CAN_MSG data;
	SCANFrame scanData;
	uint8_t tbody[8] = {0,0,0,0,0,0,0,0};
	tbody[0] = 2;	//Lines cleared
	tbody[1] = target;
	tbody[2] = n;
	hole = rand()%PLAYFIELD_X;
	tbody[3] = hole;
	SCANcreate(M, &scanData, SCAN_BROADCAST, tbody);
	SCANstructure(&scanData, &data);
	CAN_Send(LPC_CAN1, &data);
	for (i = 0; i < n; i++) {
		add_garbage_to_queue(hole, &players[target]);
	}
	return;
}

void send_ko_to_all(void) {
	CAN_MSG data;
	SCANFrame scanData;
	uint8_t tbody[8] = {0,0,0,0,0,0,0,0};
	tbody[0] = 3;	//Game over (ko)
	SCANcreate(M, &scanData, SCAN_BROADCAST, tbody);
	SCANstructure(&scanData, &data);
	CAN_Send(LPC_CAN1, &data);
	return;
}

void send_game_end_to_all(void) {
	CAN_MSG data;
	SCANFrame scanData;
	uint8_t tbody[8] = {0,0,0,0,0,0,0,0};
	tbody[0] = 5;	//Game end
	SCANcreate(M, &scanData, SCAN_BROADCAST, tbody);
	SCANstructure(&scanData, &data);
	CAN_Send(LPC_CAN1, &data);
	return;
}

void game_logic(void) {
	uint8_t multi = gameStatus == MultiPlayer;
	uint8_t linesToSend = 0;
	//Save piece from last frame
	save_piece_old(&currentPiece);
	save_piece_old(&ghostPiece);
	
	//Input management
	switch(inputStatus) {
		case KEY1:
			rotate_piece(&currentPiece);
			rotate_piece(&ghostPiece);
		break;
		case KEY2:
			if (!canBag) break; 
			patch_piece(&currentPiece, Current, 1);
			if (hasBaggedPiece) patch_piece(&baggedPiece, Bagged, 1);
			patch_piece(&ghostPiece, Current, 1);
			bag_piece(&baggedPiece, &currentPiece, &nextPiece);
			init_ghost(&ghostPiece, &currentPiece);
			draw_piece(&baggedPiece, Bagged);
		break;
		case INT0:
			if (!multi) {
				gameStatus = OpeningPause;
			} else {
				cycle_next_target();
			}
		break;
		case JLEFT:
			move_piece(&currentPiece, Left);
		break;
		case JRIGHT:
			move_piece(&currentPiece, Right);
		break;
		case JDOWN:
			fallCounter = fallThreshold - 2;
		break;
		case JSELECT:
			while(move_piece(&currentPiece, Down));
			fallCounter = fallThreshold - 2;
		break;
		default:
		break;
	}
	
	inputStatus = NONE;
	
	//Actual game logic
	if (fallCounter >= fallThreshold) {								//Piece has to move down
		if (!move_piece(&currentPiece, Down)) {					//Piece can't move down
			add_to_playfield(&currentPiece, self->playfield);
			if (multi) {
				send_piece_to_all(&currentPiece);					//Notify everyone of placed piece
				if (apply_garbage_queue(self)) draw_playfield(self->playfield);	//Place garbage from other players
			}
			clearedLines = check_playfield(self->playfield, self->cleared);		//Check if some lines are complete
			if (clearedLines) {
				if (multi && clearedLines > 1) {
					linesToSend = clearedLines;
					if (clearStreak) linesToSend++;
					if (clearedLines != 4) linesToSend--;
					send_lines_to_target(linesToSend, self->target);
				}
				clearedTotal += clearedLines;
				if (check_cleared_total()) {							//Check if it's the case to decrease the falling time
					decrease_fall_threshold();
				}
				clearStreak++;						
				clear_lines(self->playfield, self->cleared);		//Actually clear the completed lines
				draw_playfield(self->playfield);
			} else {
				clearStreak = 0;
			}
			copy_piece(&nextPiece, &currentPiece);
			if (check_game_over()) {
				if (!multi) {
					gameStatus = SinglePlayerGameOver;
				} else {
					send_ko_to_all();
					gameStatus = MultiPlayerGameOver;
				}
				return;	//////////
			}
			patch_piece(&nextPiece, Next, 1);
			init_piece(&nextPiece);
			init_ghost(&ghostPiece, &currentPiece);
			draw_piece(&nextPiece, Next);
			canBag = 1;
		}
		fallCounter = 0;
	}
	project_ghost(&ghostPiece, &currentPiece);
	patch_piece(&ghostPiece, Current, 0);
	patch_piece(&currentPiece, Current, 0);
	draw_ghost_piece(&ghostPiece, 1);
	draw_piece(&currentPiece, Current);	
	fallCounter++;
}
