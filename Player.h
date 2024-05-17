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

    /**
     * Create a new Player object.
     * @brief
     * Constructor.
     * @param x
     * The x coordinate of the player
     * @param y
     * The y coordinate of the player
     * @param scale
     * The scale of the player
     * @param color
     * The color of the player
     * @param fish_index
     * Index of the fish in our fish models. Determines the shape of the player
     * @param score
     * The score of the player
     * @param lives
     * Amount of lives the player has
     * @param vector
     * A length-2 array with the sizes of our vector
     * The first element of the list determines the speed of our fish in the x direction
     * The second element of the list determines the speed of our fish in the y direction
     */
    Player(int x, int y, int scale, uint16_t color, int fish_index, int score, int lives, int vector[2]);


    /**
     * @brief
     * This function draws the player onto the frame buffer.
     * 
     * @param isFlipped
     * States whether to draw the fish pointing to the right or to the left. True - poining to the left, False - pointing to the right
    */
    void draw(bool IsFlipped = false);

    /**
     * @brief
     * Reads middle, right knob and middle toggle switch values.
     * Middle knob handles velocity, right knob handles direction.
     * Middle toggle stops the player.
     * 
     * @param knob_values
     * Is the integer value that is read from the memory to where the knobs are mapped
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
