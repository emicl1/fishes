//
// Created by alex on 5.5.24.
//

#ifndef FISHES_FISHMANAGER_H
#define FISHES_FISHMANAGER_H


#include "Player.h"
#include <random>


class FishManager {
public:
    std::vector<Fish> fishes_facing_right;
    std::vector<Fish> fishes_facing_left;
    std::mt19937 rng;
    std::uniform_int_distribution<int> dist_x, dist_y, dist_scale, dist_direction;

    explicit FishManager();

    /**
     * @brief
     * Spawns a random fish at a random place on the display.
    */
    void spawnFish();

    /**
     * @brief
     * Draws all our fishes on the display.
    */
    void drawAllFishes();

    /**
     * @brief
     * Moves all the fish that should move to the right.
    */
    void moveAllFishesRight();

    /**
     * @brief
     * Moves all the fish that should move to the left.
    */
    void moveAllFishesLeft();

    /**
     * @brief
     * Moves all the fish. Uses the functions moveAllFishesRight and moveAllFishesLeft.
    */
    void moveAllFishes();

    /**
     * @brief
     * Detects a collision between a player and an enemy fish.
     * 
     * @param player
     * A pointer to the player fish
     * 
     * @returns Fish
     * The fish that we collided with
    */
    Fish collsionDetection(Player *player);

    /**
     * @brief
     * Removes a fish
     * 
     * @param fish
     * The fish object we wish to remove
    */
    void removeFish(Fish fish);

    /**
     * @brief
     * A helper function to decide whether two rectangles overlap.
     * 
     * @param x1
     * The x coordinate of the first rectangle
     * @param y1
     * The y coordinate of the first rectangle
     * @param width1
     * The width of the first rectangle
     * @param height1
     * The height of the first rectangle
     * @param x2
     * The x coordinate of the second rectangle
     * @param y2
     * The y coordinate of the second rectangle
     * @param width2
     * The width of the second rectangle
     * @param height2
     * The height of the second rectangle
     * @retval TRUE
     * If the rectangles are overlapping
     * @retval FALSE
     * If the rectangles are not overlapping
    */
    bool doRectanglesOverlap(int x1, int y1, int width1, int height1, int x2, int y2, int width2, int height2);

    /**
     * @brief
     * Determines whether the player or the fish should be eaten.
     * 
     * @param player
     * A pointer to our player
     * 
     * @retval TRUE
     * If the player is bigger than the fish, thus the fish is eaten
     * @retval FALSE
     * If the player is smaller than the fish and therefore is eaten
    */
    bool whoIsEaten(Player *player);

    /**
     * @brief
     * Determines whether a point is in a certain area.
     * 
     * @param x
     * The x coordinate of the point
     * @param y
     * The y coordinate of the point
     * @param x1
     * The x coordinate of the area
     * @param y1
     * The y coordinate of the area
     * @param width
     * The width of the area
     * @param height
     * The height of the area
     * @retval TRUE
     * If the point is in the area
     * @retval FALSE
     * If the point is not in the area
    */
    bool isPointInArea(int x, int y, int x1, int y1, int width, int height);

    /**
     * @brief
     * Tells us whether the player is bigger than the fish.
     * 
     * @param player
     * A pointer to our player
     * 
     * @retval TRUE
     * If the player is bigger than the fish it collided with
     * @retval FALSE
     * If the fish is bigger than the player
    */
    bool whoIsBigger(Player *player);


};


#endif //FISHES_FISHMANAGER_H
