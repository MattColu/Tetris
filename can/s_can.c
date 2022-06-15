#include "s_can.h"

extern SCANInfo boardInfo;
extern uint16_t terminalRow;

SCANMsgType typeTranslate[] = {IDGrant,IDRequest,IDAck,M8,M16,M,MPBack2,MPBack4};
char *typeToString[] = {"IDGrant","IDRequest","IDAck","M8","M16","M","MPBack2","MPBack4"};

void SCANdestructure(CAN_MSG *msg, SCANFrame *destructd) {
	int i;
	if (msg->id_format == 1) return;									//Extended ID is not used
	destructd->dst = (msg->id & 0x700) >> 8;						//0000011100000000
	destructd->src = (msg->id & 0xE0) >> 5;						//0000000011100000
	destructd->type = typeTranslate[(msg->id & 0x1C) >> 2];	//0000000000011100
	destructd->frameN = (msg->id & 0x3);							//0000000000000011
	for (i = 0; i < 8; i++) {
		if (i < 4) {
			destructd->body[i] = msg->dataA[i];
		} else {
			destructd->body[i] = msg->dataB[i%4];
		}
	}
	return;
}

void SCANstructure(SCANFrame *constructd, CAN_MSG *msg) {
	int i;
	msg->id_format = 0;
	msg->id = (constructd->dst << 8) + (constructd->src << 5) + (constructd->type << 2) + (constructd->frameN);
	msg->len = 8;
	msg->msg_type = 0;
	for (i = 0; i < 8; i++) {
		if (i < 4) {
			msg->dataA[i] = constructd->body[i];
		} else {
			msg->dataB[i%4] = constructd->body[i];
		}
	}
	return;
}

void SCANcreate(SCANMsgType type, SCANFrame *target, uint8_t dest, uint8_t *body) {
	int i;
	target->dst = dest;
	target->src = boardInfo.ID;
	target->type = type;
	target->frameN = boardInfo.FrameNumber;
	boardInfo.UsedFrameNumbers[boardInfo.FrameNumber] = 1;
	boardInfo.FrameNumber = (boardInfo.FrameNumber + 1) % 4;
	for (i = 0; i < 8; i++) {
		target->body[i] = body[i];
	}
	return;
}
uint8_t SCANBufferFirstFree() {
	uint8_t i;
	for (i = 1; i < SCAN_MAX_BOARDS; i++) {
		if (boardInfo.IDIsUsed[i] == 0) return i;
	}
	return SCAN_MAX_BOARDS;
}

uint8_t SCANSearchFrameNumbers(uint8_t fn) {
	if (boardInfo.UsedFrameNumbers[fn]) {
		boardInfo.UsedFrameNumbers[fn] = 0;
		return 1;
	}
	return 0;
}

void SCANMsgMgmt(SCANFrame *msg) {
	piece t;
	uint8_t i, src, dest, ncleared, hole;
	char str[26];
	if (msg->type != M) return;
	src = msg->src;
	switch (msg->body[0]) {
		case 0:	//Game start
			gameStatus = LoadingMultiPlayer;
		break;
		case 1:	//Piece placed
			t.archetype = archetypeArray[msg->body[1]];
			t.x = msg->body[2];
			t.y = msg->body[3];
			t.rotation = msg->body[4];
			add_to_playfield(&t, players[src].playfield);
			apply_garbage_queue(&players[src]);
			if (check_playfield(players[src].playfield, players[src].cleared)) {
				clear_lines(players[src].playfield, players[src].cleared);
			}
			draw_mini_playfield(&players[src]);
		break;
		case 2:	//Lines cleared
			dest = msg->body[1];
			ncleared = msg->body[2];
			hole = msg->body[3];
			for (i = 0; i < ncleared; i++) {
				add_garbage_to_queue(hole, &players[dest]);
			}
		break;
		case 3:	//Game over
			ko_mini_playfield(&players[src]);
			alivePlayers--;
			if (alivePlayers == 0) {
				winner = 1;
				gameStatus = MultiPlayerGameOver;
			}
			if (src == self->target) {
				cycle_next_target();
			}
		break;
		case 4:	//Player joined
			if (msg->body[1] == self->id) break;
			nPlayers++;
			sprintf(str, "%s connected", players[msg->body[1]].name);
			console_print(str);
		break;
		case 5:	//Game end
			gameStatus = OpeningLobby;
		break;
	}
	return;
}
