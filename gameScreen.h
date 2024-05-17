/**
 * @file gameScreen.h
 * @brief 
 * This file manages the actual screen of the game. 
 */

#ifndef FISHES_GAMESCREEN_H
#define FISHES_GAMESCREEN_H

/**
 * @brief
 * Draws the vicory screen on the display.
 * 
 * @param parlcd_mem_base
 * The base of the lcd display memory
*/
void show_win_screen(unsigned char *parlcd_mem_base);

/**
 * @brief
 * Draws the loss screen on the display.
 * 
 * @param parlcd_mem_base
 * The base of the lcd display memory
*/
void show_lose_screen(unsigned char *parlcd_mem_base);

/**
 * @brief
 * Starts the screen with the game.
 * 
 * @param parlcd_mem_base
 * The base of the lcd display memory
 * @param mem_base
 * The base of the peripherals memory
*/
int GameScreen(unsigned char *parlcd_mem_base, unsigned char *mem_base);

#endif //FISHES_GAMESCREEN_H