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

char timerCount = 1; 																// Keeps track of how many times interrupt triggered
signed int gameOn = 1; 																// Starts the game
playingBoard gameBoard;

void init_timer();
void init_buttons();
char * getTopLineOfBoard(playingBoard * gameBoard);
char * getBottomLineOfBoard(playingBoard * gameBoard);
void clearTimer();

void main(void)
{
    WDTCTL = WDTPW | WDTHOLD;														// Stop watchdog timer

    P1DIR |= BIT0;																	// Sets SS as GPIO

    init_SPI();																		// Sets up LCD
	LCD_init();
	LCD_CLR();

	char youWonTopLine[] = "YOU";
	char youWonBottomLine[] = "WON!";
	char gameOverTopLine[] = "GAME";
	char gameOverBottomLine[] = "OVER!";
	char boomTopLine[] = "BOOOO";
	char boomBottomLine[] = "OOOM!";

	gameBoard = initBoard();														// Sets up playing board

	init_timer();																	// Enables interrupts
	init_buttons();
	__enable_interrupt();

	// Game will run continuously
	while(1)
	{
		while(gameOn == 1)															// As long as game is still going
		{
			// Prints game board
			cursorToLineOne();
			writeString(getTopLineOfBoard(&gameBoard));
			cursorToLineTwo();
			writeString(getBottomLineOfBoard(&gameBoard));
		}

		// Once it's game over board is clear to print messages
		LCD_CLR();

		if(gameOn <= 0)
		{
			if(!timerCount && gameOn == 0)											// Timer is reset means ran out of time move
			{
				cursorToLineOne();													// Will print game over
				writeString(gameOverTopLine);
				cursorToLineTwo();
				writeString(gameOverBottomLine);
			}

			if(gameOn == BOOM)														// gameOn == BOOM will print boom since hit mine
			{
				cursorToLineOne();
				writeString(boomTopLine);
				cursorToLineTwo();
				writeString(boomBottomLine);
			}

			if(gameOn == WINNER)													// Means made it to finish so will print you won
			{
				cursorToLineOne();
				writeString(youWonTopLine);
				cursorToLineTwo();
				writeString(youWonBottomLine);
			}
		}

		while(gameOn <= 0)															// Resets game and reinitializes random gameboard
		{
		}

		gameBoard = initBoard();


	}
}

// Timer interrupt
#pragma vector = TIMER0_A1_VECTOR
__interrupt void TIMER0_A1_ISR()
{
	_disable_interrupt();															// Disables interrupts so won't trigger while in ISR
	TACTL &= ~TAIFG;            													// clears interrupt flag

	if(timerCount <= RESET_CLOCK && gameOn == 1) 									// Game is on and Clock is still counting
	{
		timerCount++;
	}

	if(timerCount > RESET_CLOCK && gameOn == 1)										// Clock has overflowed 4 times means 2 seconds exceed and game over
	{
		clearBoard(&gameBoard);
		timerCount = OVERFLOW;
		gameOn = 0;
	}

	_enable_interrupt();															// re-enables interrupts
}

// Button interrupt
#pragma vector = PORT1_VECTOR
__interrupt void Port_1_ISR(void)
{
	if(BIT1 & P1IFG)																// If flag is set
	{
		if(BIT1 & P1IES)															// Falling edge
		{
			gameOn = movePlayer(&gameBoard,UP);										// Will move player up since BIT1 is up button
			clearTimer();															// Resets timer so game can continue
		}else
		{
			debounce();
		}

		P1IES ^= BIT1;
		P1IFG &= ~BIT1;																// Clear
	}

	// Repeats same process for BIT1 but controls BIT2 for down movement
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

	// Repeats process for BIT3 which controls left movement
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

	// Repeats process for BIT4 which controls right movement
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
}

/************************************************************************
 * Function Name: init_timer
 * Author: James Dunn
 * Description: Sets correct registers to set up timer
 * Inputs: None
 * Returns: None
 ***********************************************************************/
void init_timer()
{
    TACTL &= ~(MC1|MC0);        													// stop timer
    TACTL |= TACLR;             													// clear TAR
    TACTL |= TASSEL1;           													// configure for SMCLK
    TACTL |= ID1|ID0;           													// divide clock by 8
    TACTL |= MC1;               													// set count mode to continuous
    TACTL &= ~TAIFG;            													// clear interrupt flag
    TACTL |= TAIE;              													// enable interrupt
}

/************************************************************************
 * Function Name: clearTimer
 * Author: James Dunn
 * Description: Resets timer so game can continue
 * Inputs: None
 * Returns: None
 ***********************************************************************/
void clearTimer()
{
	timerCount = 0;
	TACTL |= TACLR;             													// clear TAR
}

/************************************************************************
 * Function Name: init_buttons
 * Author: James Dunn
 * Description: Initializes buttons on geek box and configures P1 for interrupts
 * Inputs: None
 * Returns: None
 ***********************************************************************/
void init_buttons()
{
	configureP1PinAsButton(BIT1|BIT2|BIT3|BIT4);									// Sets P1.0- P1.3 as input

	P1IE |= BIT1|BIT2|BIT3|BIT4;													// enable the interrupts
    P1IES |= BIT1|BIT2|BIT3|BIT4;                   								// configure interrupt to sense falling edges
    P1REN |= BIT1|BIT2|BIT3|BIT4;                   								// enable internal pull-up/pull-down network
    P1OUT |= BIT1|BIT2|BIT3|BIT4;                   								// configure as pull-up
    P1IFG &= ~(BIT1|BIT2|BIT3|BIT4);                								// clear flags
}

/************************************************************************
 * Function Name: getTopLineOfBoard
 * Author: James Dunn
 * Description: Acquires top line of game board to print
 * Inputs: Point to game board
 * Returns: pointer to top line of board
 ***********************************************************************/
char * getTopLineOfBoard(playingBoard * gameBoard)
{
	return gameBoard->boardArray[0];
}

/************************************************************************
 * Function Name: getBottomLineOfBoard
 * Author: James Dunn
 * Description: Acquires bottom line of game board to print
 * Inputs: Point to game board
 * Returns: pointer to bottom line of board
 ***********************************************************************/
char * getBottomLineOfBoard(playingBoard * gameBoard)
{
	return gameBoard->boardArray[1];
}

