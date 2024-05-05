#define _POSIX_C_SOURCE 200112L

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include <unistd.h>
#include <termios.h>            //termios, TCSANOW, ECHO, ICANON
#include <chrono>
#include <thread>


#include "draw_helper.h"
#include "bigger_fish.h"
#include "FishManager.h"
#include "Player.h"

using namespace std;


#define X_VECTOR 5
#define WIDTH_OF_FISH 60
#define HEIGHT_OF_FISH 60

extern unsigned short *fb;

int GameScreen(unsigned char *parlcd_mem_base, unsigned char *mem_base) {
    int i,j;
    int ptr;
    unsigned int c;
    fb  = (unsigned short *)malloc(320*480*2);

    FishManager manager;
    int vector[2] = {0, 0};
    Player player(240, 160, 4, 100000, 0, 0, 0, 0, vector);

    printf("Starting with the fish!\n");

    parlcd_write_cmd(parlcd_mem_base, 0x2c);
    ptr=0;
    for (i = 0; i < 320 ; i++) {
        for (j = 0; j < 480 ; j++) {
            c = 0;
            fb[ptr]=c;
            parlcd_write_data(parlcd_mem_base, fb[ptr++]);
        }
    }

    struct timespec loop_delay;
    loop_delay.tv_sec = 0;
    loop_delay.tv_nsec = 150 * 1000 * 1000;

    int r = *(volatile uint32_t*)(mem_base + SPILED_REG_KNOBS_8BIT_o);


    random_device rd;
    mt19937 rng(rd());
    uniform_int_distribution<int> uni(2, 5);

    auto lastSpawnTime = chrono::steady_clock::now();

    while (1) {

        // Exit the loop if you click a button
        if ((r&0x1000000)!=0) {

            exit(0);
        }
        r = *(volatile uint32_t*)(mem_base + SPILED_REG_KNOBS_8BIT_o);


        auto currentTime = std::chrono::steady_clock::now();
        std::chrono::duration<float> elapsed = currentTime - lastSpawnTime;

        if (elapsed.count() >= uni(rng)) {  // Check if the elapsed time is greater than or equal to the random interval
            manager.spawnFish();  // Spawn a new fish
            lastSpawnTime = currentTime;  // Reset the last spawn time
        }


        manager.moveAllFishes();
        player.handle_movement(r);
        if (manager.whoIsEaten(player)) {
            printf("Player was eaten\n");
            break;
        }

        // Clear the screen
        for (ptr = 0; ptr < 320*480 ; ptr++) {
            fb[ptr]=0u;
        }
        // Draw all fishes
        manager.drawAllFishes();
        // Draw the player
        player.draw();


        parlcd_write_cmd(parlcd_mem_base, 0x2c);
        for (ptr = 0; ptr < 480*320 ; ptr+=2) {
            uint32_t value_to_write = 0;
            value_to_write |= fb[ptr];
            value_to_write <<= 16;
            value_to_write |= fb[ptr+1];
            parlcd_write_data2x(parlcd_mem_base, value_to_write);
        }

        clock_nanosleep(CLOCK_MONOTONIC, 0, &loop_delay, NULL);

        //TODO remove this if slow
        this_thread::sleep_for(std::chrono::milliseconds(16));
    }

    parlcd_write_cmd(parlcd_mem_base, 0x2c);
    for (ptr = 0; ptr < 480*320 ; ptr+=2) {
        parlcd_write_data2x(parlcd_mem_base, 0);
    }

    printf("Goodbye world\n");

    return 0;
}
