//
// Created by alex on 5.5.24.
//

#ifndef FISHES_PLAYER_H
#define FISHES_PLAYER_H

#include "Fish.h"


class Player : public Fish {
public:
    int score;
    int lives;
    int vector[2];
    bool hasBoost = false;

    Player(int x, int y, int scale, uint16_t color, int fish_index, int score, int lives, int vector[2]);


    void draw(bool IsFlipped = false);

    /*
     * Reads middle, right knob and middle toggle switch values
     * middle knob handles velocity, right knob handles direction
     * middle toggle stops the player
     */
    void handle_movement(int knob_values);

    int getX();

    int getY();

    void setVector(int x, int y);

    void setSpeed(int speed);

    void setScore(int score);

    void setLives(int lives);

    void setScale(int scale);

    int getScale();

    int getScore();

    bool getHasBoost();

    void setHasBoost(bool hasBoost);
};


#endif //FISHES_PLAYER_H
