#ifndef ENG_MATH_HPP
#define ENG_MATH_HPP

#include <cmath>

namespace ENG
{

    class Math
    {
    public:
        template <typename T>
        inline static T Lerp(T a, T b, float t)
        {
            return (a * (1 - t)) + (b * t);
        }

        template <typename T>
        inline static T rad2deg(T rad)
        {
            return rad * 180.0 / M_PI;
        }

        template <typename T>
        inline static T deg2rad(T deg)
        {
            return deg * M_PI / 180.0;
        }
    };

};

#endif