/*
 * game.c
 *
 *  Created on: Nov 4, 2013
 *      Author: C14James.Dunn
 */
#include <msp430.h>
#include "game.h"
#include "lcd.h"

unsigned char initPlayer()
{
	unsigned char player = 0x80;

	return player;
}

void printPlayer(unsigned char playerLocation)
{
	writeCommandByte(playerLocation);
	writeDataByte(0x2a);
}

void clearPlayer(unsigned char playerLocation)
{
	writeCommandByte(playerLocation);

}

unsigned char movePlayer(unsigned char playerPosition, unsigned char buttonToTest)
{
	switch (buttonToTest)
	{
		case UP:



		break;

		case DOWN:

		break;

		case LEFT:

			playerPosition--;

		break;

		case RIGHT:

			playerPosition++;

		break;
	}

	return playerPosition;
}

char didPlayerWin(unsigned char playerPosition)
{
	unsigned char winner = 0;

	if (playerPosition == 0xc7)
	{
		winner = 1;
	}

	return winner;
}
