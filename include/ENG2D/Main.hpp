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
#include "ENG2D/Box2DHandler.hpp"

namespace ENG
{

    // class Engine
    //{
    // private:
    // public:
    //     Engine(/* args */)
    //     {
    //     }

    inline static bool GAMESTATE = false;
    inline static Timer timer;
    inline static Console console = Console(&timer);
    inline static Input input;
    inline static DrawTools draw;

    inline static bool Init(const char *appname = "c++ project", const char *appversion = "0.0.0", const char *appidentifier = "com.name.engine")
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

    inline static void Event()
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

    inline static bool Update()
    {
        Event();
        Window::UpdateAll();
        Dingus::UpdateAll();
        timer.update();
        draw.textureDrawCount = 0;
        return GAMESTATE;
    }

    inline static void Shutdown()
    {
        SDL_Quit();
        TTF_Quit();
    }

    inline static Window CreateWindow(const char *title, Vector2<int> size, SDL_WindowFlags flags)
    {
        return Window(title, size, flags);
    }
    inline static Texture CreateTexture(Window *window, const char *path)
    {
        return Texture(window, path);
    }
    inline static Camera CreateCamera(Window *window, Vector2<double> position = Vector2<double>(0, 0), double zoom = 1, double angle = 0)
    {
        return Camera(window, position, zoom, angle);
    }
    inline static Font CreateFont(const char *path, int point)
    {
        return Font(path, point);
    }
    inline static Audio CreateAudio(const char *path)
    {
        return Audio(path);
    }
    inline static File CreateFile(const char *path)
    {
        return File(path);
    }
    inline static Dingus CreateDingus(Camera *camera, Texture *texture)
    {
        Dingus dingus = Dingus(camera, texture);
        dingus.AssignTimer(&timer);
        return dingus;
    }
    //};

};

#endif