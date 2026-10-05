#ifndef ENG_TEXTURE_HPP
#define ENG_TEXTURE_HPP

#include "ENG2D/Window.hpp"
#include "ENG2D/Vector.hpp"
#include "ENG2D/Console.hpp"
#include <SDL3_image/SDL_image.h>
#include "ENG2D/Font.hpp"
#include "ENG2D/Camera.hpp"

namespace ENG
{

    class Texture
    {
    private:
        struct texture
        {
            /* data */
        };

    public:
        Texture()
        {
        }
        Texture(Window *window, const char *path) : path{path},
                                                    window{window}
        {
            Console::LogLoadStart((std::string) "Loading texture [" + path + "]");

            surface = IMG_Load(path);
            if (surface == NULL)
            {
                Console::LogLoadEnd(false);
                return;
            }
            size = Vector2<int>(surface->w, surface->h);
            pointer = SDL_CreateTextureFromSurface(window->renderer, surface);
            if (pointer == NULL)
            {
                Console::LogLoadEnd(false);
                return;
            }
            Console::LogLoadEnd(true);
            state = true;
        }

        // SDL_Renderer *renderer;
        Window *window;
        const char *path;
        SDL_Texture *pointer;
        SDL_Surface *surface;
        bool state = false;
        Vector2<int> size;

        operator SDL_Texture *()
        {
            return pointer;
        }
        operator SDL_Surface *()
        {
            return surface;
        }
        template <typename T>
        operator Vector2<T>()
        {
            return Vector2<T>((T)size.x, (T)size.y);
        }
    };

};

#endif