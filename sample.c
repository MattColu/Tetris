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
	init_RIT(0x0016E360);									/* RIT Initialization 30 msec (30 fps)	*/
	//init_RIT(0xCB734);										/* RIT Initialization 30 msec (30 fps)	*/
	//ADC_init();
	
	init_timer(1, 0x001312D0);								/* Timer 1 Initialization 50 msec		*/
	init_timer(2, 0xBEBC20 ); 						  		/* 500ms * 25MHz = 12.5*10^6 = 0xBEBC20 */	
	
	boardInfo.CANState = Idle;
	boardInfo.ID = 0;
	
	CAN_Init(LPC_CAN1, 125000);
	CAN_Init(LPC_CAN2, 125000);
		
	LCD_Initialization();
	
	TP_Init();
	
	draw_boot_image();
	
	gameStatus = Boot;
	inputStatus = NONE;
	
	enable_timer(1);
	enable_RIT();												/* RIT enabled									*/
	
	LPC_SC->PCON |= 0x1;									/* power-down	mode										*/
	LPC_SC->PCON &= ~(0x2);						
		
	
	
  while (1) {                           /* Loop forever                       */	
		__ASM("wfi");
  }

}
