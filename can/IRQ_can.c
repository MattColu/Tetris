
#include <stdio.h>
#include "can_lib.h"
#include "s_can.h"
#include "lpc17xx.h"
#include "../GLCD/GLCD.h" 
#include "../RIT/RIT.h"
#include "../timer/timer.h"
#include "../game/game.h"

extern SCANInfo boardInfo;
extern uint16_t terminalRow;

void CAN_IRQHandler(){
	char str[CONSOLE_STRING_LEN];
	CAN_MSG data;
	SCANFrame scanData;
	uint8_t tbody[8] = {0,0,0,0,0,0,0,0};
	if (CAN_HasReceivedMessage(LPC_CAN2)) {
		CAN_Receive(LPC_CAN2, &data);
		SCANdestructure(&data, &scanData);
		switch (boardInfo.CANState) {
			case Idle:
				CAN_Send(LPC_CAN1, &data);
			break;
			case Outcast:
				CAN_Send(LPC_CAN1, &data);
			break;
			case LookForMaster:
				if (scanData.type == IDGrant) {
					boardInfo.ID = scanData.body[0]; 		//?????
					if (boardInfo.ID == SCAN_BROADCAST) {
						boardInfo.CANState = Outcast;
					} else {
						boardInfo.CANState = Peer;
					}
					SCANcreate(IDAck, &scanData, SCAN_MASTER, tbody);
					SCANstructure(&scanData, &data);
					CAN_Send(LPC_CAN1, &data);									//Send IDAck to master
					
					if (boardInfo.CANState == Outcast) {
						console_print("Connected as spectator");
						nPlayers = boardInfo.ID - 1;
						disable_timer(2);
						return;
					}
					
					if (boardInfo.ID != 0) {
						self = &players[boardInfo.ID];
						nPlayers = self->id + 1;
					}
					
					sprintf(str, "Connected as %s", self->name);
					console_print(str);
					
					disable_timer(2);
					return;
				} else if (scanData.src == boardInfo.ID && SCANSearchFrameNumbers(scanData.frameN)) {
					return;
				} else {
					CAN_Send(LPC_CAN1, &data);
				}
			break;
			case Master:
				if (scanData.src == boardInfo.ID && SCANSearchFrameNumbers(scanData.frameN)) return;
				switch(scanData.type) {
					case IDRequest:
						boardInfo.NextID = SCANBufferFirstFree();
						tbody[0] = boardInfo.NextID;
						if (boardInfo.NextID == SCAN_BROADCAST) {
							console_print("Maximum players exceeded");
						}
						boardInfo.IDNegotiationBuffer[boardInfo.NextID] = 1;
						boardInfo.IDNegotiationCounter++;
						SCANcreate(IDGrant, &scanData, SCAN_BROADCAST, tbody);
						SCANstructure(&scanData, &data);
						CAN_Send(LPC_CAN1, &data);
						reset_timer(2);
						enable_timer(2);
					break;
					case IDAck:										//Assignment successful
						if (boardInfo.IDNegotiationBuffer[scanData.src] > 0) {
							boardInfo.IDNegotiationBuffer[scanData.src] = 0;
							if (scanData.src != SCAN_BROADCAST) boardInfo.IDIsUsed[scanData.src] = 1; //Set used only if valid, SCAN_BROADCAST is always unused
							boardInfo.IDNegotiationCounter--;
						}
						if (scanData.src == MAX_PLAYERS) break;
						sprintf(str, "%s connected", players[scanData.src].name);
						tbody[0] = 4;
						tbody[1] = nPlayers;
						nPlayers++;
						SCANcreate(M, &scanData, SCAN_BROADCAST, tbody);
						SCANstructure(&scanData, &data);
						CAN_Send(LPC_CAN1, &data);
						console_print(str);
					break;
					case IDGrant:									//Shouldn't happen, will pretend i didn't see anything
					break;
					default:
						if (scanData.dst == SCAN_BROADCAST) CAN_Send(LPC_CAN1, &data);
						SCANMsgMgmt(&scanData);
					break;
				}
			break;
			case Peer:
				if (scanData.src == boardInfo.ID && SCANSearchFrameNumbers(scanData.frameN)) return;
				if (scanData.dst == boardInfo.ID || scanData.dst == SCAN_BROADCAST) {
					if (scanData.dst == SCAN_BROADCAST) CAN_Send(LPC_CAN1, &data);
					SCANMsgMgmt(&scanData);
				} else {
					CAN_Send(LPC_CAN1, &data);
				}
			break;
			
			default:
			break;
		}
	}
	return;
}
