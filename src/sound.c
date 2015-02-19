#include "sound.h"

void audio_callback(void *userdata, int8_t *stream, int length) {
    gb_sound *sound = (gb_sound*) userdata;
    uint8_t *memory = sound->memory->mem;
    
    int8_t amplitude[4] = {0,0,0,0};
    int freq[4] = {440,440,440,440};
    int duty[2] = {4,4};
    
    if(memory[0xFF26] & 0x80) { // sound enabled?
        if(memory[0xFF25] & 0x11) { // sound 1 is routed to sound output terminal
            duty[0] = (memory[0xFF11] & 0xc0) >> 5;
            freq[0] = 131072/(2048 - memory[0xFF13] - 256*(memory[0xFF14] & 0x07));
            amplitude[0] = (memory[0xFF12] & 0xf0) >> 2;
        }
        
        if(memory[0xFF25] & 0x22) { // sound 2 is routed to sound output terminal
            duty[1] = (memory[0xFF16] & 0xc0) >> 5;
            freq[1] = 131072/(2048 - memory[0xFF18] - 256*(memory[0xFF19] & 0x07));
            amplitude[1] = (memory[0xFF17] & 0xf0) >> 2;
        }
        
        if(memory[0xFF25] & 0x88) { // sound 4 is routed to sound output terminal
            int s = ((memory[0xFF22] & 0xf0) >> 4) + 1;
            int r = (memory[0xFF22] & 0x03) ?: 1; // 1 ist falsch, eigentlich 0.5
            freq[3] = 524288 / r / (1<<s);

            amplitude[3] = (memory[0xFF21] & 0xf0) >> 2;
        }
    }
    
    for(int i = 0; i < 2; ++i)
        if(duty[i] == 0)
            duty[i] = 1;
    
    int T[4] = {SAMPLE_FREQUENCY / freq[0],
                SAMPLE_FREQUENCY / freq[1],
                SAMPLE_FREQUENCY / freq[2],
                SAMPLE_FREQUENCY / freq[3] ?: 1};
    
    int tmp = 0;
    for(int i = 0; i < length; ++i) {
        
        if(amplitude[3] && (i + sound->offset[3])%T[3] == 0) {
            sound->noise_val = (rand()%2) ? -1 : 1;
        }
        
        tmp = (((8*(i + sound->offset[0])/T[0]) % 8) < duty[0] ? amplitude[0] : -amplitude[0])
            + (((8*(i + sound->offset[1])/T[1]) % 8) < duty[1] ? amplitude[1] : -amplitude[1])
            + sound->noise_val * amplitude[3];
        
        if(tmp > 127) tmp = 127;
        else if(tmp < -127) tmp = -127;
            
        stream[i] = tmp;
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
    
    sound->offset[0] = 0;
    sound->offset[1] = 0;
    sound->offset[2] = 0;
    sound->offset[3] = 0;
    sound->memory = memory;
    sound->noise_val = 0;
    
    if(SDL_OpenAudio(&want, &have) != 0)
        return false;
    
    SDL_PauseAudio(0);
    return true;
}

void pause_sound(gb_sound *sound, bool pause) {
    SDL_PauseAudio(pause ? 1 : 0);
}

bool deinit_sound(gb_sound *sound) {
    SDL_PauseAudio(1);
    SDL_CloseAudio();
    return true;
}
