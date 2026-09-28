#ifndef ENG_MATH_HPP
#define ENG_MATH_HPP

class ENG_Math
{
public:
    template <typename T>
    inline static T Lerp(T a, T b, float t)
    {
        return (a * (1 - t)) + (b * t);
    }

};

#endif