/**************************************************************
 * main.c
 * Author: James Dunn
 * Created on: 04 November 2013
 * Description: Main that implements game for Lab 5
 *************************************************************/
#include <msp430.h> 
#include "lcd.h"
#include "buttons/button.h"
#include "game.h"
#include "msp430-rng/rand.h"

char timerCount = 1; // Keeps track of how many times interrupt triggered
signed int gameOn = 1; // Starts the game
playingBoard gameBoard;

void init_timer();
void init_buttons();
char * getTopLineOfBoard(playingBoard * gameBoard);
char * getBottomLineOfBoard(playingBoard * gameBoard);
void clearTimer();

void main(void)
{
    WDTCTL = WDTPW | WDTHOLD;							// Stop watchdog timer

    P1DIR |= BIT0;										// Sets SS as GPIO

    init_SPI();											// Sets up LCD
	LCD_init();
	LCD_CLR();

	char youWonTopLine[] = "YOU";
	char youWonBottomLine[] = "WON!";
	char gameOverTopLine[] = "GAME";
	char gameOverBottomLine[] = "OVER!";
	char boomTopLine[] = "BOOOO";
	char boomBottomLine[] = "OOOM!";

	gameBoard = initBoard();

	init_timer();
	init_buttons();
	__enable_interrupt();

	while(1)
	{
		while(gameOn == 1)
		{
			cursorToLineOne();
			writeString(getTopLineOfBoard(&gameBoard));
			cursorToLineTwo();
			writeString(getBottomLineOfBoard(&gameBoard));
		}

		LCD_CLR();

		if(gameOn <= 0)
		{
			if(!timerCount && gameOn == 0)
			{
				cursorToLineOne();
				writeString(gameOverTopLine);
				cursorToLineTwo();
				writeString(gameOverBottomLine);
			}

			if(gameOn == BOOM)
			{
				cursorToLineOne();
				writeString(boomTopLine);
				cursorToLineTwo();
				writeString(boomBottomLine);
			}

			if(gameOn == WINNER)
			{
				cursorToLineOne();
				writeString(youWonTopLine);
				cursorToLineTwo();
				writeString(youWonBottomLine);
			}
		}

		while(gameOn <= 0)
		{
		}

		gameBoard = initBoard();


	}
}

#pragma vector = TIMER0_A1_VECTOR
__interrupt void TIMER0_A1_ISR()
{
	_disable_interrupt();
	TACTL &= ~TAIFG;            // clears interrupt flag

	if(timerCount <= RESET_CLOCK && gameOn == 1) // Game is on and Clock is still counting
	{
		timerCount++;
	}

	if(timerCount > RESET_CLOCK && gameOn == 1)
	{
		clearBoard(&gameBoard);
		timerCount = OVERFLOW;
		gameOn = 0;
	}

	_enable_interrupt();
}

#pragma vector = PORT1_VECTOR
__interrupt void Port_1_ISR(void)
{
	//_disable_interrupt();

	if(BIT1 & P1IFG)
	{
		if(BIT1 & P1IES)
		{
			gameOn = movePlayer(&gameBoard,UP);
			clearTimer();
		}else
		{
			debounce();
		}

		P1IES ^= BIT1;
		P1IFG &= ~BIT1;
	}

	if(BIT2 & P1IFG)
	{
		if(BIT2 & P1IES)
		{
			gameOn = movePlayer(&gameBoard,DOWN);
			clearTimer();
		}else
		{
			debounce();
		}

		P1IES ^= BIT2;
		P1IFG &= ~BIT2;
	}

	if(BIT3 & P1IFG)
	{
		if(BIT3 & P1IES)
		{
			gameOn = movePlayer(&gameBoard,LEFT);
			clearTimer();
		}else
		{
			debounce();
		}

		P1IES ^= BIT3;
		P1IFG &= ~BIT3;
	}

	if(BIT4 & P1IFG)
	{
		if(BIT4 & P1IES)
		{
			gameOn = movePlayer(&gameBoard,RIGHT);
			clearTimer();
		}else
		{
			debounce();
		}

		P1IES ^= BIT4;
		P1IFG &= ~BIT4;
	}

	//_enable_interrupt();
}

void init_timer()
{
    TACTL &= ~(MC1|MC0);        						// stop timer
    TACTL |= TACLR;             						// clear TAR
    TACTL |= TASSEL1;           						// configure for SMCLK
    TACTL |= ID1|ID0;           						// divide clock by 8
    TACTL |= MC1;               						// set count mode to continuous
    TACTL &= ~TAIFG;            						// clear interrupt flag
    TACTL |= TAIE;              						// enable interrupt
}

void clearTimer()
{
	timerCount = 0;
	TACTL |= TACLR;             						// clear TAR
}

void init_buttons()
{
	configureP1PinAsButton(BIT1|BIT2|BIT3|BIT4);				// Sets P1.0- P1.3 as input

	P1IE |= BIT1|BIT2|BIT3|BIT4;								// enable the interrupts
    P1IES |= BIT1|BIT2|BIT3|BIT4;                   			// configure interrupt to sense falling edges
    P1REN |= BIT1|BIT2|BIT3|BIT4;                   			// enable internal pull-up/pull-down network
    P1OUT |= BIT1|BIT2|BIT3|BIT4;                   			// configure as pull-up
    P1IFG &= ~(BIT1|BIT2|BIT3|BIT4);                			// clear flags
}

char * getTopLineOfBoard(playingBoard * gameBoard)
{
	return gameBoard->boardArray[0];
}

char * getBottomLineOfBoard(playingBoard * gameBoard)
{
	return gameBoard->boardArray[1];
}

