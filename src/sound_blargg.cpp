extern "C" {
#include "sound.h"
}

#include <climits>
#include <mutex>
#include "Gb_Apu.h"
#include "Multi_Buffer.h"

struct sound_detail {
    Gb_Apu apu;
    Blip_Buffer buffer;
    std::mutex mut;
};

void sound_reg_write(gb_sound *sound, uint16_t addr, uint8_t val, uint64_t time) {
    std::lock_guard<std::mutex> lock(sound->snd->mut);
    if(addr >= 0xff10 && addr <= 0xff3f)
        sound->snd->apu.write_register(time, addr, val);
}

void audio_callback(void *userdata, uint8_t *stream, int length) {
    gb_sound *sound = (gb_sound*) userdata;
    std::lock_guard<std::mutex> lock(sound->snd->mut);
    uint8_t *memory = sound->memory->mem;
    
    int time = 4194304 * length/sizeof(int16_t) / SAMPLE_FREQUENCY;
    
    for(uint16_t reg = 0xff10; reg <= 0xff3f; ++reg) {
        memory[reg] = sound->snd->apu.read_register(0, reg);
    }
    
    sound->snd->apu.end_frame(time);
    sound->snd->buffer.end_frame(time);
    
    sound->snd->buffer.read_samples((int16_t*)stream, length/sizeof(int16_t));
    sound->snd->buffer.remove_samples(sound->snd->buffer.samples_avail());
}

bool init_sound(gb_sound *sound, gb_memory *memory) {
    if(SDL_InitSubSystem(SDL_INIT_AUDIO) != 0)
        return false;
    
    SDL_AudioSpec want, have;
    
    want.freq = SAMPLE_FREQUENCY;
    want.format = AUDIO_S16;
    want.channels = 1;
    want.samples = 256;
    want.callback = audio_callback;
    want.userdata = (void*) sound;
    
    sound->snd = new sound_detail();
    
//    sound->snd->apu.treble_eq( -20.0 );
//    sound->snd->buffer.bass_freq( 461 );
    sound->snd->apu.volume(0.5);

    sound->snd->apu.output(&sound->snd->buffer);
    sound->snd->buffer.clock_rate( 4194304 );
    sound->snd->buffer.set_sample_rate(SAMPLE_FREQUENCY, 200);
    
    sound->memory = memory;
    
    if(SDL_OpenAudio(&want, &have) != 0)
        return false;
    
    SDL_PauseAudio(0);
    return true;
}

void pause_sound(gb_sound *sound, bool pause) {
    SDL_PauseAudio(pause ? 1 : 0);
}

bool deinit_sound(gb_sound *sound) {
    delete sound->snd;

    SDL_PauseAudio(1);
    SDL_CloseAudio();
    return true;
}
