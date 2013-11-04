/**************************************************************
 * game.h
 * Author: James Dunn
 * Created on: 04 November 2013
 * Description: Library for the game to be implemented in Lab 5
 *************************************************************/
#include <msp430.h>

#define UP 1
#define DOWN 2
#define LEFT 3
#define RIGHT 4

#define ROW_MASK 0x40

#define NUM_MINES 2

#ifndef GAME_H_
#define GAME_H_

// Initializes player at starting location
unsigned char initPlayer();

// Prints player marker (*) at the player location passed in.
void printPlayer(unsigned char playerLocation);

// Clears the location passed in.
void clearPlayer(unsigned char playerLocation);

// Given a player's current position and a direction, returns an updated player position.
unsigned char movePlayer(unsigned char playerPosition, unsigned char buttonToTest);

// Returns true if the position passed in is the winning game position.
char didPlayerWin(unsigned char playerPosition);

// Functions which may prove useful in achieving A-Functionality.
	/*
	char didPlayerHitMine(unsigned char player, unsigned char mines[NUM_MINES]);
	void generateMines(unsigned char mines[NUM_MINES]);
	void printMines(unsigned char mines[NUM_MINES]);
	*/

#endif /* GAME_H_ */
