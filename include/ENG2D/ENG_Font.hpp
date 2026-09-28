#ifndef ENG_FONT_HPP
#define ENG_FONT_HPP

#include <SDL3_ttf/SDL_ttf.h>
#include <string>

#include "ENG_Console.hpp"
#include "ENG_Texture.hpp"

class ENG_Font
{
private:
    static inline bool flag_ttfInit = false;

public:
    ENG_Font(std::string path, int point) : path{path},
                                            point{point}
    {
        if (!flag_ttfInit)
        {
            ENG_Console::LogLoadStart("Initializing SDL_TTF");
            if (!TTF_Init())
            {
                ENG_Console::LogLoadEnd(false);
                return;
            }
            ENG_Console::LogLoadEnd(true);
            flag_ttfInit = true;
        }
        ENG_Console::LogLoadStart("Loading TTF font [" + path + "]");
        font = TTF_OpenFont(path.c_str(), point);
        if (font == NULL)
        {
            ENG_Console::LogLoadEnd(false);
            return;
        }
        ENG_Console::LogLoadEnd(true);
        state = true;
    }
    TTF_Text *MakeText(std::string text, ENG_Window *window)
    {
        TTF_Text *textOut = TTF_CreateText(window->ttfEngine, font, text.c_str(), 0);
        TTF_SetTextColor(textOut, 255, 255, 255, SDL_ALPHA_OPAQUE);
        return textOut;
    }
    TTF_Font *font;
    std::string path;
    int point;
    bool state = false;
};

#endif