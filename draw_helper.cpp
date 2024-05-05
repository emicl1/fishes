
#define _POSIX_C_SOURCE 200112L


#include <stdint.h>
#include <cstdio>

#include "font_types.h"
#include "font_prop14x16.cpp"
#include "fish_bitman.h"
#include "fish_models.h"

#define TEXT_SCALE 4

unsigned short* fb;

void keep_on_display_x(int* x_coord){
    if (*x_coord > 480){
        *x_coord = (*x_coord) - 480;
    }else if (*x_coord < 0){
        *x_coord = (*x_coord) + 480;
    }
}

void keep_on_display_y(int* y_coord){
    if (*y_coord > 320){
        *y_coord = (*y_coord) - 320;
    }else if (*y_coord < 0) {
        *y_coord = (*y_coord)  + 320;
    }
}



void draw_pixel(int x, int y, unsigned short color)
{
    if (x >= 0 && x < 480 && y >= 0 && y < 320)
    {
        fb[x+480*y] = color;
    }
}

void draw_pixel_scaled(int x, int y, int scale, unsigned short color)
{
    for (int i = 0; i < scale; i++)
    {
        for (int j = 0; j < scale; j++)
        {

            int final_x_coord = i + x;
            int final_y_coord = j + y;
            keep_on_display_x(&final_x_coord);
            //printf("before y: %d\n", final_y_coord);
            keep_on_display_y(&final_y_coord);
            //printf("after y: %d\n", final_y_coord);

            draw_pixel(final_x_coord, final_y_coord, color);
       }
    }
}

int char_width(font_descriptor_t *fdes, int ch)
{
    int width;
    if (!fdes->width)
    {
        width = fdes->maxwidth;
    } else
    {
        width = fdes->width[ch - fdes->firstchar];
    }
    return width;
}

void draw_char(int x, int y, font_descriptor_t *fdes, char ch, uint16_t color, int text_scale)
{
    int w = char_width(fdes, ch);
    const font_bits_t *char_ptr;
    if ((ch >= fdes->firstchar) && (ch - fdes->firstchar < fdes->size))
    {
        char_ptr = &fdes->bits[(ch-fdes->firstchar) * fdes->height];
        for (int row = 0; row < fdes->height; row++)
        {
            font_bits_t cur_bit = *char_ptr;
            for (int col = 0; col < w; col++)
            {
                if ((cur_bit & 0x8000) != 0)
                {
                    draw_pixel_scaled(x + col * text_scale, y + row * text_scale, text_scale, color);
                }
                cur_bit <<= 1;
            }
            char_ptr++;
        }
    }
}

void draw_fish_model(int x, int y, fish_model_descriptor_t *fmdes, int fish_index, uint16_t color, int text_scale) {
    int w = fmdes->widths[fish_index];
    const fish_model_bits_t *char_ptr;
    char_ptr = &fmdes->bits[fmdes->offset[fish_index]]; // Adjusted indexing
    for (int row = 0; row < fmdes->heights[fish_index]; row++) {
        fish_model_bits_t cur_bit = *char_ptr;
        for (int col = 0; col < w; col++) {
            if ((cur_bit & 0x80000000) != 0) {
                draw_pixel_scaled(x + col * text_scale, y + row * text_scale, text_scale, color);
            }
            cur_bit <<= 1;
        }
        char_ptr++; // Move to the next row
    }
}

void draw_fish_model_flipped(int x, int y, fish_model_descriptor_t *fmdes, int fish_index, uint16_t color, int text_scale) {
    int w = fmdes->widths[fish_index];
    const fish_model_bits_t *char_ptr;
    char_ptr = &fmdes->bits[fmdes->offset[fish_index]]; // Adjusted indexing
    for (int row = fmdes->heights[fish_index]; row > 0; row--) {
        fish_model_bits_t cur_bit = *char_ptr;
        for (int col = w; col > 0; col--) {
            if ((cur_bit & 0x80000000) != 0) {
                draw_pixel_scaled(x + col * text_scale, y + row * text_scale, text_scale, color);
            }
            cur_bit <<= 1;
        }
        char_ptr++; // Move to the next row
    }
}
