#ifndef ENG_TRANSFORM2_HPP
#define ENG_TRANSFORM2_HPP

#include "ENG2D/Texture.hpp"

namespace ENG
{
    template <class T = double>
    class Transform2
    {
    private:
        /* data */
    public:
        Transform2(T x = 0, T y = 0, T w = 1, T h = 1) : position{Vector2<T>(x, y)},
                                                         size{Vector2<T>(w, h)}
        {
        }

        Vector2<T> position;
        Vector2<T> size;
    };
};
#endif