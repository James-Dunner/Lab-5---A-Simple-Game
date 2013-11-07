/*
 * game.c
 *
 *  Created on: Nov 4, 2013
 *      Author: C14James.Dunn
 */
#include <msp430.h>
#include "game.h"
#include "msp430-rng/rand.h"

unsigned int initialSeed = 0x1234; // Initial seed that will be used to gen random number

playingBoard initBoard()
{
	playingBoard board;

	unsigned int i, j;
	unsigned int randomSeed;

	board.boardWidth = BOARD_WIDTH;
	board.boardHeight = BOARD_HEIGHT;

	for(i = 0; i < board.boardHeight; i++) // Initialize each location as blank space
	{
		for(j = 0; j < board.boardWidth; j++)
		{
			board.boardArray[i][j] = BLANK;
		}

		board.boardArray[i][BOARD_WIDTH] = 0x0; // Placed null character at the end of each line so I can print to LCD
	}

	initPlayer(&board);
	randomSeed = prand(initialSeed);

	do	// If generated mines do not meet spec, will regenerate mines
	{
		placeMines(&board, randomSeed);
	} while(!correctMinePlacement(&board));

	return board;
}

void initPlayer(playingBoard * board)
{
	board->boardArray[0][0] = PLAYER; // Place * at start
}

void placeMines(playingBoard * board, unsigned int randomSeed)
{
	char mine_1_X, mine_1_Y, mine_2_X, mine_2_Y;
	unsigned int randomNum;
	int i, j;

	for(i = 0; i < board->boardHeight; i++) // need to clear board in case previous mines were incorrectly placed
	{
		for(j = 0; j < board->boardWidth; j++)
		{
			if(i != 0 && j != 0) // Won't clear * at the start
			{
				board->boardArray[i][j] = BLANK;
			}
		}
	}

	randomNum = prand(randomSeed);
	mine_1_X = (randomNum)%8; // Generates x-coord for first mine

	randomNum = prand(randomNum);
	mine_1_Y = (randomNum)%2; // Generates y-coord for first mine

	randomNum = prand(randomNum);
	mine_2_X = (randomNum)%8; // Generates x-coord for second mine

	randomNum = prand(randomNum);
	mine_2_Y = (randomNum)%2; // Generates y-coord for second mine

	board->boardArray[mine_1_Y][mine_1_X] = MINE;
	board->boardArray[mine_2_Y][mine_2_X] = MINE;
}

char correctMinePlacement(playingBoard * board)
{
	int i, j;

	// Cannot use 0 or 1 because those are potential coordinates of mines
	signed char mine_1_x = -1; // Changed to signed char
	signed char mine_1_y = -1;
	signed char mine_2_x = -1;
	signed char mine_2_y = -1;

	// Will search for both mines and record coordinates so I can test for correct placement
	for(i = 0; i < board->boardHeight; i++)
	{
		for(j = 0; j < board->boardWidth; j++)
		{
			if (board->boardArray[i][j] == MINE)
			{
				if(mine_1_x == -1)
				{
					mine_1_x = j;
					mine_1_y = i;
				}
				else
				{
					mine_2_x = j;
					mine_2_y = i;
				}
			}
		}
	}

	if(mine_1_x == -1 || mine_1_y == -1 || mine_2_x == -1 || mine_2_y == -1) // Checks to make sure two mines were placed
	{
		return 0;
	}

	if((mine_1_x == 0 && mine_1_y == 0) || (mine_2_x == 0 && mine_2_y == 0)) // Checks if mine at start
	{
		return 0;
	}

	if((mine_1_x == 7 && mine_1_y == 1) || (mine_2_x == 7 && mine_2_y == 1)) // Checks if mine at finish
	{
		return 0;
	}

	if((mine_1_y != mine_2_y) && (mine_2_x - mine_1_x < 2)) // Checks to make sure path isn't blocked by mines
	{
		return 0;
	}

	return 1;
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

unsigned char movePlayer(playingBoard * gameBoard, unsigned char movementDirection)
{
	unsigned volatile char playerLocation_X, playerLocation_Y;

	playerLocation_X = findPlayer_X(gameBoard);
	playerLocation_Y = findPlayer_Y(gameBoard);

	switch (movementDirection)
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

unsigned int findPlayer_X(playingBoard * gameBoard)
{
	unsigned int i, j;
	unsigned int xCoordinate;

	for(i = 0; i < gameBoard->boardHeight; i++)
	{
		for(j = 0; j < gameBoard->boardWidth; j++)
		{
			if(gameBoard->boardArray[i][j] == PLAYER)
			{
				xCoordinate = j;
			}
		}
	}

	return xCoordinate;
}

unsigned int findPlayer_Y(playingBoard * gameBoard)
{
	unsigned int i, j;
	unsigned int yCoordinate;

	for(i = 0; i < gameBoard->boardHeight; i++)
	{
		for(j = 0; j < gameBoard->boardWidth; j++)
		{
			if(gameBoard->boardArray[i][j] == PLAYER)
			{
				yCoordinate = i;
			}
		}
	}

	return yCoordinate;
}
