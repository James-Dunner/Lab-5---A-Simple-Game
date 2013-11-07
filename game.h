/**************************************************************
 * game.h
 * Author: James Dunn
 * Created on: 04 November 2013
 * Description: Library for the game to be implemented in Lab 5
 *************************************************************/
#include <msp430.h>

#ifndef GAME_H_
#define GAME_H_

#define BOARD_WIDTH 8 // Length of board (char)
#define BOARD_HEIGHT 2 // Height of board (char)
#define PLAYER	0x2a // * character for player
#define MINE 0x58 // X for mines
#define BLANK 0x20 // Space character
#define UP 1
#define DOWN 2
#define LEFT 3
#define RIGHT 4

typedef struct playingBoard
{
	char boardArray[BOARD_HEIGHT][BOARD_WIDTH+1];
	char boardWidth;
	char boardHeight;
} playingBoard;

// Initializes board
playingBoard initBoard();

// Initializes player at starting location
void initPlayer(playingBoard * board);

// Places mines on board
void placeMines(playingBoard * board, unsigned int randomSeed);

// Checks that mines were correctly placed on board
char correctMinePlacement(playingBoard * board);

// Given a player's current position and a direction, returns an updated player position.
unsigned char movePlayer(playingBoard * gameBoard, unsigned char movementDirection);

// Locates Player X-Coord
unsigned int findPlayer_X(playingBoard * gameBoard);

// Locates Player Y-Coord
unsigned int findPlayer_Y(playingBoard * gameBoard);

// Returns true if the position passed in is the winning game position.
char didPlayerWin(unsigned char playerPosition);

// Functions which may prove useful in achieving A-Functionality.
	/*
	char didPlayerHitMine(unsigned char player, unsigned char mines[NUM_MINES]);
	void generateMines(unsigned char mines[NUM_MINES]);
	void printMines(unsigned char mines[NUM_MINES]);
	*/

#endif /* GAME_H_ */
