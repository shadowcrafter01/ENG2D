#ifndef ENG_AUDIO_HPP
#define ENG_AUDIO_HPP

#include <SDL3_mixer/SDL_mixer.h>

// #include "ENG_Main.hpp"
#include "ENG_Console.hpp"

class ENG_Audio
{
private:
    static inline MIX_Mixer *mixer;
    static inline bool flag_mixerInit = false;

public:
    ENG_Audio(const char *path)
    {
        if (!flag_mixerInit)
        {
            ENG_Console::LogLoadStart("Initializing mixer");
            if (!MIX_Init())
            {
                ENG_Console::LogLoadEnd(false);
                return;
            }
            mixer = MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, NULL);
            if (mixer == NULL)
            {
                ENG_Console::LogLoadEnd(false);
                return;
            }
            ENG_Console::LogLoadEnd(true);
            flag_mixerInit = true;
        }
        ENG_Console::LogLoadStart((std::string) "Loading audio file [" + path + "]");
        audio = MIX_LoadAudio(mixer, path, false);
        if (audio == NULL)
        {
            ENG_Console::LogLoadEnd(false);
            return;
        }
        ENG_Console::LogLoadEnd(true);
        state = true;
    }

    void Play()
    {
        if (!MIX_PlayAudio(mixer, audio))
        {
            ENG_Console::LogError("Play Audio Fail");
        }
    }

    MIX_Audio *audio;
    bool state = false;
};

#endif