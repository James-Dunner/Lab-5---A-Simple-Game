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

void init_timer();
void init_buttons();
void testAndRespondToButtonPush(unsigned char buttonToTest);

void main(void)
{
    WDTCTL = WDTPW | WDTHOLD;							// Stop watchdog timer

    P1DIR |= BIT0;										// Sets SS as GPIO
    configureP1PinAsButton(BIT1|BIT2|BIT3|BIT4);		// Sets PB1-3 as Input

    init_SPI();
	LCD_init();
	LCD_CLR();

	unsigned char player;
	unsigned char gameIsOn, gameOver, gameWon;

	char youWonTopLine[] = "YOU";
	char youWonBottomLine[] = "WON!";
	char gameOverTopLine[] = "GAME";
	char gameOverBottomLine[] = "OVER!";

	player = initPlayer();
	gameIsOn = 1;
	gameOver = 0;
	gameWon = 0;

	//init_timer();
	init_buttons();
	__enable_interrupt();

	printPlayer(player);

	while(1)
	{
		while(gameIsOn)
		{
			/*while(flag <= 4)
			{

			}*/



			gameWon = didPlayerWin(player);
			if (gameWon == 1 || gameOver == 1)
			{
				gameIsOn = 0;
			}
		}

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
	}



	


}

#pragma vector = TIMER0_A1_VECTOR
__interrupt void TIMER0_A1_ISR()
{
    _disable_interrupt();
	TACTL &= ~TAIFG;            // clear interrupt flag


	flag++;
}

#pragma vector = PORT1_VECTOR
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
	P1IE |= BIT1|BIT2|BIT3|BIT4;								// enable the interrupts
    P1IES |= BIT1|BIT2|BIT3|BIT4;                   			// configure interrupt to sense falling edges

    P1REN |= BIT1|BIT2|BIT3|BIT4;                   			// enable internal pull-up/pull-down network
    P1OUT |= BIT1|BIT2|BIT3|BIT4;                   			// configure as pull-up

    P1IFG &= ~(BIT1|BIT2|BIT3|BIT4);                			// clear flags
}

void testAndRespondToButtonPush(char buttonToTest)
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

void debounce()
{
	_delay_cycles(1000);
}


}
