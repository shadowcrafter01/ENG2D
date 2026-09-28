#ifndef ENG_WINDOW_HPP
#define ENG_WINDOW_HPP

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3_image/SDL_image.h>
#include "Vector2.hpp"
#include <vector>
#include "ENG_Console.hpp"
#include <string>

class ENG_Window
{
private:
    struct ENG_Renderer
    {
        SDL_Renderer *pointer;
        operator SDL_Renderer *() const
        {
            return pointer;
        }
    };

    static std::vector<ENG_Window *> &instances()
    {
        static std::vector<ENG_Window *> v;
        return v;
    }

public:
    ENG_Window(const char *title, Vector2<int> size, SDL_WindowFlags flags) : title{title},
                                                                              size{size},
                                                                              flags{flags}
    {
        ENG_Console::LogLoadStart((std::string) "Creating window [" + title + "]");
        if (!SDL_CreateWindowAndRenderer(title, size.x, size.y, flags, &pointer, &renderer.pointer))
        {
            ENG_Console::LogLoadEnd(false);
            return;
        }
        if (!SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0))
        {
            ENG_Console::LogLoadEnd(false);
            return;
        }
        ttfEngine = TTF_CreateRendererTextEngine(renderer);
        if (ttfEngine == NULL)
        {
            ENG_Console::LogLoadEnd(false);
        }
        SDL_SetDefaultTextureScaleMode(renderer, SDL_SCALEMODE_PIXELART);
        instances().push_back(this);
        ENG_Console::LogLoadEnd(true);
    }
    //    ~ENG_Window()
    //    {
    //        //std::vector<ENG_Window*> &v = instances();
    //        if (renderer.pointer) SDL_DestroyRenderer(renderer);
    //        if (pointer) SDL_DestroyWindow(pointer);
    //    }

    // ENG_Window(const ENG_Window&) = delete;
    // ENG_Window& operator=(const ENG_Window&) = delete;

    SDL_Window *pointer;
    ENG_Renderer renderer;
    TTF_TextEngine *ttfEngine;

    const char *title;
    Vector2<int> size;
    Vector2<int> center;
    SDL_WindowFlags flags;

    void SetIcon(const char *path)
    {
        SDL_Surface *surface = IMG_Load(path);
        if (surface == NULL)
        {
            ENG_Console::LogError((std::string) "Error loading texture file [" + path + "] for window [" + title + "] icon");
            return;
        }
        int width = surface->w;
        int height = surface->h;
        if (!SDL_SetWindowIcon(pointer, surface))
        {
            ENG_Console::LogError((std::string) "Error applying texture file [" + path + "] to window [" + title + "] icon");
        }
    }
    void Update()
    {
        SDL_SetRenderTarget(renderer, NULL);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_SetRenderScale(renderer, 1, 1);
        SDL_GetWindowSizeInPixels(pointer, &size.x, &size.y);
        center = Vector2<int>(size.x / 2, size.y / 2);
        SDL_RenderPresent(renderer);
        SDL_RenderClear(renderer);
    }
    static void UpdateAll()
    {
        for (ENG_Window *w : instances())
        {
            w->Update();
        }
    }
    static std::vector<ENG_Window *> &GetAllWindows()
    {
        return instances();
    }
};

#endif