//
// Created by alex on 5.5.24.
//

#ifndef FISHES_FISH_H
#define FISHES_FISH_H

using namespace std;
#include <iostream>
#include <string>

#include "draw_helper.h"

class Fish {
public:
    int x, y;            // Position of the fish
    int scale;           // Size of the fish
    uint16_t color;      // Color of the fish
    int fish_index;      // Index to select different fish models if available

    Fish(int x, int y, int scale, uint16_t color, int fish_index);

    void draw(bool IsFlipped = false);

};


#endif //FISHES_FISH_H
