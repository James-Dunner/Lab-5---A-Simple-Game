/*
 * game.c
 *
 *  Created on: Nov 4, 2013
 *      Author: C14James.Dunn
 */
#include <msp430.h>
#include "game.h"
#include "msp430-rng/rand.h"

playingBoard initBoard()
{
	playingBoard board;

	unsigned int i, j;
	unsigned int randomSeed;

	board.boardWidth = BOARD_WIDTH;
	board.boardHeight = BOARD_HEIGHT;

	for(i = 0; i < board.boardWidth; i++)
	{
		for(j = 0; j < board.boardHeight; j++)
		{
			board.boardArray[j][i] = BLANK;
		}
	}

	initPlayer(&board);
	randomSeed = rand();
	placeMines(&board, randomSeed);

	return board;
}

void placeMines(playingBoard * board, unsigned int randNum)
{

}

void clearBoard(playingBoard board)
{
	int i, j;

	for(i = 0; i < board.boardWidth; i++)
	{
		for(j = 0; j < board.boardHeight; j++)
		{
			board.boardArray[j][i] = BLANK;
		}
	}
}

void initPlayer(playingBoard * board)
{
	board->boardArray[0][0] = PLAYER;
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
