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

    void spawnFish();

    void drawAllFishes();

    void moveAllFishesRight();

    void moveAllFishesLeft();

    void moveAllFishes();

    Fish collsionDetection(Player player);

    void removeFish(Fish fish);

    bool doRectanglesOverlap(int x1, int y1, int width1, int height1, int x2, int y2, int width2, int height2);

    bool whoIsEaten(Player player);

    bool isPointInArea(int x, int y, int x1, int y1, int width, int height);

    bool whoIsBigger(Player player);


};


#endif //FISHES_FISHMANAGER_H
