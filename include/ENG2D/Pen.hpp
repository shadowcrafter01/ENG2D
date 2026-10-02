#ifndef ENG_PEN_HPP
#define ENG_PEN_HPP

#include <ENG2D/Camera.hpp>
#include <ENG2D/DrawTools.hpp>
#include "Vector.hpp"

namespace ENG
{

    class Pen
    {
    private:
    public:
        Pen(Camera *camera) : camera{camera}
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
                DrawTools::DrawLine(camera, pos, newPos);
            }
            pos = newPos;
        }

        Vector2<double> pos = {0, 0};
        bool isDown = false;
        Camera *camera;
    };

};

#endif