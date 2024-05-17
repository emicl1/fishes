/**
 * @file bigger_fish.h
 * @brief 
 * A file with general functions to run the game.
 */
#ifndef FISHES_BIGGER_FISH_H
#define FISHES_BIGGER_FISH_H

/**
 * @brief
 * Draws the main menu onto the display.
 * 
 * @param parlcd_mem_base
 * The base of the lcd display memory
 * @param menuoption
 * A parameter that determines which type of main menu we want to draw
 * 0 for the default mainmenu
 * 1 if we have selected "New game"
 * 2 if we have selected "Exit"
 * @param color
 * The color that we want the selected option in the main menu to have
*/
void redraw_main_menu(unsigned char *parlcd_mem_base, char menuoption, u_int16_t color);

/**
 * @brief
 * Makes the whole screen black.
 * 
 * @param parlcd_mem_base
 * The base of the lcd display memory
*/

void black_screen(unsigned char *parlcd_mem_base);
/**
 * @brief
 * The main function of the whole game.
*/
int main(int argc, char *argv[]);

#endif //FISHES_BIGGER_FISH_H
