//
// Created by alex on 5.5.24.
//


#include "Player.h"
#include "macros.h"



Player::Player(int x, int y, int scale, uint16_t color, int fish_index, int score, int lives, int vector[2]) : Fish(x, y, scale, color, fish_index) {
    this->score = score;
    this->lives = lives;
    this->vector[0] = vector[0];
    this->vector[1] = vector[1];
}

void Player::handle_movement(int knob_values) {
    //printf("Handling player movement\n");
    int right_knob_val = (knob_values&0xff);
    int mid_knob_val = ((knob_values>>8)&0xff);
    //change the speed of the fish
    if (hasBoost && (knob_values&0x4000000)){
        printf("Boosting\n");
        this->vector[0] = mid_knob_val / 2;
        this->vector[1] = mid_knob_val / 2;
        this->hasBoost = false;
    }else{
        this->vector[0] = mid_knob_val / 16;
        this->vector[1] = mid_knob_val / 16;
    }

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


    this->x%=LCD_WIDTH;
    this->y%=LCD_HEIGHT;

    this->x < 0 ? this->x += LCD_WIDTH : this->x = this->x;
    this->y < 0 ? this->y += LCD_HEIGHT : this->y = this->y;
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

void Player::setScore(int score) {
    printf("Setting score to %d\n", this->score);
    this->score = score;
    printf("Score set to %d\n", this->score);
}

void Player::setScale(int scale) {
    this->scale = scale;
}

int Player::getScore() {
    return this->score;
}

int Player::getScale() {
    return this->scale;
}

bool Player::getHasBoost() {
    return this->hasBoost;
}

void Player::setHasBoost(bool hasBoost) {
    this->hasBoost = hasBoost;
}

void Player::setVector(int x, int y) {
    this->vector[0] = x;
    this->vector[1] = y;
}

int Player::getYVector() {
    return this->vector[1];
}
