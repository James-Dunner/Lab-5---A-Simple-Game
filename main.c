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

//char timerCount = 1;
playingBoard gameBoard;

//void init_timer();
//void init_buttons();
char * getTopLineOfBoard(playingBoard * gameBoard);
char * getBottomLineOfBoard(playingBoard * gameBoard);
//void testAndRespondToButtonPush(unsigned char buttonToTest);

void main(void)
{
    WDTCTL = WDTPW | WDTHOLD;							// Stop watchdog timer

    P1DIR |= BIT0;										// Sets SS as GPIO

    init_SPI();											// Sets up LCD
	LCD_init();
	LCD_CLR();

	char volatile gameOn = 1;								// Set it to one so game will continue

	/*char youWonTopLine[] = "YOU";
	char youWonBottomLine[] = "WON!";
	char gameOverTopLine[] = "GAME";
	char gameOverBottomLine[] = "OVER!";*/

	gameBoard = initBoard();
	//init_timer();
	//init_buttons();
	//__enable_interrupt();

	//while(1)
	//{
		while(gameOn)
		{
			cursorToLineOne();
			writeString(getTopLineOfBoard(&gameBoard));
			cursorToLineTwo();
			writeString(getBottomLineOfBoard(&gameBoard));

		}

		/*
		if(gameWon == 1)
		{
			LCD_CLR();
			writeString(youWonTopLine);
			cursorToLineTwo();
			writeString(youWonBottomLine);
		}

		else
		{
			LCD_CLR();
			writeString(gameOverTopLine);
			cursorToLineTwo();
			writeString(gameOverBottomLine);
		}
	}*/



	


}

/*#pragma vector = TIMER0_A1_VECTOR
__interrupt void TIMER0_A1_ISR()
{
    _disable_interrupt();
	TACTL &= ~TAIFG;            // clear interrupt flag


	flag++;
}*/

/*#pragma vector = PORT1_VECTOR
__interrupt void Port_1_ISR(void)
{
	_disable_interrupt();

	testAndRespondToButtonPush();
	testAndRespondToButtonPush();
	testAndRespondToButtonPush();
	testAndRespondToButtonPush();
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

void init_buttons()
{
	configureP1PinAsButton(BIT1|BIT2|BIT3|BIT4);				// Sets P1.0- P1.3 as input

	P1IE |= BIT1|BIT2|BIT3|BIT4;								// enable the interrupts
    P1IES |= BIT1|BIT2|BIT3|BIT4;                   			// configure interrupt to sense falling edges
    P1REN |= BIT1|BIT2|BIT3|BIT4;                   			// enable internal pull-up/pull-down network
    P1OUT |= BIT1|BIT2|BIT3|BIT4;                   			// configure as pull-up
    P1IFG &= ~(BIT1|BIT2|BIT3|BIT4);                			// clear flags
}*/

/*void testAndRespondToButtonPush(char buttonToTest)
{
    if (buttonToTest & P1IFG)
    {
        if (buttonToTest & P1IES)
        {

            clearTimer();
        } else
        {
            debounce();
        }

        P1IES ^= buttonToTest;
        P1IFG &= ~buttonToTest;
    }
*/

char * getTopLineOfBoard(playingBoard * gameBoard)
{
	return gameBoard->boardArray[0];
}

char * getBottomLineOfBoard(playingBoard * gameBoard)
{
	return gameBoard->boardArray[1];
}

/*void debounce()
{
	_delay_cycles(1000);
}*/
