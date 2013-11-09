/**************************************************************
 * game.h
 * Author: James Dunn
 * Created on: 04 November 2013
 * Description: Library for the game to be implemented in Lab 5
 *************************************************************/
#include <msp430.h>

#ifndef GAME_H_
#define GAME_H_

#define BOARD_WIDTH 8 											// Length of board (char)
#define BOARD_HEIGHT 2 											// Height of board (char)
#define PLAYER	0x2a 											// * character for player
#define MINE 0x58 												// X character for mines
#define BLANK 0x20 												// Space character to make empty board spaces
#define UP 1 													// Move up
#define DOWN 2 													// Move down
#define LEFT 3 													// Move left
#define RIGHT 4 												// Move right
#define BOOM -2 												// Constant for when a mine is hit
#define WINNER -1 												// Constant for when make it to finish
#define FINISH 0x46 											// F character to mark finish
#define RESET_CLOCK 4 											// Number of times clock overflows before timer interrupt triggered
#define OVERFLOW 0 												// Constant used to reset timer


// Struct to define variable of type playing board
typedef struct playingBoard
{
	char boardArray[BOARD_HEIGHT][BOARD_WIDTH+1];
	char boardWidth;
	char boardHeight;
} playingBoard;

/************************************************************************
 * Function Name: initBoard
 * Author: James Dunn
 * Description: Initializes playing board with player, mines, and finish
 * Inputs: None
 * Returns: None
 ***********************************************************************/
playingBoard initBoard();

/************************************************************************
 * Function Name: initPlayer
 * Author: James Dunn
 * Description: Initializes playing board with player at start position
 * Inputs: Pointer to gameboard
 * Returns: None
 ***********************************************************************/
void initPlayer(playingBoard * board);

/************************************************************************
 * Function Name: placeMines
 * Author: James Dunn
 * Description: Randomly places 2 mines on playing board
 * Inputs: Pointer to game board
 * Returns: None
 ***********************************************************************/
void placeMines(playingBoard * board);

/************************************************************************
 * Function Name:  correctMinePlacement
 * Author: James Dunn
 * Description: Checks that mines are placed so game can be won
 * Inputs: Pointer to game board
 * Returns: 1 if mines correctly placed, 0 if not
 ***********************************************************************/
char correctMinePlacement(playingBoard * board);

/************************************************************************
 * Function Name:  movePlayer
 * Author: James Dunn
 * Description: Moves player one position (left, right, up, down)
 * depending on button push
 * Inputs: Pointer to game board, constant that defines direction
 * Returns: Returns int that determines if hit mine, reached end, or
 * standard move
 ***********************************************************************/
signed int movePlayer(playingBoard * gameBoard, unsigned char movementDirection);

/************************************************************************
 * Function Name:  findPlayer_X
 * Author: James Dunn
 * Description: Determines x coordinate of players current position
 * Inputs: Pointer to game board
 * Returns: X coordinate of current position
 ***********************************************************************/
unsigned char findPlayer_X(playingBoard * gameBoard);

/************************************************************************
 * Function Name:  findPlayer_Y
 * Author: James Dunn
 * Description: Determines y coordinate of players current position
 * Inputs: Pointer to game board
 * Returns: Y coordinate of current position
 ***********************************************************************/
unsigned char findPlayer_Y(playingBoard * gameBoard);

/************************************************************************
 * Function Name:  clearBoard
 * Author: James Dunn
 * Description: Clears entire game board
 * Inputs: Pointer to current game board
 * Returns: None
 ***********************************************************************/
void clearBoard(playingBoard * gameBoard);

#endif /* GAME_H_ */
