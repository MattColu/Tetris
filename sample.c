/*----------------------------------------------------------------------------
 * Name:    sample.c
 * Purpose: to control led through EINT and joystick buttons
 * Note(s):
 *----------------------------------------------------------------------------
 *
 * This software is supplied "AS IS" without warranties of any kind.
 *
 * Copyright (c) 2017 Politecnico di Torino. All rights reserved.
 *----------------------------------------------------------------------------*/
                  
#include <stdio.h>
#include "LPC17xx.H"                    /* LPC17xx definitions                */
#include "led/led.h"
#include "button_EXINT/button.h"
#include "timer/timer.h"
#include "RIT/RIT.h"
#include "joystick/joystick.h"
#include "adc/adc.h"
#include "GLCD/GLCD.h"
#include "TouchPanel/TouchPanel.h"
#include "game/game.h"
#include "game/gamegfx.h"
#include "game/music.h"
#include "can/s_can.h"

/*----------------------------------------------------------------------------
  Main Program
 *----------------------------------------------------------------------------*/

SCANInfo boardInfo;

int main (void) {
	
	SystemInit();  											/* System Initialization (i.e., PLL)	*/
	LED_init();                           				/* LED Initialization               	*/
	BUTTON_init();
	joystick_init();											/* Joystick Initialization          	*/
	init_RIT(0x0016E360);									/* RIT Initialization 60 msec				*/
	//init_RIT(0xCB734);										/* RIT Initialization 33 msec				*/
	
	init_timer(1, 0x001312D0);								/* Timer 1 Initialization 50 msec		*/
	init_timer(2, 0x00BEBC20); 						  	/* Timer 2 Initialization 500 msec		*/	
	
	boardInfo.CANState = Idle;
	boardInfo.ID = 0;
	
	CAN_Init(LPC_CAN1, 125000);
	CAN_Init(LPC_CAN2, 125000);
		
	LCD_Initialization();
	
	draw_boot_image();
	
	gameStatus = Boot;
	inputStatus = NONE;
	
	enable_timer(1);
	enable_RIT();												/* RIT enabled									*/
	
	LPC_SC->PCON |= 0x1;									/* power-down	mode										*/
	LPC_SC->PCON &= ~(0x2);						
	
	LPC_PINCON->PINSEL1 |= (1<<21);
	LPC_PINCON->PINSEL1 &= ~(1<<20);
	LPC_GPIO0->FIODIR |= (1<<26);
	
  while (1) {                           /* Loop forever                       */	
		__ASM("wfi");
  }

}
