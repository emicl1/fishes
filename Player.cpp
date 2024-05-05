//
// Created by alex on 5.5.24.
//

#include "Player.h"


Player::Player(int x, int y, int scale, uint16_t color, int fish_index, int score, int lives, int speed, int vector[2]) : Fish(x, y, scale, color, fish_index) {
    this->score = score;
    this->lives = lives;
    this->speed = speed;
    this->vector[0] = vector[0];
    this->vector[1] = vector[1];
}

void Player::handle_movement(int knob_values) {
    //printf("Handling player movement\n");
    int right_knob_val = (knob_values&0xff);
    int mid_knob_val = ((knob_values>>8)&0xff);
    //change the speed of the fish
    this->vector[0] = mid_knob_val / 16;
    this->vector[1] = mid_knob_val / 16;
    if ((knob_values&0x2000000)!=0) {
        this->vector[0] = 0;
        this->vector[1] = 0;
    }

    if ((right_knob_val >= 0 && right_knob_val < 16) || (right_knob_val >= 64 && right_knob_val < 80) ||
        (right_knob_val >= 128 && right_knob_val < 144) || (right_knob_val >= 192 && right_knob_val < 208)){
        if (this->vector[0] > 0){
            this->vector[0] *= -1;
        }
        if (this->vector[1] > 0) {
            this->vector[1] *= -1;
        }
    }else if ((right_knob_val >= 16 && right_knob_val < 32) || (right_knob_val >= 80 && right_knob_val < 96) ||
              (right_knob_val >= 144 && right_knob_val < 160) || (right_knob_val >= 208 && right_knob_val < 224)) {
        if (this->vector[0] > 0){
            this->vector[0] *= 1;
        }else{
            this->vector[0] *= -1;
        }
        if (this->vector[1] > 0) {
            this->vector[1] *= -1;
        }
    }else if ((right_knob_val >= 32 && right_knob_val < 48) || (right_knob_val >= 96 && right_knob_val < 112) ||
              (right_knob_val >= 160 && right_knob_val < 176) || (right_knob_val >= 224 && right_knob_val < 240)) {
        if (this->vector[0] > 0){
            this->vector[0] *= 1;
        }else{
            this->vector[0] *= -1;
        }
        if (this->vector[1] > 0) {
            this->vector[1] *= +1;
        }else{
            this->vector[1] *= -1;
        }
    }else if ((right_knob_val >= 48 && right_knob_val < 64) || (right_knob_val >= 112 && right_knob_val < 128) ||
              (right_knob_val >= 176 && right_knob_val < 192) || (right_knob_val >= 240 && right_knob_val < 256)) {
        if (this->vector[0] > 0) {
            this->vector[0] *= -1;
        }
        if (this->vector[1] > 0) {
            this->vector[1] *= +1;
        } else {
            this->vector[1] *= -1;
        }
    }

    this->x += this->vector[0];
    this->y += this->vector[1];

    this->x%=480;
    this->y%=320;
}

void Player::draw(bool IsFlipped) {
    //printf("Drawing player\n");
    this->vector[0] > 0 ? draw_fish_model(this->x, this->y, &fish_models, this->fish_index, this->color, this->scale) : draw_fish_model_flipped(this->x, this->y, &fish_models, this->fish_index, this->color, this->scale);
}

int Player::getX() {
    return this->x;
}

int Player::getY() {
    return this->y;
}