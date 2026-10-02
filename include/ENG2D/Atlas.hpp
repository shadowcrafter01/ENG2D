#ifndef ENG_ATLAS_HPP
#define ENG_ATLAS_HPP

#include "ENG2D/Texture.hpp"

namespace ENG
{
    class Atlas
    {
    private:
        /* data */
    public:
        Atlas(Window *window, const char *path)
        {
            texture = Texture(window, path);

            Console::LogInfo((std::string) "Assigned Texture [" + path + "] to atlas", " -LOAD : ");
        }

        Texture texture;
        SDL_FRect rect;
    };
};
#endif