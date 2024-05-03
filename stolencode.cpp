//
// Created by alex on 3.5.24.
//
/*******************************************************************
  Project main function template for MicroZed based MZ_APO board
  designed by Petr Porazil at PiKRON

  change_me.c      - main file

  include your name there and license for distribution.

  Remove next text: This line should not appear in submitted
  work and project name should be change to match real application.
  If this text is there I want 10 points subtracted from final
  evaluation.

 *******************************************************************/

#define _POSIX_C_SOURCE 200112L

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include <unistd.h>

#include "mzapo_parlcd.h"
#include "mzapo_phys.h"
#include "mzapo_regs.h"

#include "font_types.h"
#include "font_prop14x16.cpp"

#define TEXT_SCALE 2


unsigned short *fb;

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

void draw_char(int x, int y, font_descriptor_t *fdes, char ch, uint16_t color)
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
                    draw_pixel_scaled(x + col * TEXT_SCALE, y + row * TEXT_SCALE, TEXT_SCALE, color);
                }
                cur_bit <<= 1;
            }
            char_ptr++;
        }
    }
}



int main(int argc, char *argv[])
{
    font_descriptor_t* fdes = &font_winFreeSystem14x16;
    unsigned char *mem_base;
    unsigned char *parlcd_mem_base;
    int i,j;
    int ptr;
    unsigned int c;
    fb  = (unsigned short *)malloc(320*480*2);

    printf("Hello world\n");

    sleep(1);

    /*
     * Setup memory mapping which provides access to the peripheral
     * registers region of RGB LEDs, knobs and line of yellow LEDs.
     */
    mem_base = (unsigned  char * )map_phys_address(SPILED_REG_BASE_PHYS, SPILED_REG_SIZE, 0);

    /* If mapping fails exit with error code */
    if (mem_base == NULL)
        exit(1);


    parlcd_mem_base = (unsigned  char * )map_phys_address(PARLCD_REG_BASE_PHYS, PARLCD_REG_SIZE, 0);

    if (parlcd_mem_base == NULL)
        exit(1);

    parlcd_hx8357_init(parlcd_mem_base);

    parlcd_write_cmd(parlcd_mem_base, 0x2c);
    ptr=0;
    for (i = 0; i < 320 ; i++)
    {
        for (j = 0; j < 480 ; j++)
        {
            c = 0;
            fb[ptr]=c;
            parlcd_write_data(parlcd_mem_base, fb[ptr++]);
        }
    }

    int x = 10;
    int y = 100;
    char str[]="Goodbye world";
    char *ch=str;
    uint16_t color = 10000;

    for (ptr = 0; ptr < 320*480 ; ptr++)
    {
        fb[ptr]=0u;
    }
    for (i=0; i<13; i++)
    {
        draw_char(x, y, fdes, *ch, color);
        x+= TEXT_SCALE*char_width(fdes, *ch);
        ch++;
    }

    for (ptr = 0; ptr < 480*320 ; ptr++)
    {
        parlcd_write_data(parlcd_mem_base, fb[ptr]);
    }
    printf("Goodbye world\n");

    return 0;
}