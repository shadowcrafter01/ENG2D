#ifndef ENG_CAMERA_HPP
#define ENG_CAMERA_HPP

#include "ENG_Window.hpp"
#include "ENG_Timer.hpp"

class ENG_Camera
{
public:
    ENG_Camera(ENG_Window *window, Vector2<double> position = Vector2<double>(0, 0), double zoom = 1, double angle = 0) : window{window},
                                                                                                                          position{position},
                                                                                                                          zoom{zoom},
                                                                                                                          angle{angle}
    {
    }
    Vector2<double> position;
    double zoom;
    double angle;
    ENG_Window *window;
    
};

#endif