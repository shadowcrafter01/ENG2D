#ifndef ENG_ATLAS_HPP
#define ENG_ATLAS_HPP

#include "ENG_Texture.hpp"

class ENG_Atlas
{
private:
    /* data */
public:
    ENG_Atlas(ENG_Window *window, const char *path)
    {
        texture = ENG_Texture(window, path);

        ENG_Console::LogInfo((std::string) "Assigned Texture [" + path + "] to atlas", " -LOAD : ");
    }

    ENG_Texture texture;
    SDL_FRect rect;
};

#endif