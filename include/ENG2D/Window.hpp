#ifndef ENG_WINDOW_HPP
#define ENG_WINDOW_HPP

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3_image/SDL_image.h>
#include "ENG2D/Vector.hpp"
#include <vector>
#include "ENG2D/Console.hpp"
#include <string>

namespace ENG
{

    class Window
    {
    private:
        struct Renderer
        {
            SDL_Renderer *pointer;
            operator SDL_Renderer *() const
            {
                return pointer;
            }
        };

        static std::vector<Window *> &instances()
        {
            static std::vector<Window *> v;
            return v;
        }

    public:
        Window(const char *title, Vector2<int> size, SDL_WindowFlags flags) : title{title},
                                                                              size{size},
                                                                              flags{flags}
        {
            Console::LogLoadStart((std::string) "Creating window [" + title + "]");
            if (!SDL_CreateWindowAndRenderer(title, size.x, size.y, flags, &pointer, &renderer.pointer))
            {
                Console::LogLoadEnd(false);
                return;
            }
            if (!SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0))
            {
                Console::LogLoadEnd(false);
                return;
            }
            ttfEngine = TTF_CreateRendererTextEngine(renderer);
            if (ttfEngine == NULL)
            {
                Console::LogLoadEnd(false);
            }
            SDL_SetDefaultTextureScaleMode(renderer, SDL_SCALEMODE_PIXELART);
            instances().push_back(this);
            Console::LogLoadEnd(true);
        }
        //    ~Window()
        //    {
        //        //std::vector<Window*> &v = instances();
        //        if (renderer.pointer) SDL_DestroyRenderer(renderer);
        //        if (pointer) SDL_DestroyWindow(pointer);
        //    }

        // Window(const Window&) = delete;
        // Window& operator=(const Window&) = delete;

        SDL_Window *pointer;
        Renderer renderer;
        TTF_TextEngine *ttfEngine;
        SDL_Texture *cursor;
        Vector2<int> cursorSize = Vector2<int>(0, 0);
        Vector2<float> cursorScale = Vector2<int>(1, 1);
        bool flag_hasCustomCursor = false;
        const char *title;
        Vector2<int> size;
        Vector2<int> center;
        SDL_WindowFlags flags;

        void SetIcon(const char *path)
        {
            SDL_Surface *surface = IMG_Load(path);
            if (surface == NULL)
            {
                Console::LogError((std::string) "Error loading texture file [" + path + "] for window [" + title + "] icon");
                return;
            }
            int width = surface->w;
            int height = surface->h;
            if (!SDL_SetWindowIcon(pointer, surface))
            {
                Console::LogError((std::string) "Error applying texture file [" + path + "] to window [" + title + "] icon");
            }
        }
        void SetCustomCursor(const char *path)
        {
            SDL_Surface *surface = IMG_Load(path);
            cursor = SDL_CreateTextureFromSurface(renderer, surface);
            if (cursor == NULL)
            {
                return;
            }
            cursorSize = {surface->w, surface->h};
            if (!flag_hasCustomCursor)
            {
                SDL_HideCursor();
                flag_hasCustomCursor = true;
            }
        }
        void SetCustomCursor(SDL_Texture *texture, Vector2<int> size)
        {
            cursor = texture;
            cursorSize = size;
            if (!flag_hasCustomCursor)
            {
                SDL_HideCursor();
                flag_hasCustomCursor = true;
            }
        }
        void SetCustomCursor(SDL_Surface *surface)
        {
            cursor = SDL_CreateTextureFromSurface(renderer, surface);
            if (cursor == NULL)
            {
                return;
            }
            cursorSize = {surface->w, surface->h};
            if (!flag_hasCustomCursor)
            {
                SDL_HideCursor();
                flag_hasCustomCursor = true;
            }
        }
        void Update()
        {
            if (flag_hasCustomCursor)
            {
                float x, y;
                SDL_GetMouseState(&x, &y);
                SDL_FRect r;
                r.w = cursorSize.x * cursorScale.x;
                r.h = cursorSize.y * cursorScale.y;
                r.x = x - (r.w / 2);
                r.y = y - (r.h / 2);
                SDL_RenderTexture(renderer, cursor, NULL, &r);
            }
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
            for (Window *w : instances())
            {
                w->Update();
            }
        }
        static std::vector<Window *> &GetAllWindows()
        {
            return instances();
        }
    };

};

#endif