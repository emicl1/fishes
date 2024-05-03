
#define _POSIX_C_SOURCE 200112L


#include <stdint.h>

#include "font_types.h"
#include "font_prop14x16.cpp"

#define TEXT_SCALE 4

unsigned short* fb;

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
            draw_pixel(x + i, y + j, color);
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

void draw_fish_model(int x, int y, font_descriptor_t *fdes, char ch, uint16_t color, int text_scale) {
    int w = char_width(fdes, ch);
    const font_bits_t *char_ptr;
    char_ptr = &fdes->bits[(ch - fdes->firstchar) * fdes->height];
    for (int row = 0; row < fdes->height; row++) {
        font_bits_t cur_bit = *char_ptr;
        for (int col = 0; col < w; col++) {
            if ((cur_bit & 0x8000) != 0) {
                draw_pixel_scaled(x + col * text_scale, y + row * text_scale, text_scale, color);
            }
            cur_bit <<= 1;
        }
        char_ptr++;
    }
}
