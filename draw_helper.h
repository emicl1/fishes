/**
 * @file draw_helper.h
 * @brief 
 * A file to help us draw on the screen.
 */

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include <unistd.h>
#include <termios.h>

#include "mzapo_parlcd.h"
#include "mzapo_phys.h"
#include "mzapo_regs.h"

#include "font_types.h"
#include "fish_models.h"

#ifndef FISHES_DRAW_HELPER_H
#define FISHES_DRAW_HELPER_H

/**
 * @brief
 * Makes sure the x coordinate is on the display.
 * 
 * @param x_coord
 * A pointer to the value of the x coordinate
*/
void keep_on_display_x(int* x_coord);

/**
 * @brief
 * Makes sure the y coordinate is on the display.
 * 
 * @param y_coord
 * A pointer to the value of the y coordinate
*/
void keep_on_display_y(int* y_coord);
/**
 * @brief
 * Draws a pixel on the display.
 * 
 * @param x
 * The x coordinate of the pixel
 * @param y
 * The y coordinate of the pixel
 * @param color
 * The color of the pixel
*/
void draw_pixel(int x, int y, unsigned short color);

/**
 * @brief
 * Draws a scaled pixel on the display.
 * 
 * @param x
 * The x coordinate of the pixel
 * @param y
 * The y coordinate of the pixel
 * @param scale
 * The scale of the pixel
 * @param color
 * The color of the pixel
*/
void draw_pixel_scaled(int x, int y, int scale, unsigned short color);

/**
 * @brief
 * A function that gets us the width of the character.
 * 
 * @param fdes
 * A font descriptor that we are using
 * @param ch
 * The character we want to get the width of
 * @returns int
 * The width of the character
*/
int char_width(font_descriptor_t *fdes, int ch);

/**
 * @brief
 * A function for drawing a character
 * 
 * @param x
 * The x coordinate where we want to draw the character
 * @param y
 * The y coordinate where we want to draw the character
 * @param fdes
 * A font descriptor that we are using
 * @param ch
 * The character we want to draw
 * @param color
 * The color that we want the character to have
 * @param text_scale
 * The scale of the character - determines the size
*/
void draw_char(int x, int y, font_descriptor_t *fdes, char ch, uint16_t color, int text_scale);

/**
 * @brief
 * Draws the fish onto the display.
 * 
 * @param x
 * The x coordinate of the fish
 * @param y
 * The y coordinate of the fish
 * @param fdes
 * A fish model descriptor that we are using
 * @param fish_index
 * The index of the fish we want to draw - different numbers draw different fish
 * @param color
 * The color that we want the fish to have
 * @param text_scale
 * The scale of the fish - determines the size
*/
void draw_fish_model(int x, int y, fish_model_descriptor_t *fmdes, int fish_index, uint16_t color, int text_scale);


/**
 * @brief
 * Draws a flipped fish onto the display - a fish that is looking to the left.
 * 
 * @param x
 * The x coordinate of the fish
 * @param y
 * The y coordinate of the fish
 * @param fdes
 * A fish model descriptor that we are using
 * @param fish_index
 * The index of the fish we want to draw - different numbers draw different fish
 * @param color
 * The color that we want the fish to have
 * @param text_scale
 * The scale of the fish - determines the size
*/
void draw_fish_model_flipped(int x, int y, fish_model_descriptor_t *fmdes, int fish_index, uint16_t color, int text_scale);


#endif //FISHES_DRAW_HELPER_H
