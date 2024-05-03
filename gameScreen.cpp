#define _POSIX_C_SOURCE 200112L

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include <unistd.h>
#include <termios.h>            //termios, TCSANOW, ECHO, ICANON


#include "draw_helper.h"


#define X_VECTOR 5
#define WIDTH_OF_FISH 60
#define HEIGHT_OF_FISH 60

extern unsigned short *fb;

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


int GameScreen() {
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
        x_vector = mid_knob_val / 16;
        y_vector = mid_knob_val / 16;
        if ((r&0x2000000)!=0) {
            x_vector = 0;
            y_vector = 0;
        }

        if (right_knob_val >= 0 && right_knob_val < 16 || right_knob_val >= 64 && right_knob_val < 80 ||
        right_knob_val >= 128 && right_knob_val < 144 ||right_knob_val >= 192 && right_knob_val < 208){
            if (x_vector > 0){
                x_vector *= -1;
            }
            if (y_vector > 0) {
                y_vector *= -1;
            }
        }else if (right_knob_val >= 16 && right_knob_val < 32 || right_knob_val >= 80 && right_knob_val < 96 ||
        right_knob_val >= 144 && right_knob_val < 160 ||right_knob_val >= 208 && right_knob_val < 224) {
            if (x_vector > 0){
                x_vector *= 1;
            }else{
                x_vector *= -1;
            }
            if (y_vector > 0) {
                y_vector *= -1;
            }
        }else if (right_knob_val >= 32 && right_knob_val < 48 || right_knob_val >= 96 && right_knob_val < 112 ||
        right_knob_val >= 160 && right_knob_val < 176 ||right_knob_val >= 224 && right_knob_val < 240) {
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
        }else if (right_knob_val >= 48 && right_knob_val < 64 || right_knob_val >= 112 && right_knob_val < 128 ||
        right_knob_val >= 176 && right_knob_val < 192 ||right_knob_val >= 240 && right_knob_val < 256) {
            if (x_vector > 0){
                x_vector *= -1;
            }
            if (y_vector > 0) {
                y_vector *= +1;
            }else{
                y_vector *= -1;
            }

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
