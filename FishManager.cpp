#include "FishManager.h"  // Include the header file instead of redefining the class

FishManager::FishManager()
        : dist_x(0, 480), dist_y(0, 320), dist_scale(1, 7), dist_direction(0, 1){
    std::random_device rd;
    rng.seed(rd());
}

void FishManager::spawnFish() {
    int x = dist_x(rng);
    int y = dist_y(rng);
    int scale = dist_scale(rng);
    uint16_t color = 0xFFFF; // White, for example; customize as needed
    int fish_index = 0; // Assuming only one type of fish for simplicity
    // add fish to random vector of shies facing right or left
    int direction = dist_direction(rng);

    if (direction == 0) {
        fishes_facing_right.push_back(Fish(x, y, scale, color, fish_index));
    } else {
        fishes_facing_left.push_back(Fish(x, y, scale, color, fish_index));
    }

}

void FishManager::drawAllFishes() {

    for (Fish &fish : fishes_facing_right) {
        fish.draw();
    }
    for (Fish &fish : fishes_facing_left) {
        fish.draw(true);
    }
}

void FishManager::moveAllFishesRight() {
    for (Fish &fish : fishes_facing_right) {
        // Move the fish
        fish.x += 5;
        if (fish.x > 480) {
            fish.x = 0;
        }
    }
}

void FishManager::moveAllFishesLeft() {
    for (Fish &fish : fishes_facing_left) {
        // Move the fish
        fish.x -= 5;
        if (fish.x < 0) {
            fish.x = 480;
        }
    }
}

void FishManager::moveAllFishes() {
    moveAllFishesRight();
    moveAllFishesLeft();
}

Fish FishManager::collsionDetection(Player player) {
    // Check for collision with player
    for (Fish &fish : fishes_facing_right) {
        if (doRectanglesOverlap(fish.x, fish.y,((int) fish_models.widths[fish.fish_index] )* fish.scale,
                                  ((int) fish_models.heights[fish.fish_index] )* fish.scale, player.x, player.y,
                                  ((int) fish_models.widths[player.fish_index] )* player.scale,
                                  ((int) fish_models.heights[player.fish_index] )* player.scale)) {
            printf("Collision detected\n");
            return fish;
        }
    }
    for (Fish &fish : fishes_facing_left) {
        if (doRectanglesOverlap(fish.x, fish.y,((int) fish_models.widths[fish.fish_index] )* fish.scale,
                                  ((int) fish_models.heights[fish.fish_index] )* fish.scale, player.x, player.y,
                                  ((int) fish_models.widths[player.fish_index] )* player.scale,
                                  ((int) fish_models.heights[player.fish_index] )* player.scale)){
            printf("Collision detected\n");
            return fish;
        }
    }
    return Fish(-1, -1, -1, 0, -1); // No collision
}


bool FishManager::isPointInArea(int x, int y, int x1, int y1, int width, int height) {
    return x >= x1 && x < x1 + width && y >= y1 && y < y1 + height;
}

bool FishManager::doRectanglesOverlap(int x1, int y1, int width1, int height1, int x2, int y2, int width2, int height2) {
    if (isPointInArea(x1, y1, x2, y2, width2, height2)) {
        return true;
    }
    if (isPointInArea(x1 + width1, y1, x2, y2, width2, height2)) {
        return true;
    }

    if (isPointInArea(x1, y1 + height1, x2, y2, width2, height2)) {
        return true;
    }

    if (isPointInArea(x1 + width1, y1 + height1, x2, y2, width2, height2)) {
        return true;
    }

    return false;
}

bool FishManager::whoIsEaten(Player player){
    if (whoIsBigger(player)){
        return true;
    }
    return false;
}

void FishManager::removeFish(Fish fish) {
    if (fish.fish_index != -1) {
        for (int i = 0; i < fishes_facing_right.size(); i++) {
            if (fishes_facing_right[i].x == fish.x && fishes_facing_right[i].y == fish.y) {
                fishes_facing_right.erase(fishes_facing_right.begin() + i);
                return;
            }
        }
        for (int i = 0; i < fishes_facing_left.size(); i++) {
            if (fishes_facing_left[i].x == fish.x && fishes_facing_left[i].y == fish.y) {
                fishes_facing_left.erase(fishes_facing_left.begin() + i);
                return;
            }
        }
    }
}


bool FishManager::whoIsBigger(Player player){
    Fish eatenFish = collsionDetection(player);
    if (eatenFish.fish_index != -1){
        if (player.scale > eatenFish.scale){
            removeFish(eatenFish);
            player.score += 1;
            return true;
        }
    }
    return false;
}