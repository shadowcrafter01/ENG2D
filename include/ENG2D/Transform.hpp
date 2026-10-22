#ifndef ENG_TRANSFORM2_HPP
#define ENG_TRANSFORM2_HPP

#include "ENG2D/Texture.hpp"
#include <box2d/box2d.h>
#include "ENG2D/Math.hpp"

namespace ENG
{
    template <class T = double>
    class Transform2
    {
    private:
        /* data */
    public:
        Transform2(T x = 0, T y = 0, T w = 1, T h = 1, T a = 0) : position{Vector2<T>(x, y)},
                                                                  size{Vector2<T>(w, h)},
                                                                  angle{a}
        {
        }
        Transform2(Vector2<T> pos, Vector2<T> size, T angle = 0) : position{pos},
                                                                        size{size},
                                                                        angle{angle}
        {
        }
        Transform2(Vector2<T> pos, T w, T h, T angle = 0) : position{pos},
                                                                 size{Vector2<T>(w, h)},
                                                                 angle{angle}
        {
        }
        Transform2(T x, T y, Vector2<T> size, T angle = 0) : position{Vector2<T>(x, y)},
                                                                  size{size},
                                                                  angle{angle}
        {
        }

        bool ContainsPoint(Vector2<T> point)
        {
            T xMax = position.x + (size.x / (T)2.0);
            T yMax = position.y + (size.y / (T)2.0);
            T xMin = position.x - (size.x / (T)2.0);
            T yMin = position.y - (size.y / (T)2.0);
            return (point.x <= xMax && point.x >= xMin) && (point.y <= yMax && point.y >= yMin);
        }

        Vector2<T> position;
        Vector2<T> size;
        T angle;

        operator b2Rot()
        {
            return b2MakeRot(Math::deg2rad(angle));
        }
        operator b2Vec2()
        {
            return position;
        }
    };
};
#endif