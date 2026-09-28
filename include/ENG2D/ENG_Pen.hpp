#ifndef ENG_PEN_HPP
#define ENG_PEN_HPP

#include <ENG_Camera.hpp>
#include <ENG_DrawTools.hpp>
#include "Vector2.hpp"

class ENG_Pen
{
private:
public:
    ENG_Pen(ENG_Camera *camera) : camera{camera}
    {
    }

    void Down()
    {
        isDown = true;
    }
    void Up()
    {
        isDown = false;
    }
    void GoTo(Vector2<double> newPos)
    {
        if (isDown)
        {
            ENG_DrawTools::DrawLine(camera, pos, newPos);
        }
        pos = newPos;
    }

    Vector2<double> pos = {0, 0};
    bool isDown = false;
    ENG_Camera *camera;
};

#endif