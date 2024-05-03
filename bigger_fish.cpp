
/*
#define _POSIX_C_SOURCE 200112L

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include <unistd.h>
#include <termios.h>            //termios, TCSANOW, ECHO, ICANON

#include "mzapo_parlcd.h"
#include "mzapo_phys.h"
#include "mzapo_regs.h"


#define X_VECTOR 5
#define WIDTH_OF_FISH 60
#define HEIGHT_OF_FISH 60

unsigned short *fb;

void draw_pixel(int x, int y, unsigned short color) {
    if (x>=0 && x<480 && y>=0 && y<320) {
        fb[x+480*y] = color;
    }
}
void keep_on_display_x(int* x_coord){
    if (*x_coord > 0){
        *x_coord = (*x_coord) % 480;
    }else {
        *x_coord = (*x_coord) + 480;
    }
}

void keep_on_display_y(int* y_coord){
    if (*y_coord > 0){
        *y_coord = (*y_coord) % 320;
    }else {
        *y_coord = (*y_coord) + 320;
    }
}


int main(int argc, char *argv[]) {
    unsigned char *parlcd_mem_base, *mem_base;
    int i,j;
    int ptr;
    unsigned int c;
    fb  = (unsigned short *)malloc(320*480*2);

    printf("Hello world\n");

    parlcd_mem_base = (unsigned char*) map_phys_address(PARLCD_REG_BASE_PHYS, PARLCD_REG_SIZE, 0);
    if (parlcd_mem_base == NULL)
        exit(1);

    mem_base = (unsigned char*) map_phys_address(SPILED_REG_BASE_PHYS, SPILED_REG_SIZE, 0);
    if (mem_base == NULL)
        exit(1);

    parlcd_hx8357_init(parlcd_mem_base);

    parlcd_write_cmd(parlcd_mem_base, 0x2c);
    ptr=0;
    for (i = 0; i < 320 ; i++) {
        for (j = 0; j < 480 ; j++) {
            c = 0;
            fb[ptr]=c;
            parlcd_write_data(parlcd_mem_base, fb[ptr++]);
        }
    }

    struct timespec loop_delay;
    loop_delay.tv_sec = 0;
    loop_delay.tv_nsec = 150 * 1000 * 1000;
    int x_coordinate = 0;
    int y_coordinate = 0;

    int r = *(volatile uint32_t*)(mem_base + SPILED_REG_KNOBS_8BIT_o);
    int x_vector = 5;
    int y_vector = 5;

    while (1) {

        // Exit the loop if you click a button
        if ((r&0x1000000)!=0) {
            break;
        }
        r = *(volatile uint32_t*)(mem_base + SPILED_REG_KNOBS_8BIT_o);
        int right_knob_val = (r&0xff);
        int mid_knob_val = ((r>>8)&0xff);
        //change the speed of the fish
//        printf("mid knob: %d\n", mid_knob_val);
        x_vector = mid_knob_val / 16;
        y_vector = mid_knob_val / 16;
        if ((r&0x2000000)!=0) {
            x_vector = 0;
            y_vector = 0;
        }

//        printf("right knob: %d\n", right_knob_val);

        if (right_knob_val >= 0 && right_knob_val < 16 || right_knob_val >= 64 && right_knob_val < 80 || right_knob_val >= 128 && right_knob_val < 144 ||right_knob_val >= 192 && right_knob_val < 208){
            if (x_vector > 0){
                x_vector *= -1;
            }
            if (y_vector > 0) {
                y_vector *= -1;
            }
//            printf("first quadrant\n");
        }else if (right_knob_val >= 16 && right_knob_val < 32 || right_knob_val >= 80 && right_knob_val < 96 || right_knob_val >= 144 && right_knob_val < 160 ||right_knob_val >= 208 && right_knob_val < 224) {
            if (x_vector > 0){
                x_vector *= 1;
            }else{
                x_vector *= -1;
            }
            if (y_vector > 0) {
                y_vector *= -1;
            }

//            printf("second quadrant\n");

        }else if (right_knob_val >= 32 && right_knob_val < 48 || right_knob_val >= 96 && right_knob_val < 112 || right_knob_val >= 160 && right_knob_val < 176 ||right_knob_val >= 224 && right_knob_val < 240) {
            if (x_vector > 0){
                x_vector *= 1;
            }else{
                x_vector *= -1;
            }
            if (y_vector > 0) {
                y_vector *= +1;
            }else{
                y_vector *= -1;
            }

//            printf("third quadrant\n");
        }else if (right_knob_val >= 48 && right_knob_val < 64 || right_knob_val >= 112 && right_knob_val < 128 || right_knob_val >= 176 && right_knob_val < 192 ||right_knob_val >= 240 && right_knob_val < 256) {
            if (x_vector > 0){
                x_vector *= -1;
            }
            if (y_vector > 0) {
                y_vector *= +1;
            }else{
                y_vector *= -1;
            }
//            printf("fourth quadrant\n");

        }
        x_coordinate += x_vector;
        y_coordinate += y_vector;

        for (ptr = 0; ptr < 320*480 ; ptr++) {
            fb[ptr]=0u;
        }
        for (j=0; j<WIDTH_OF_FISH; j++) {
            for (i=0; i<HEIGHT_OF_FISH; i++) {
                int final_x_coord = i + x_coordinate;
                int final_y_coord = j + y_coordinate;
                keep_on_display_x(&final_x_coord);
                keep_on_display_y(&final_y_coord);
                //printf("x: %d, y: %d\n", final_x_coord, final_y_coord);
                draw_pixel(final_x_coord, final_y_coord, 0x7ff);
            }
        }

        parlcd_write_cmd(parlcd_mem_base, 0x2c);
        for (ptr = 0; ptr < 480*320 ; ptr+=2) {
            uint32_t value_to_write = 0;
            value_to_write |= fb[ptr];
            value_to_write <<= 16;
            value_to_write |= fb[ptr+1];
            parlcd_write_data2x(parlcd_mem_base, value_to_write);
        }

        clock_nanosleep(CLOCK_MONOTONIC, 0, &loop_delay, NULL);
    }

    parlcd_write_cmd(parlcd_mem_base, 0x2c);
    for (ptr = 0; ptr < 480*320 ; ptr+=2) {
        parlcd_write_data2x(parlcd_mem_base, 0);
    }

    printf("Goodbye world\n");

    return 0;
}

*/

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

