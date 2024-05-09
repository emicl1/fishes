#include <iostream>
#include <vector>
#include "draw_helper.h"
#include "Fish.h"



Fish::Fish(int x, int y, int scale, uint16_t color, int fish_index)
        : x(x), y(y), scale(scale), color(color), fish_index(fish_index) {}

void Fish::draw(bool IsFlipped) {
    if (!IsFlipped)
        draw_fish_model(x, y, &fish_models, fish_index, color, scale);
    else
    draw_fish_model_flipped(x, y,&fish_models , fish_index, color, scale);
}
