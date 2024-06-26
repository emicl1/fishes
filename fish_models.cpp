#include "fish_models.h"

static fish_model_bits_t fish_model_bits[] = {

    /* Character � (0xfe):
       ht=16, width=8
       +--------+
       |        |
       |  ***   |
       | *****  |
       |        |
       |        |
       | ****** |
       | ****** |
       | ****** |
       | ****** |
       | ****** |
       | ****** |
       |        |
       |        |
       |        |
       |        |
       |        |
       +--------+ */
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x80000000, // Row 0: 1000 0000 0000 0000
    0xc1f80000, // Row 1: 1100 0001 1111 1000
    0xe3fc0000, // Row 2: 1110 0011 1111 1100
    0xffff0000, // Row 3: 1111 1111 1111 1111
    0xe3fc0000, // Row 4: 1110 0011 1111 1100
    0xc1f80000, // Row 5: 1100 0001 1111 1000
    0x80000000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000000,
};

static uint32_t fish_model_offset[] = {
    4};

static unsigned char fish_models_width[] = {
    16};

static unsigned char fish_models_height[] = {
    7};

/* Exported structure definition. */
fish_model_descriptor_t fish_models = {
    fish_model_offset,
    fish_model_bits,
    fish_models_width,
    fish_models_height};
