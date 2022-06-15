/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           IRQ_timer.c
** Last modified Date:  2014-09-25
** Last Version:        V1.00
** Descriptions:        functions to manage T0 and T1 interrupts
** Correlated files:    timer.h
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
#include "lpc17xx.h"
#include "timer.h"
#include "../led/led.h"
#include "../GLCD/GLCD.h"
#include "../can/s_can.h"


/******************************************************************************
** Function name:		Timer0_IRQHandler
**
** Descriptions:		Timer/Counter 0 interrupt handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/
#define N 10

/* k=1/f'*f/n  k=f/(f'*n) k=25MHz/(f'*45) */

//const int freqs[8]={4240,3779,3367,3175,2834,2525,2249,2120};
/* 
131Hz		k=4240 C3
147Hz		k=3779
165Hz		k=3367
175Hz		k=3175
196Hz		k=2834		
220Hz		k=2525
247Hz		k=2249
262Hz		k=2120 C4
*/

const int freqs[8]={2120,1890,1684,1592,1417,1263,1125,1062};
/*
262Hz	k=2120		c4
294Hz	k=1890		
330Hz	k=1684		
349Hz	k=1592		
392Hz	k=1417		
440Hz	k=1263		
494Hz	k=1125		
523Hz	k=1062		c5

*/

uint16_t SinTable[45] =                                       /* ÕýÏÒ±í                       */
{
    410, 467, 523, 576, 627, 673, 714, 749, 778,
    799, 813, 819, 817, 807, 789, 764, 732, 694, 
    650, 602, 550, 495, 438, 381, 324, 270, 217,
    169, 125, 87 , 55 , 30 , 12 , 2  , 0  , 6  ,   
    20 , 41 , 70 , 105, 146, 193, 243, 297, 353
};

int seq4beats[3][N]={{0,8,7,8,8,8,8,8,8,8},
										 {0,8,3,8,6,8,5,8,7,8},
										 {0,0,0,8,8,0,0,0,8,8}};
/* contains sequence of notes from c4 to C5 */
/* position of c4 = 0 to C5 - 7 in the freq const vect ADC */ 
/* position 8 is an artifact to include a pause of 1/4 beat */	
	
int play;
/* current status of sound (on/off) */

int blink_mask = 0xFF;

extern int check;

void TIMER0_IRQHandler (void)
{
	static int ticks=0;
	/* DAC management */	
	LPC_DAC->DACR = (SinTable[ticks]/3)<<6;
	ticks++;
	if(ticks==45) ticks=0;
	
  LPC_TIM0->IR = 1;			/* clear interrupt flag */
  return;
}


/******************************************************************************
** Function name:		Timer1_IRQHandler
**
** Descriptions:		Timer/Counter 1 interrupt handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/
int down_INT0=0;
int down_KEY1=0;
int down_KEY2=0;
int down_JOY[5] = {0, 0, 0, 0, 0};

extern status gameStatus;
extern input inputStatus;

void TIMER1_IRQHandler (void)
{
	uint8_t i;
	uint8_t anyJoy = 0;
	/* INT0 */
	if(down_INT0!=0){ 
		if((LPC_GPIO2->FIOPIN & (1<<10)) == 0){	/* INT0 pressed */
			down_INT0++;				
			switch(down_INT0){
				case 2:
					inputStatus = INT0;
				break;
				default:
					inputStatus = NONE;
				break;
			}
		}
		else {	/* button released */
			down_INT0=0;
			inputStatus = NONE;
			NVIC_EnableIRQ(EINT0_IRQn);				/* enable Button interrupts			*/
			LPC_PINCON->PINSEL4    |= (1 << 20);   /* External interrupt 0 pin selection */
		}
	}
	
	/* KEY1 */
	if(down_KEY1!=0){ 
		if((LPC_GPIO2->FIOPIN & (1<<11)) == 0){	/* KEY1 pressed */
			down_KEY1++;				
			switch(down_KEY1){
				case 2:
					inputStatus = KEY1;
				break;
				default:
					inputStatus = NONE;
				break;
			}
		}
		else {	/* button released */
			down_KEY1=0;
			inputStatus = NONE;
			NVIC_EnableIRQ(EINT1_IRQn);				/* enable Button interrupts			*/
			LPC_PINCON->PINSEL4    |= (1 << 22);   /* External interrupt 0 pin selection */
		}
	}
	/* KEY2 */
	if(down_KEY2!=0){ 
		if((LPC_GPIO2->FIOPIN & (1<<12)) == 0){	/* KEY2 pressed */
			down_KEY2++;				
			switch(down_KEY2){
				case 2:
					inputStatus = KEY2;
				break;
				default:
					inputStatus = NONE;
				break;
			}
		}
		else {	/* button released */
			down_KEY2=0;
			inputStatus = NONE;
			NVIC_EnableIRQ(EINT2_IRQn);							 /* enable Button interrupts			*/
			LPC_PINCON->PINSEL4    |= (1 << 24);     /* External interrupt 0 pin selection */
		}
	}

	/* joystick management */
	for (i = 0; i < 5; i++) {
		if ((LPC_GPIO1->FIOPIN & (1<<(25 + i))) == 0) {
			anyJoy = 1;
			down_JOY[i]++;
			if (down_JOY[i] == 2 || down_JOY[i] > JOY_HOLD_TIME) {
				switch (i){
					case 0:
						inputStatus = JSELECT;
					break;
					case 1:
						inputStatus = JDOWN;
					break;
					case 2:
						inputStatus = JLEFT;
					break;
					case 3:
						inputStatus = JRIGHT;
					break;
					case 4:
						inputStatus = JUP;
					break;
					default:
					break;
				}
			} else {
				inputStatus = NONE;
			}
		}
	}
	if (!anyJoy) {
		for (i = 0; i < 5; i++) {
			down_JOY[i] = 0;
		}
	}

	LPC_TIM1->IR = 1;			/* clear interrupt flag */
	return;
}

