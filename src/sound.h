#ifndef DYNASMGB_SOUND_H
#define DYNASMGB_SOUND_H

#include <SDL/SDL.h>
#include <SDL/SDL_audio.h>

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "memory.h"

#define SAMPLE_FREQUENCY 22050

typedef struct {
    gb_memory *memory;
    struct sound_detail *snd;
} gb_sound;

void sound_reg_write(gb_sound *sound, uint16_t addr, uint8_t val, uint64_t time);

bool init_sound(gb_sound *sound, gb_memory *memory);

void pause_sound(gb_sound *sound, bool pause);

bool deinit_sound(gb_sound *sound);

#endif
