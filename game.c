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
	writeDataByte(0x20);

}

unsigned char movePlayer(unsigned char playerPosition, unsigned char buttonPushed)
{
	switch (buttonPushed)
	{
		case UP:

			if (0x80 <= playerPosition && playerPosition <= 0x87)
			{
				playerPosition -= 0x40;
			}
			else;

		break;

		case DOWN:

			if (0xc0 <= playerPosition && playerPosition <= 0xc7)
			{
				playerPosition += 0x40;
			}

		break;

		case LEFT:

			if (playerPosition != 0xc0 || playerPosition != 0x80)
			{
				playerPosition--;
			}

		break;

		case RIGHT:

			if(playerPosition != 0xc7 || playerPosition != 0x87)
			{
				playerPosition++;
			}

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
