/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           IRQ_RIT.c
** Last modified Date:  2014-09-25
** Last Version:        V1.00
** Descriptions:        functions to manage T0 and T1 interrupts
** Correlated files:    RIT.h
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
#include "lpc17xx.h"
#include "RIT.h"
#include "../led/led.h"
#include "../GLCD/GLCD.h"
#include "../button_EXINT/button.h"
#include "../timer/timer.h"
#include "../TouchPanel/TouchPanel.h"
#include "../game/game.h"
#include "../can/s_can.h"

/******************************************************************************
** Function name:		RIT_IRQHandler
**
** Descriptions:		REPETITIVE INTERRUPT TIMER handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/

extern SCANInfo boardInfo;
unsigned int time = 0;

void RIT_IRQHandler (void)
{
	switch(gameStatus) {
		case Boot:
			time++;
			switch(inputStatus) {
				case NONE:
				break;
				default:
					srand(time);
					init_variables();
					gameStatus = LoadingMainMenu;
				break;
			}
		break;
		case LoadingMainMenu:
			reset_selection(&mainMenu);
			draw_boot_image();
			draw_option_screen(&mainMenu);
			gameStatus = MainMenu;
		break;
		case MainMenu:
			manage_option_input(&mainMenu);
		break;
		case LoadingSinglePlayer:
			LCD_Clear(Black);
		
			init_archetypes();
			init_game_variables();
			init_playfield(self->playfield);
			init_piece(&currentPiece);
			init_piece(&nextPiece);
			init_ghost(&ghostPiece, &currentPiece);
		
			draw_playfield(self->playfield);
			draw_background_next();
			draw_bagged_next();
			draw_piece(&nextPiece, Next);
		
			gameStatus = SinglePlayer;
		break;
		case SinglePlayer:
			game_logic();
		break;
		case SinglePlayerGameOver:
			GUI_ExtraText(20, 20, (uint8_t *)":P", 3, White, Black);
			switch (inputStatus) {
				case KEY2:
				case INT0:
				case JSELECT:
				case KEY1:
					gameStatus = LoadingMainMenu;
				break;
				default:
				break;
			}
		break;
		case OpeningLobby:
			enable_timer(2);
			LCD_Clear(Black);
			if (boardInfo.CANState == Idle) {
				boardInfo.CANState = LookForMaster;
				console_print("Searching for lobbies...");
			} else {
				console_refresh();
			}
			gameStatus = Lobby;
		break;
		case Lobby:
			switch (inputStatus) {
				case JSELECT:
				case KEY1:
					if (boardInfo.CANState == LookForMaster) {
						console_print("Please wait...");
						break;
					}
					if (nPlayers == 1) {
						gameStatus = LoadingSinglePlayer;
						break;
					}
					if (boardInfo.CANState == Master) {
						send_game_start_to_all();
						gameStatus = LoadingMultiPlayer;
					}
				break;
				case INT0:
					gameStatus = LoadingMainMenu;
				break;
				default:
				break;
			}
		break;
		case LoadingMultiPlayer:
			LCD_Clear(Black);

			init_archetypes();
			init_game_variables();
			init_all_players();
			configure_mini_playfields();
			init_piece(&currentPiece);
			init_piece(&nextPiece);
			init_ghost(&ghostPiece, &currentPiece);
		
			draw_playfield_frame(PaletteArray[self->id][PALETTE_SIZE/2]);
			draw_playfield(self->playfield);
			draw_all_mini_playfields();
			draw_background_next();
			draw_bagged_next();
			draw_piece(&nextPiece, Next);

			gameStatus = MultiPlayer;
		break;
		case MultiPlayer:
			game_logic();
		break;
		case MultiPlayerGameOver:
			switch (inputStatus) {
				case KEY2:
				case INT0:
				case JSELECT:
				case KEY1:
					if (winner) {
						send_game_end_to_all();
						gameStatus = OpeningLobby;
					}
				break;
				default:
				break;
			}
		break;
		case OpeningPause:
			LCD_Clear(Black);
			GUI_ExtraText(40, 15, (uint8_t*)"PAUSE", 4, White, Black);
			reset_selection(&pauseMenu);
			draw_option_screen(&pauseMenu);
			gameStatus = Pause;
		break;
		case Pause:
			if (inputStatus == INT0) {
				gameStatus = ClosingPause;
			}
			manage_option_input(&pauseMenu);
		break;
		case ClosingPause:
			LCD_Clear(Black);
			draw_playfield(self->playfield);
			draw_background_next();
			draw_bagged_next();
			draw_piece(&nextPiece, Next);
			gameStatus = SinglePlayer;
		break;
		default:
		break;
	}
	inputStatus = NONE;
	
	LPC_RIT->RICTRL |= 0x1;	/* clear interrupt flag */
}

/******************************************************************************
**                            End Of File
******************************************************************************/
