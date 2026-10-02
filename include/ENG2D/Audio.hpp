#ifndef ENG_AUDIO_HPP
#define ENG_AUDIO_HPP

#include <SDL3_mixer/SDL_mixer.h>

// #include "Main.hpp"
#include "ENG2D/Console.hpp"

namespace ENG
{

    class Audio
    {
    private:
        static inline MIX_Mixer *mixer;
        static inline bool flag_mixerInit = false;

    public:
        Audio(const char *path)
        {
            if (!flag_mixerInit)
            {
                Console::LogLoadStart("Initializing mixer");
                if (!MIX_Init())
                {
                    Console::LogLoadEnd(false);
                    return;
                }
                mixer = MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, NULL);
                if (mixer == NULL)
                {
                    Console::LogLoadEnd(false);
                    return;
                }
                Console::LogLoadEnd(true);
                flag_mixerInit = true;
            }
            Console::LogLoadStart((std::string) "Loading audio file [" + path + "]");
            audio = MIX_LoadAudio(mixer, path, false);
            if (audio == NULL)
            {
                Console::LogLoadEnd(false);
                return;
            }
            Console::LogLoadEnd(true);
            state = true;
        }

        void Play()
        {
            if (!MIX_PlayAudio(mixer, audio))
            {
                Console::LogError("Play Audio Fail");
            }
        }

        MIX_Audio *audio;
        bool state = false;
    };
};
#endif