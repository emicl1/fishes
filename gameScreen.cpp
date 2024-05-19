#define _POSIX_C_SOURCE 200112L

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include <unistd.h>
#include <termios.h> //termios, TCSANOW, ECHO, ICANON
#include <chrono>
#include <thread>

#include "macros.h"
#include "draw_helper.h"
#include "bigger_fish.h"
#include "FishManager.h"
#include "Player.h"

using namespace std;

extern unsigned short *fb;

#define TEXT_SCALE 3

void show_win_screen(unsigned char *parlcd_mem_base)
{
    int i, j;
    int ptr;
    unsigned int c;
    parlcd_write_cmd(parlcd_mem_base, 0x2c);
    ptr = 0;
    for (i = 0; i < 320; i++)
    {
        for (j = 0; j < LCD_WIDTH; j++)
        {
            c = 0;
            fb[ptr] = c;
            parlcd_write_data(parlcd_mem_base, fb[ptr++]);
        }
    }
    int x = 100;
    int y = 120;
    char str[] = "You won! :)";
    char *ch = str;
    for (i = 0; i < 11; i++)
    {
        draw_char(x, y, &font_winFreeSystem14x16, *ch, COLOR_WHITE, TEXT_SCALE);
        x += TEXT_SCALE * char_width(&font_winFreeSystem14x16, *ch);
        ch++;
    }
    for (ptr = 0; ptr < LCD_WIDTH * LCD_HEIGHT; ptr += 2)
    {
        uint32_t value_to_write = 0;
        value_to_write |= fb[ptr];
        value_to_write <<= 16;
        value_to_write |= fb[ptr + 1];
        parlcd_write_data2x(parlcd_mem_base, value_to_write);
    }
    sleep(3);
}

void show_lose_screen(unsigned char *parlcd_mem_base)
{
    int i, j;
    int ptr;
    parlcd_write_cmd(parlcd_mem_base, 0x2c);
    for (i = 0; i < LCD_HEIGHT; i++)
    {
        for (j = 0; j < LCD_WIDTH / 2; j++)
        { // half the width, because we are writing 2x
            parlcd_write_data2x(parlcd_mem_base, 0);
        }
    }
    int x = 60;
    int y = 120;
    char str[] = "Game Over :( (loser)";
    char *ch = str;
    for (i = 0; i < 20; i++)
    {
        draw_char(x, y, &font_winFreeSystem14x16, *ch, COLOR_WHITE, TEXT_SCALE);
        x += TEXT_SCALE * char_width(&font_winFreeSystem14x16, *ch);
        ch++;
    }
    for (ptr = 0; ptr < LCD_WIDTH * LCD_HEIGHT; ptr += 2)
    {
        uint32_t value_to_write = 0;
        value_to_write |= fb[ptr];
        value_to_write <<= 16;
        value_to_write |= fb[ptr + 1];
        parlcd_write_data2x(parlcd_mem_base, value_to_write);
    }
    sleep(3);
}

int GameScreen(unsigned char *parlcd_mem_base, unsigned char *mem_base)
{
    int i, j;
    int ptr;

    FishManager manager;
    int vector[2] = {0, 0};
    Player player(240, 160, 4, 100000, 0, 0, 0, vector);

    printf("Starting with the fish!\n");

    parlcd_write_cmd(parlcd_mem_base, 0x2c);
    for (i = 0; i < LCD_HEIGHT; i++)
    { // half the width, because we are writing 2x
        for (j = 0; j < LCD_WIDTH / 2; j++)
        {
            parlcd_write_data2x(parlcd_mem_base, 0);
        }
    }
    struct timespec loop_delay;
    loop_delay.tv_sec = 0;
    loop_delay.tv_nsec = 150 * 1000 * 1000;

    int r = *(volatile uint32_t *)(mem_base + SPILED_REG_KNOBS_8BIT_o);

    random_device rd;
    mt19937 rng(rd());
    uniform_int_distribution<int> uni(2, 5);

    auto lastSpawnTime = chrono::steady_clock::now();

    int stateTime = 0;
    while (1)
    {
        // Exit the loop if you click a button
        if ((r & 0x1000000) != 0)
        {
            exit(0);
        }
        r = *(volatile uint32_t *)(mem_base + SPILED_REG_KNOBS_8BIT_o);

        auto currentTime = std::chrono::steady_clock::now();
        std::chrono::duration<float> elapsed = currentTime - lastSpawnTime;
        stateTime += elapsed.count();

        if (elapsed.count() >= uni(rng))
        {                                // Check if the elapsed time is greater than or equal to the random interval
            manager.spawnFish();         // Spawn a new fish
            lastSpawnTime = currentTime; // Reset the last spawn time
        }
        uint32_t ledReg = *(volatile uint32_t *)(mem_base + SPILED_REG_LED_RGB1_o);

        if (player.getHasBoost())
        {
            printf("Changing the RGB to green\n");
            printf("Elapsed time: %f\n", elapsed.count());
            printf("value of ledReg: %d\n", ledReg);
            ledReg = COLOR_888_GREEN;
        }
        else
        {
            printf("Changing the RGB to RED\n");
            printf("Elapsed time: %f\n", elapsed.count());
            printf("value of ledReg: %d\n", ledReg);
            ledReg = COLOR_888_RED;
        }

        *(volatile uint32_t *)(mem_base + SPILED_REG_LED_RGB1_o) = ledReg;
        if (stateTime % 20 == 19)
        {
            player.setHasBoost(true);
        }

        manager.moveAllFishes();
        player.handle_movement(r);
        if (!manager.whoIsEaten(&player))
        {
            printf("Player was eaten\n");
            show_lose_screen(parlcd_mem_base);
            sleep(3);
            break;
        }
        if (player.scale >= 7)
        {
            printf("Player won\n");
            show_win_screen(parlcd_mem_base);
            break;
        }
        // Clear the screen
        for (ptr = 0; ptr < LCD_HEIGHT * LCD_WIDTH; ptr++)
        {
            fb[ptr] = 0u;
        }
        // Draw all fishes
        manager.drawAllFishes();
        // Draw the player
        player.draw();

        parlcd_write_cmd(parlcd_mem_base, 0x2c);
        for (ptr = 0; ptr < LCD_WIDTH * LCD_HEIGHT; ptr += 2)
        {
            uint32_t value_to_write = 0;
            value_to_write |= fb[ptr];
            value_to_write <<= 16;
            value_to_write |= fb[ptr + 1];
            parlcd_write_data2x(parlcd_mem_base, value_to_write);
        }

        clock_nanosleep(CLOCK_MONOTONIC, 0, &loop_delay, NULL);

        // TODO remove this if slow
        this_thread::sleep_for(std::chrono::milliseconds(16));
    }

    parlcd_write_cmd(parlcd_mem_base, 0x2c);
    for (ptr = 0; ptr < LCD_WIDTH * LCD_HEIGHT; ptr += 2)
    {
        parlcd_write_data2x(parlcd_mem_base, 0);
    }

    printf("Goodbye world\n");

    return 0;
}
