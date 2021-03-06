/*
    This file is part of dynasmgb.

    dynasmgb is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    dynasmgb is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with dynasmgb.  If not, see <http://www.gnu.org/licenses/>.
    
    Copyright 2014, 2015 Thomas Witte
*/

#include "sound.h"

typedef struct {
    int offset[4];
    int amplitude[4];
    unsigned env_count[4];
    int noise_val;
    int offset_256hz;
} sound_detail;

void sound_reg_write(gb_sound *sound, uint16_t addr, uint8_t val, uint64_t time) {
}

void audio_callback(void *userdata, int8_t *stream, int length) {
    sound_detail *sound = ((gb_sound*) userdata)->snd;
    uint8_t *memory = ((gb_sound*) userdata)->memory->mem;
    
    int count[4] = {0,0,0,0};
    int freq[4] = {440,440,440,440};
    int duty[2] = {4,4};
    
    int cnt_256hz = (length + sound->offset_256hz) * 256 / SAMPLE_FREQUENCY;
    sound->offset_256hz = (length + sound->offset_256hz) % (SAMPLE_FREQUENCY / 256);

    if(memory[0xFF14] & 0x80) { // reset sound 1
        memory[0xFF14] &= ~0x80;
        if(!(memory[0xFF11] & 0x3f))
            memory[0xFF11] |= 0x3f;
        sound->amplitude[0] = (memory[0xFF12] & 0xf0) >> 1;
        //memory[0xFF12] &= ~0x07;

        memory[0xFF26] |= 0x01;
    }
    
    if(memory[0xFF19] & 0x80) { // reset sound 2
        memory[0xFF19] &= ~0x80;
        if(!(memory[0xFF16] & 0x3f))
            memory[0xFF16] |= 0x3f;
        sound->amplitude[1] = (memory[0xFF17] & 0xf0) >> 1;
        //memory[0xFF17] &= ~0x07;
        
        memory[0xFF26] |= 0x02;
    }
   
    if(memory[0xFF23] & 0x80) { // reset sound 4
        memory[0xFF23] &= ~0x80;
        if(!(memory[0xFF20] & 0x3f))
            memory[0xFF20] |= 0x3f;
        sound->amplitude[3] = (memory[0xFF21] & 0xf0) >> 1;
                
        memory[0xFF26] |= 0x04;
    }
   
    if(memory[0xFF26] & 0x80) { // sound enabled?
        // sound 1
        duty[0] = (memory[0xFF11] & 0xc0) >> 5;
        count[0] = (memory[0xFF11] & 0x3f) * SAMPLE_FREQUENCY / 256;
        if(count[0] - length > 0) {
            memory[0xFF11] -= cnt_256hz;
        } else {
            memory[0xFF11] &= 0xc0;
        }
        
        freq[0] = 131072/(2048 - memory[0xFF13] - 256*(memory[0xFF14] & 0x07));
        
        if(sound->amplitude[0] && memory[0xFF12] & 0x07) { // envelope sweep active
        
            sound->env_count[0] += cnt_256hz;
            uint8_t steps = sound->env_count[0] / (uint8_t)(memory[0xFF12] & 0x07);
            sound->env_count[0] %= (uint8_t)(memory[0xFF12] & 0x07);
            
            // calculate new amplitude
            sound->amplitude[0] += (memory[0xFF12] & 0x08 ? 1 : -1) * 8 * steps;
            if(sound->amplitude[0] < 0) {
                sound->amplitude[0] = 0;
                //memory[0xFF12] &= ~0x07;
            }
            if(sound->amplitude[0] > (0xf0 >> 1)) {
                sound->amplitude[0] = 0xf0 >> 1;
                //memory[0xFF12] &= ~0x07;
            }
        }
        
        // sound 2
        duty[1] = (memory[0xFF16] & 0xc0) >> 5;
        count[1] = (memory[0xFF16] & 0x3f) * SAMPLE_FREQUENCY / 256;
        memory[0xFF16] &= 0xc0;
        if(count[0] - length > 0) {
            memory[0xFF16] -= cnt_256hz;
        } else {
            memory[0xFF16] &= 0xc0;
        }
        
        freq[1] = 131072/(2048 - memory[0xFF18] - 256*(memory[0xFF19] & 0x07));
        
        if(sound->amplitude[1] && memory[0xFF17] & 0x07) { // envelope sweep active
        
            sound->env_count[1] += cnt_256hz;
            uint8_t steps = sound->env_count[1] / (uint8_t)(memory[0xFF17] & 0x07);
            sound->env_count[1] %= (uint8_t)(memory[0xFF17] & 0x07);
            
            // calculate new amplitude
            sound->amplitude[1] += (memory[0xFF17] & 0x08 ? 1 : -1) * 8 * steps;
            if(sound->amplitude[1] < 0) {
                sound->amplitude[1] = 0;
                //memory[0xFF17] &= ~0x07;
            }
            if(sound->amplitude[1] > (0xf0 >> 1)) {
                sound->amplitude[1] = 0xf0 >> 1;
                //memory[0xFF17] &= ~0x07;
            }
        }
        
        // sound 4
        count[3] = (memory[0xFF20] & 0x3f) * SAMPLE_FREQUENCY / 256;
        memory[0xFF20] &= 0xc0;
        if(count[0] - length > 0) {
            memory[0xFF20] -= cnt_256hz;
        } else {
            memory[0xFF20] &= 0xc0;
        }
        int s = ((memory[0xFF22] & 0xf0) >> 4) + 1;
        int r = (memory[0xFF22] & 0x03) ? (memory[0xFF22] & 0x03)*2 : 1; // 1 ist falsch, eigentlich 0.5
        freq[3] = 1048576 / r / (1<<s);

        if(sound->amplitude[3] && memory[0xFF21] & 0x07) { // envelope sweep active
        
            sound->env_count[3] += cnt_256hz;
            uint8_t steps = sound->env_count[3] / (uint8_t)(memory[0xFF21] & 0x07);
            sound->env_count[3] %= (uint8_t)(memory[0xFF21] & 0x07);
            
            // calculate new amplitude
            sound->amplitude[3] += (memory[0xFF21] & 0x08 ? 1 : -1) * 8 * steps;
            if(sound->amplitude[3] < 0) {
                sound->amplitude[3] = 0;
                //memory[0xFF21] &= ~0x07;
            }
            if(sound->amplitude[3] > (0xf0 >> 1)) {
                sound->amplitude[3] = 0xf0 >> 1;
                //memory[0xFF21] &= ~0x07;
            }
        }
    }
    
    for(int i = 0; i < 2; ++i)
        if(duty[i] == 0)
            duty[i] = 1;
    
    int T[4] = {SAMPLE_FREQUENCY / freq[0] ?: 1,
                SAMPLE_FREQUENCY / freq[1] ?: 1,
                SAMPLE_FREQUENCY / freq[2],
                SAMPLE_FREQUENCY / freq[3] ?: 1};
    
    int s01 = 0, s02 = 0;
    for(int i = 0; i < length; ++i) {
        
        if(sound->amplitude[3] && (i + sound->offset[3])%T[3] == 0) {
            sound->noise_val = (rand()%2) ? -1 : 1;
        }
        
        if(i > count[0] && memory[0xFF15] & 0x20) {
            sound->amplitude[0] = 0;
            memory[0xFF26] &= ~0x01; // reset sound on flag
        }

        if(i > count[1] && memory[0xFF19] & 0x20) {
            sound->amplitude[1] = 0;
            memory[0xFF26] &= ~0x02; // reset sound on flag
        }

        if(i > count[3] && memory[0xFF23] & 0x20) {
            sound->amplitude[3] = 0;
            memory[0xFF26] &= ~0x04; // reset sound on flag
        }
        
        s01 = (memory[0xFF25] & 0x01) * (((8*(i + sound->offset[0])/T[0]) % 8) < duty[0] ? sound->amplitude[0] : -sound->amplitude[0])
            + (memory[0xFF25] & 0x02) * (((8*(i + sound->offset[1])/T[1]) % 8) < duty[1] ? sound->amplitude[1] : -sound->amplitude[1])
            + (memory[0xFF25] & 0x08) * (sound->noise_val * sound->amplitude[3]);
             
        s02 = (memory[0xFF25] & 0x10) * (((8*(i + sound->offset[0])/T[0]) % 8) < duty[0] ? sound->amplitude[0] : -sound->amplitude[0])
            + (memory[0xFF25] & 0x20) * (((8*(i + sound->offset[1])/T[1]) % 8) < duty[1] ? sound->amplitude[1] : -sound->amplitude[1])
            + (memory[0xFF25] & 0x80) * (sound->noise_val * sound->amplitude[3]);
        
        s01 *= ((memory[0xFF24] & 0x70) >> 4) + 1;
        s02 *= (memory[0xFF24] & 0x07) + 1;
        
        stream[i] = (s01 + s02) / 48;
    }
    
    sound->offset[0] = (length + sound->offset[0]) % T[0];
    sound->offset[1] = (length + sound->offset[1]) % T[1];
    sound->offset[2] = (length + sound->offset[2]) % T[2];
    sound->offset[3] = (length + sound->offset[3]) % T[3];
}

bool init_sound(gb_sound *sound, gb_memory *memory) {
    if(SDL_InitSubSystem(SDL_INIT_AUDIO) != 0)
        return false;
    
    SDL_AudioSpec want, have;
    
    want.freq = SAMPLE_FREQUENCY;
    want.format = AUDIO_S8;
    want.channels = 1;
    want.samples = 512;
    want.callback = (void*) audio_callback;
    want.userdata = (void*) sound;
    
    sound->snd = malloc(sizeof(sound_detail));
    
    for(int i = 0; i < 4; ++i) {
        sound->snd->offset[i] = 0;
        sound->snd->amplitude[i] = 0;
        sound->snd->env_count[i] = 0;
    }
    sound->memory = memory;
    sound->snd->noise_val = 0;
    sound->snd->offset_256hz = 0;
    
    if(SDL_OpenAudio(&want, &have) != 0)
        return false;
    
    SDL_PauseAudio(0);
    return true;
}

void pause_sound(gb_sound *sound, bool pause) {
    free(sound->snd);

    SDL_PauseAudio(pause ? 1 : 0);
}

bool deinit_sound(gb_sound *sound) {
    SDL_PauseAudio(1);
    SDL_CloseAudio();
    return true;
}
