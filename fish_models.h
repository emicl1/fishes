#ifndef FISH_MODELS_H
#define FISH_MODELS_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef uint32_t fish_model_bits_t;

/* builtin C-based proportional/fixed font structure*/
typedef struct {
    const fish_model_bits_t *offset;        /* offsets into bitmap data*/
    const fish_model_bits_t *bits;        /* 16-bit right-padded bitmap data*/
    const unsigned char *widths;     /* character widths or 0 if fixed*/
    const unsigned char *heights;     /* character widths or 0 if fixed*/
} fish_model_descriptor_t;

extern fish_model_descriptor_t fish_models;

#ifdef __cplusplus
} /* extern "C"*/
#endif

#endif  /*FISH_MODELS_H*/