#define TEXT_SCALE 4



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

void redraw_main_menu(unsigned char *mem_base, unsigned char *parlcd_mem_base, char menuoption, u_int16_t color){
    font_descriptor_t* fdes = &font_winFreeSystem14x16;
    int i,j;
    int ptr;
    unsigned int c;
    fb = (unsigned short *)malloc(320*480*2);
    // sleep(1); this might be needed
    /*
     * Setup memory mapping which provides access to the peripheral
     * registers region of RGB LEDs, knobs and line of yellow LEDs.
     */
    if (menuoption == 0) {
        ptr = 0;
        for (i = 0; i < 320; i++) {
            for (j = 0; j < 480; j++) {
                c = 0;
                fb[ptr] = c;
                parlcd_write_data(parlcd_mem_base, fb[ptr++]);
            }
        }
    }
    u_int16_t defaultcolor = 10000;

    u_int16_t color1 = defaultcolor;
    u_int16_t color2 = defaultcolor;

    // We want to pass 0 to draw the default mainmenu, then we select the desired ones with 1,2
    if (menuoption == 1){
        color1 = color;
    }else if (menuoption == 2){
        color2 = color;
    }
    int x = 100;
    int y = 60;
    char str[] = "Fish Game";
    char *ch = str;
    for (i = 0; i < 10; i++) {
        draw_char(x, y, fdes, *ch, defaultcolor, TEXT_SCALE);
        x += TEXT_SCALE * char_width(fdes, *ch);
        ch++;
    }
    x = 160;
    y = 140;
    char str1[]="New Game";
    char *ch1=str1;
    for (i=0; i<9; i++)
    {
        draw_char(x, y, fdes, *ch1, color1, TEXT_SCALE /2);
        x+= (TEXT_SCALE/2)*char_width(fdes, *ch1);
        ch1++;
    }
    x = 170;
    y = 180;
    char str2[]="Exit";
    char *ch2=str2;
    for (i=0; i<16; i++)
    {
        draw_char(x, y, fdes, *ch2, color2, TEXT_SCALE /2);
        x+= (TEXT_SCALE/2)*char_width(fdes, *ch2);
        ch2++;
    }

    for (ptr = 0; ptr < 480*320 ; ptr+=2)
    {
        uint32_t value_to_write = 0;
        value_to_write |= fb[ptr];
        value_to_write <<= 16;
        value_to_write |= fb[ptr+1];
        parlcd_write_data2x(parlcd_mem_base, value_to_write);
    }
}


void black_screen(unsigned char *parlcd_mem_base){
    int ptr = 0;
    for (int i = 0; i < 320; i++) {
        for (int j = 0; j < 480; j++) {
            int c = 0;
            fb[ptr] = c;
            parlcd_write_data(parlcd_mem_base, fb[ptr++]);
        }
    }
}


int main(int argc, char *argv[])
{
    printf("Hello world\n");
    unsigned char *mem_base;
    unsigned char *parlcd_mem_base;
    mem_base = (unsigned  char *)map_phys_address(SPILED_REG_BASE_PHYS, SPILED_REG_SIZE, 0);
    /* If mapping fails exit with error code */
    if (mem_base == NULL)
        exit(1);
    parlcd_mem_base = (unsigned  char *)map_phys_address(PARLCD_REG_BASE_PHYS, PARLCD_REG_SIZE, 0);
    if (parlcd_mem_base == NULL)
        exit(1);

    parlcd_hx8357_init(parlcd_mem_base);

    parlcd_write_cmd(parlcd_mem_base, 0x2c);

    redraw_main_menu(mem_base, parlcd_mem_base, 0, 10000);
     bool previous = false;
     while (1){
        int r = *(volatile uint32_t*)(mem_base + SPILED_REG_KNOBS_8BIT_o);
        int left_knob_val = ((r>>16)&0xff);

        if ((!previous) && ((r&0x4000000)!=0)){
            black_screen(parlcd_mem_base, fb);
            break;
        }

         if (previous && ((r&0x4000000)!=0)){
             black_screen(parlcd_mem_base, fb);
             break;
         }

        if ((left_knob_val % 32) < 16){
            if (!previous){
                redraw_main_menu(mem_base, parlcd_mem_base,1, 50000);
            }
            previous = true;
        }else if ((left_knob_val % 32) >= 16){
            if (previous){
                redraw_main_menu(mem_base, parlcd_mem_base,2, 50000);
            }
            previous = false;
        }
    }
    printf("Goodbye world\n");
    return 0;
}