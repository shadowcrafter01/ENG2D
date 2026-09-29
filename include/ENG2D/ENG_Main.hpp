#ifndef ENG_MAIN_HPP
#define ENG_MAIN_HPP

#include <SDL3/SDL.h>
// #include <SDL3/SDL_main.h>
//#include <SDL3_image/SDL_image.h>

//#include <B2D/include/box2d/box2d.h>

#include <iostream>
#include <functional>

#include "ENG2D/Vector2.hpp"

#include "ENG2D/ENG_Console.hpp"
#include "ENG2D/ENG_Window.hpp"
#include "ENG2D/ENG_Texture.hpp"
#include "ENG2D/ENG_Atlas.hpp"
#include "ENG2D/ENG_Font.hpp"
#include "ENG2D/ENG_Timer.hpp"
#include "ENG2D/ENG_Audio.hpp"
#include "ENG2D/ENG_Input.hpp"
#include "ENG2D/ENG_Camera.hpp"
#include "ENG2D/ENG_File.hpp"
#include "ENG2D/ENG_DrawTools.hpp"
#include "ENG2D/ENG_Pen.hpp"
#include "ENG2D/ENG_CollisionShape.hpp"
#include "ENG2D/ENG_Dingus.hpp"
#include "ENG2D/ENG_Math.hpp"
#include "ENG2D/ENG_Box2DHandler.hpp"

class ENG
{
private:
public:
    ENG(/* args */)
    {
    }

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
        SDL_Event ENG_Event;
        while (SDL_PollEvent(&ENG_Event))
        {
            switch (ENG_Event.type)
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
            ENG_Input::Update(ENG_Event);
        }
    }

    inline static bool Update()
    {
        Event();
        ENG_Window::UpdateAll();
        ENG_Dingus::UpdateAll();
        timer.update();
        draw.textureDrawCount = 0;
        return GAMESTATE;
    }

    inline static void Shutdown()
    {
        SDL_Quit();
        TTF_Quit();
    }

    inline static bool GAMESTATE = false;
    inline static ENG_Timer timer;
    inline static ENG_Console console = ENG_Console(&timer);
    inline static ENG_Input input;
    inline static ENG_DrawTools draw;

    

    inline static ENG_Window CreateWindow(const char *title, Vector2<int> size, SDL_WindowFlags flags)
    {
        return ENG_Window(title, size, flags);
    }
    inline static ENG_Texture CreateTexture(ENG_Window *window, const char *path)
    {
        return ENG_Texture(window ,path);
    }
    inline static ENG_Camera CreateCamera(ENG_Window *window, Vector2<double> position = Vector2<double>(0, 0), double zoom = 1, double angle = 0)
    {
        return ENG_Camera(window, position, zoom, angle);
    }
    inline static ENG_Font CreateFont(const char *path, int point)
    {
        return ENG_Font(path, point);
    }
    inline static ENG_Audio CreateAudio(const char *path)
    {
        return ENG_Audio(path);
    }
    inline static ENG_File CreateFile(const char *path)
    {
        return ENG_File(path);
    }
    inline static ENG_Dingus CreateDingus(ENG_Camera *camera, ENG_Texture *texture)
    {
        ENG_Dingus dingus = ENG_Dingus(camera,texture);
        dingus.AssignTimer(&timer);
        return dingus;
    }
};

#endif