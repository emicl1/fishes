// fish_bitmap.h

#ifndef FISH_BITMAP_H
#define FISH_BITMAP_H

#include <stdint.h>

#define FISH_WIDTH 16  // Adjusted to fit within 16 bits
#define FISH_HEIGHT 10

static const uint16_t fish_bitmap[FISH_HEIGHT] = {
        0x07E0,  // 0000 0111 1110 0000
        0x07E0,  // 0000 0111 1110 0000
        0x0C30,  // 0000 1100 0011 0000
        0x0C30,  // 0000 1100 0011 0000
        0x0FF0,  // 0000 1111 1111 0000
        0x0FF0,  // 0000 1111 1111 0000
        0x0C30,  // 0000 1100 0011 0000
        0x0C30,  // 0000 1100 0011 0000
        0x07E0,  // 0000 0111 1110 0000
        0x07E0   // 0000 0111 1110 0000
};

#endif // FISH_BITMAP_H