/*int song=0;
int note=0;
void TIMER2_IRQHandler (void)
{  	
	if(note==N){ 
		note=0;
		song++;
		if(song!=3){
			disable_timer(1);
			disable_timer(0);
		}
		else{
			song--;
		}
	}
	else
		play=seq4beats[song][note++];
	
	if(play!=8){
		disable_timer(0);
		reset_timer(0);
		init_timer(0,freqs[play]);
		enable_timer(0);
	}
	else{
		disable_timer(0);
	}
	
	LPC_TIM2->IR = 1;	
  return;
}*/

extern SCANInfo boardInfo;
uint8_t retries = 0;
uint8_t maxRetries = 5;
CAN_MSG data;
SCANFrame scanData; 

void TIMER2_IRQHandler (void) {
	uint8_t i = 0;
	uint8_t tbody[8] = {0,0,0,0,0,0,0,0};
	char str[15];
	switch (boardInfo.CANState){
		case LookForMaster:
			if (retries < maxRetries) {
				//sprintf(str,"%d/%d",retries+1, maxRetries);
				//GUI_Terminal(0, &terminalRow, (uint8_t *)str, White, Black);
				SCANcreate(IDRequest, &scanData, SCAN_MASTER, 0);
				SCANstructure(&scanData, &data);
				CAN_Send(LPC_CAN1, &data);
				retries++;
			} else {
				//GUI_Terminal(0, &terminalRow, (uint8_t *)"Master not found", White, Black);
				//GUI_Terminal(0, &terminalRow, (uint8_t *)"Becoming master", White, Black);
				console_print("Lobby created!");
				
				boardInfo.ID = SCAN_MASTER;
				boardInfo.CANState = Master;
				boardInfo.NextID = 1;
				for (i = 0; i < SCAN_MAX_BOARDS; i++) {
					boardInfo.IDNegotiationBuffer[i] = 0;
				}
				boardInfo.IDNegotiationCounter = 0;
				reset_timer(2);
				disable_timer(2);
				break;
			}
		break;
		case Master:
			if (boardInfo.IDNegotiationCounter == 0) {
				disable_timer(2);
				break;
			} else {
				for (i = 1; i < SCAN_MAX_BOARDS; i++) {
					if (boardInfo.IDNegotiationBuffer[i] > 0) {		//ID is being assigned
						if (boardInfo.IDNegotiationBuffer[i] < maxRetries) {
							tbody[0] = i;
							SCANcreate(IDGrant, &scanData, SCAN_BROADCAST, tbody);
							SCANstructure(&scanData, &data);
							CAN_Send(LPC_CAN1, &data);
							boardInfo.IDNegotiationBuffer[i]++;
							sprintf(str, "Trying to assign ID %d", i);
						} else {													//Retries exceeded, assignment not successful -> free ID
							boardInfo.IDNegotiationBuffer[i] = 0;
							boardInfo.IDNegotiationCounter--;
							sprintf(str, "Dropping ID %d: no response", i);
						}
						//GUI_Terminal(0, &terminalRow, (uint8_t *)str, White, Black);
					}
				}
			}
		break;
		default:
			disable_timer(2);
		break;
	}
  LPC_TIM2->IR = 1;			/* clear interrupt flag */
  return;
}


void TIMER3_IRQHandler (void)
{
	/*
	check=2;
	song=2;
	note=0;
	enable_timer(1);	*/
	LPC_TIM3->IR = 1;			/* clear interrupt flag */
  return;
}


/******************************************************************************
**                            End Of File
******************************************************************************/
