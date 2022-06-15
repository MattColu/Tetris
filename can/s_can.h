#include "LPC17xx.h"
#include "can_lib.h"
#include "../GLCD/GLCD.h"
#include <stdio.h>

#ifndef __S_CAN_H
#define __S_CAN_H

#define SCAN_MAX_BOARDS 8
#define SCAN_BROADCAST 7
#define SCAN_MASTER 0

typedef enum {
	Idle,
	LookForMaster,
	Master,
	Peer,
	Outcast
}SCANState;

typedef struct {
	uint8_t ID;
	uint8_t MasterID;
	uint8_t FrameNumber;
	SCANState CANState;
	uint8_t UsedFrameNumbers[4];
	//Master only
	uint8_t NextID;
	uint8_t IDIsUsed[SCAN_MAX_BOARDS];
	uint8_t IDNegotiationBuffer[SCAN_MAX_BOARDS];
	uint8_t IDNegotiationCounter;
} SCANInfo;

typedef enum {
	IDGrant,
	IDRequest,
	IDAck,
	M8,
	M16,
	M,
	MPBack2,
	MPBack4
} SCANMsgType;

typedef struct {
	uint8_t dst;
	uint8_t src;
	SCANMsgType type;
	uint8_t frameN;
	uint8_t body[8];
} SCANFrame;

void SCANdestructure(CAN_MSG *msg, SCANFrame *destructd);
void SCANstructure(SCANFrame *constructd, CAN_MSG *msg);
void SCANcreate(SCANMsgType type, SCANFrame *target, uint8_t dest, uint8_t *body);
void SCANMsgMgmt(SCANFrame *msg);
uint8_t SCANBufferFirstFree(void);
uint8_t SCANSearchFrameNumbers(uint8_t fn);

#endif
