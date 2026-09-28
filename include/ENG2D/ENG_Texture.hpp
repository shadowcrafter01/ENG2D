#ifndef ENG_TEXTURE_HPP
#define ENG_TEXTURE_HPP

#include "ENG_Window.hpp"
#include "Vector2.hpp"
#include "ENG_Console.hpp"
#include <SDL3_image/SDL_image.h>
#include "ENG_Font.hpp"
#include "ENG_Camera.hpp"

class ENG_Texture
{
private:
    struct texture
    {
        /* data */
    };

public:
    ENG_Texture()
    {
    }
    ENG_Texture(ENG_Window *window, const char *path) : path{path},
                                                        renderer{window->renderer.pointer}
    {
        ENG_Console::LogLoadStart((std::string) "Loading texture [" + path + "]");

        surface = IMG_Load(path);
        if (surface == NULL)
        {
            ENG_Console::LogLoadEnd(false);
            return;
        }
        size = Vector2<int>(surface->w, surface->h);
        pointer = SDL_CreateTextureFromSurface(window->renderer, surface);
        if (pointer == NULL)
        {
            ENG_Console::LogLoadEnd(false);
            return;
        }
        ENG_Console::LogLoadEnd(true);
        state = true;
    }

    SDL_Renderer *renderer;
    const char *path;
    SDL_Texture *pointer;
    SDL_Surface *surface;
    bool state = false;
    Vector2<int> size;
};

#endif