/**
 * @file fish_models.h
 * @brief 
 * This file that sets the template of our fish models.
 */
#ifndef FISH_MODELS_H
#define FISH_MODELS_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef uint32_t fish_model_bits_t;
/**
 * @brief
 * A template of the model we use for drawing fish.
 * @param offset
 * An array of integers that determine how much we need to shift in the array with our bitmaps to get to the fish we want
 * @param bits
 * An array of bitmaps with the fishes
 * @param widths
 * An array of widths of our fishes
 * @param heights
 * An array of heights of our fishes
 */
typedef struct {
    const fish_model_bits_t *offset;        /* offsets into bitmap data*/
    const fish_model_bits_t *bits;        /* 16-bit right-padded bitmap data*/
    const unsigned char *widths;     /* character widths or 0 if fixed*/
    const unsigned char *heights;     /* character widths or 0 if fixed*/
} fish_model_descriptor_t;
/**
 * @brief
 * The specific model we use for drawing fish.
 */
extern fish_model_descriptor_t fish_models;

#ifdef __cplusplus
} /* extern "C"*/
#endif

#endif  /*FISH_MODELS_H*/
