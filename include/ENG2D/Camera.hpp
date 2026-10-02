#ifndef ENG_CAMERA_HPP
#define ENG_CAMERA_HPP

#include "ENG2D/Window.hpp"
#include "ENG2D/Timer.hpp"

namespace ENG
{

    class Camera
    {
    public:
        Camera(Window *window, Vector2<double> position = Vector2<double>(0, 0), double zoom = 1, double angle = 0) : window{window},
                                                                                                                      position{position},
                                                                                                                      zoom{zoom},
                                                                                                                      angle{angle}
        {
        }
        Vector2<double> position;
        double zoom;
        double angle;
        Window *window;
    };
};
#endif