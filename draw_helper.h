//
// Created by alex on 3.5.24.
//

//#define _POSIX_C_SOURCE 200112L

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
//#include "font_prop14x16.cpp"

#ifndef FISHES_DRAW_HELPER_H
#define FISHES_DRAW_HELPER_H

void draw_pixel(int x, int y, unsigned short color);

void draw_pixel_scaled(int x, int y, int scale, unsigned short color);

int char_width(font_descriptor_t *fdes, int ch);

void draw_char(int x, int y, font_descriptor_t *fdes, char ch, uint16_t color, int text_scale);

void draw_fish_model(int x, int y, fish_model_descriptor_t *fmdes, int fish_index, uint16_t color, int text_scale);

void draw_fish(int x, int y, uint16_t color);

void draw_fish_model_flipped(int x, int y, fish_model_descriptor_t *fmdes, int fish_index, uint16_t color, int text_scale);


#endif //FISHES_DRAW_HELPER_H
