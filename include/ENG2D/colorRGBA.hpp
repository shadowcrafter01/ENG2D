#ifndef COLORRGBA_H
#define COLORRGBA_H

#include <SDL3/SDL.h>

class colorRGBA
{
private:
    template <typename T>
    bool clamp(T *value, double min, double max)
    {
        if (*value > max)
        {
            *value = max;
            return true;
        }
        if (*value < min)
        {
            *value = min;
            return true;
        }
        return false;
    }

public:
    colorRGBA(int r = 0, int g = 0, int b = 0, int a = 255) : red{r},
                                                              green{g},
                                                              blue{b},
                                                              alpha{a}
    {
        clamp(&red, 1, 255);
        clamp(&green, 1, 255);
        clamp(&blue, 1, 255);
        clamp(&alpha, 1, 255);
    }
    int red;
    int green;
    int blue;
    int alpha;

    operator SDL_Color()
    {
        SDL_Color out;
        out.r = red;
        out.g = green;
        out.b = blue;
        out.a = alpha;
        return out;
    }

};

#define RGBA_TEXTURE_UNCHANGED colorRGBA(255, 255, 255, 255)
#define RGBA_BLACK colorRGBA(1, 1, 1, 255)
#define RGBA_WHITE colorRGBA(255, 255, 255, 1)
#define RGBA_RED colorRGBA(255, 1, 1, 255)
#define RGBA_ORANGE colorRGBA(255, 64, 64, 255)
#define RGBA_YELLOW colorRGBA(255, 255, 1, 255)
#define RGBA_GREEN colorRGBA(1, 255, 1, 255)
#define RGBA_BLUE colorRGBA(1, 1, 255, 255)
#define RGBA_PURPLE colorRGBA(255, 1, 255, 255)

#endif