#ifndef ENG_MAIN_HPP
#define ENG_MAIN_HPP

#include <SDL3/SDL.h>
// #include <SDL3/SDL_main.h>
// #include <SDL3_image/SDL_image.h>

// #include <B2D/include/box2d/box2d.h>

#include <iostream>
#include <functional>

#include "ENG2D/Vector.hpp"

#include "ENG2D/Console.hpp"
#include "ENG2D/Window.hpp"
#include "ENG2D/Texture.hpp"
#include "ENG2D/Atlas.hpp"
#include "ENG2D/Font.hpp"
#include "ENG2D/Timer.hpp"
#include "ENG2D/Audio.hpp"
#include "ENG2D/Input.hpp"
#include "ENG2D/Camera.hpp"
#include "ENG2D/File.hpp"
#include "ENG2D/DrawTools.hpp"
#include "ENG2D/Pen.hpp"
#include "ENG2D/CollisionShape.hpp"
#include "ENG2D/Dingus.hpp"
#include "ENG2D/Math.hpp"
#include "ENG2D/Transform.hpp"

namespace ENG
{

    // class Engine
    //{
    // private:
    // public:
    //     Engine(/* args */)
    //     {
    //     }

    bool GAMESTATE = false;
    Timer timer;
    Console console = Console(&timer);
    bool hasCustomCursor = false;
    Texture *cursor;
    Vector2<float> cursorScale;
    // inline Input input;
    // inline DrawTools draw;

    bool Init(const char *appname = "c++ project", const char *appversion = "0.0.0", const char *appidentifier = "com.name.engine")
    {
        bool flag = false;
        console.LogLoadStart("Initializing engine");
        // metadata
        if (!SDL_SetAppMetadata(appname, appversion, appidentifier))
        {
            flag = true;
        }

        // init sdl
        if (!SDL_Init(SDL_INIT_VIDEO))
        {
            console.LogLoadEnd(false, "Error initializing SDL3");
            return false;
        }

        if (flag)
        {
            console.LogLoadEnd(true, (std::string) "Non-fatal errors encountered -> " + SDL_GetError());
        }
        else
        {
            console.LogLoadEnd(true, "Engine started!");
        }
        GAMESTATE = true;
        return true;
    }

    void Event()
    {
        SDL_Event Event;
        while (SDL_PollEvent(&Event))
        {
            switch (Event.type)
            {
            case SDL_EVENT_QUIT:
                GAMESTATE = false;
                break;
            case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
                GAMESTATE = false;
                break;
            default:
                break;
            }
            Input::Update(Event);
        }
    }

    bool Update()
    {
        Event();
        if (hasCustomCursor)
        {
            DrawTools::DrawTexture(cursor, Input::GetMousePos(), 1.0f, cursorScale);
        }
        Window::UpdateAll();
        Dingus::UpdateAll();
        timer.update();
        DrawTools::textureDrawCount = 0;
        return GAMESTATE;
    }

    void Shutdown()
    {
        Dingus::DestroyAll();
        b2DestroyWorld(Dingus::GetWorld());
        SDL_Quit();
        TTF_Quit();
    }

    Window CreateWindow(const char *title, Vector2<int> size, SDL_WindowFlags flags)
    {
        return Window(title, size, flags);
    }
    Texture CreateTexture(Window *window, const char *path)
    {
        return Texture(window, path);
    }
    Camera CreateCamera(Window *window, Vector2<double> position = Vector2<double>(0, 0), double zoom = 1, double angle = 0)
    {
        return Camera(window, position, zoom, angle);
    }
    Font CreateFont(const char *path, int point)
    {
        return Font(path, point);
    }
    Audio CreateAudio(const char *path)
    {
        return Audio(path);
    }
    File CreateFile(const char *path)
    {
        return File(path);
    }
    // inline Dingus CreateDingus(Camera *camera, Texture *texture)
    //{
    //     Dingus dingus = Dingus(camera, texture);
    //     return dingus;
    // }
    // };

    // hides the OS cursor and displays provided texture instead
    //

    /// Hides OS cursor and displays provided texture at the mouse position instead
    /// the custom image is rendered last, so always on top of everything else
    /// also the texture draws at the center of the image so use that to control the "hotspot"
    void SetCustomCursor(Texture *tex, Vector2<float> scale = Vector2<float>(1, 1))
    {
        SDL_HideCursor();
        cursor = tex;
        cursorScale = scale;
        hasCustomCursor = true;
    }

};

#endif