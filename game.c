/*
 * game.c
 *
 *  Created on: Nov 4, 2013
 *      Author: C14James.Dunn
 */
#include <msp430.h>
#include "game.h"
#include "msp430-rng/rand.h"

unsigned int random = 2207; // Initial seed that will be used to gen random number

playingBoard initBoard()
{
	playingBoard board;

	unsigned int i, j;

	board.boardWidth = BOARD_WIDTH;
	board.boardHeight = BOARD_HEIGHT;

	for(i = 0; i < board.boardHeight; i++) // Initialize each location as blank space
	{
		for(j = 0; j < board.boardWidth; j++)
		{
			if(i == 1 && j == 7)
			{
				board.boardArray[i][j] = 0x46;
			}
			board.boardArray[i][j] = BLANK;
		}

		board.boardArray[i][BOARD_WIDTH] = 0x0; // Placed null character at the end of each line so I can print to LCD
		//board.boardArray[1][7] = 0x46; // Marks end of the game
	}

	initPlayer(&board);

	do	// If generated mines do not meet spec, will regenerate mines
	{
		random = prand(random);
		placeMines(&board);
	} while(!correctMinePlacement(&board));

	return board;
}

void initPlayer(playingBoard * board)
{
	board->boardArray[0][0] = PLAYER; // Place * at start
}

void placeMines(playingBoard * board)
{
	char mine_1_X, mine_1_Y, mine_2_X, mine_2_Y;
	unsigned int i, j;

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

	board->boardArray[1][7] = 0x46;

	random = prand(random);
	mine_1_X = (random)%8; // Generates x-coord for first mine

	random = prand(random);
	mine_1_Y = (random)%2; // Generates y-coord for first mine

	random = prand(random);
	mine_2_X = (random)%8; // Generates x-coord for second mine

	random = prand(random);
	mine_2_Y = (random)%2; // Generates y-coord for second mine

	board->boardArray[mine_1_Y][mine_1_X] = MINE;
	board->boardArray[mine_2_Y][mine_2_X] = MINE;
}

char correctMinePlacement(playingBoard * board)
{
	unsigned int i, j;

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

void clearBoard(playingBoard * gameBoard)
{
	unsigned int i, j;

	for(i = 0; i < gameBoard->boardWidth; i++)
	{
		for(j = 0; j < gameBoard->boardHeight; j++)
		{
			gameBoard->boardArray[j][i] = BLANK;
		}
	}
}

signed int movePlayer(playingBoard * gameBoard, unsigned char movementDirection)
{
	unsigned volatile char playerLocation_X, playerLocation_Y;

	playerLocation_X = findPlayer_X(gameBoard);
	playerLocation_Y = findPlayer_Y(gameBoard);

	// Executes the movement
	switch (movementDirection)
	{
		case UP:

			if (playerLocation_Y != 0)
			{
				playerLocation_Y--;
			}

		break;

		case DOWN:

			if (playerLocation_Y != 1)
			{
				playerLocation_Y ++;
			}

		break;

		case LEFT:

			if (playerLocation_X != 0)
			{
				playerLocation_X--;
			}

		break;

		case RIGHT:

			if(playerLocation_X != 7)
			{
				playerLocation_X++;
			}

		break;
	}

	// Tests if player hit mine
	if(gameBoard->boardArray[playerLocation_Y][playerLocation_X] == MINE)
	{
		//clearBoard(gameBoard);
		return BOOM;
	}

	// Tests if player made it to finish
	if(gameBoard->boardArray[playerLocation_Y][playerLocation_X] == FINISH) // Adding mark on game board to indicate finish
	{
		//clearBoard(gameBoard);
		return WINNER;
	}

	// If player neither wins nor loses, just move the * and remove from previous location
	gameBoard->boardArray[findPlayer_Y(gameBoard)][findPlayer_X(gameBoard)] = BLANK;
	gameBoard->boardArray[playerLocation_Y][playerLocation_X] = PLAYER;

	return 1;
}

unsigned char findPlayer_X(playingBoard * gameBoard)
{
	unsigned int i, j;
	unsigned char xCoordinate;

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

unsigned char findPlayer_Y(playingBoard * gameBoard)
{
	unsigned int i, j;
	unsigned char yCoordinate;

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
