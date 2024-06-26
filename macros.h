#define _POSIX_C_SOURCE 200112L
/**
 * @file macros.h
 * @brief 
 * A collection of useful macros for the purposes of our game.
 */
#ifndef MACROS_H
#define MACROS_H

/**
 * @def LCD_WIDTH
 * @brief 
 * This macro tells us the width of the LCD display we are working on.
 */
#define LCD_WIDTH 480

/**
 * @def LCD_HEIGHT
 * @brief 
 * This macro tells us the height of the LCD display we are working on.
 */
#define LCD_HEIGHT 320
#define TEXT_SCALE 4

#define COLOR uint16_t
#define COLOR_32 uint32_t

/**
 * @def RGB(r, g, b)
 * @brief 
 * This macro gives us the desired uint16 value for the color specified in the parameters.
 *
 * @param r
 * the value (0-255) of the red color component
 * @param g
 * the value (0-255) of the green color component
 * @param b
 * the value (0-255) of the blue color component
 */
#define RGB(r, g, b) ((COLOR) (((r) & 0x1F) << 11) | (((g) & 0x3F) << 5) | ((b) & 0x1F))

/**
 * @def RGB_888(r, g, b)
 * @brief 
 * This macro gives us the desired uint32 value for the color specified in the parameters.
 *
 * @param r
 * the value (0-255) of the red color component
 * @param g
 * the value (0-255) of the green color component
 * @param b
 * the value (0-255) of the blue color component
 */
#define RGB_888(r,g,b) ((COLOR_32) (((r) & 0xFF) << 16) | (((g) & 0xFF) << 8) | ((b) & 0xFF))

#define COLOR_WHITE RGB(255, 255, 255)
#define COLOR_RED RGB(255, 0, 0)
#define COLOR_GREEN RGB(0, 255, 0)
#define COLOR_BLUE RGB(0, 0, 255)
#define COLOR_BLACK RGB(0, 0, 0)
#define COLOR_YELLOW RGB(255, 255, 0)
#define COLOR_CYAN RGB(0, 255, 255)
#define COLOR_MAGENTA RGB(255, 0, 255)
#define COLOR_ORANGE RGB(255, 165, 0)
#define COLOR_PURPLE RGB(128, 0, 128)
#define COLOR_PINK RGB(255, 192, 203)
#define COLOR_BROWN RGB(165, 42, 42)
#define COLOR_LIGHT_BLUE RGB(173, 216, 230)
#define COLOR_LIGHT_GREEN RGB(144, 238, 144)
#define COLOR_LIGHT_YELLOW RGB(255, 255, 224)
#define COLOR_LIGHT_CYAN RGB(224, 255, 255)
#define COLOR_LIGHT_MAGENTA RGB(255, 224, 255)
#define COLOR_LIGHT_ORANGE RGB(255, 218, 185)
#define COLOR_LIGHT_PURPLE RGB(221, 160, 221)
#define COLOR_LIGHT_PINK RGB(255, 182, 193)
#define COLOR_LIGHT_BROWN RGB(210, 105, 30)
#define COLOR_DARK_BLUE RGB(0, 0, 139)
#define COLOR_DARK_GREEN RGB(0, 100, 0)


#define COLOR_888_GREEN RGB_888(0, 255, 0)
#define COLOR_888_RED RGB_888(255, 0, 0)
#define COLOR_888_BLUE RGB_888(0, 0, 255)
#define COLOR_888_ORANGE RGB_888(255, 165, 0)


#endif //MACROS_H