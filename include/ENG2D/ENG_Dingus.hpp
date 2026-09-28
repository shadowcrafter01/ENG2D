#ifndef ENG_DINGUS_HPP
#define ENG_DINGUS_HPP

#include "ENG_Window.hpp"
#include "ENG_Texture.hpp"
#include "ENG_Camera.hpp"
#include "ENG_DrawTools.hpp"
#include "ENG_Timer.hpp"
#include "Vector2.hpp"
#include <vector>
#include "ENG_CollisionShape.hpp"
#include <functional>
#include "Stopwatch.hpp"
#include "RunOnce.hpp"

class ENG_Dingus
{
private:
    static std::vector<ENG_Dingus *> &_instances()
    {
        static std::vector<ENG_Dingus *> v;
        return v;
    }

    Vector2<double> _forceSum = {0, 0};

    void _PropagatePhysics()
    {
        _forceSum += velocity.Scale(-damping, true);

        velocity += (_forceSum / mass) * timer->delta * 0.5;
        position += velocity * timer->delta;
        velocity += (_forceSum / mass) * timer->delta * 0.5;
        _forceSum = Vector2<double>(0, 0);
    }

    void _OnClick()
    {
    }

    RunOnce _mouseDownRunner_R;
    RunOnce _mouseDownRunner_L;
    RunOnce _mouseDownRunner_M;
    RunOnce _mouseHoverRunner;
    RunOnce _mouseUpRunner_R;
    RunOnce _mouseUpRunner_L;
    RunOnce _mouseUpRunner_M;

public:
    ENG_Dingus()
    {
        _instances().push_back(this);
    }
    ENG_Dingus(ENG_Camera *camera) : camera{camera}
    {
        _instances().push_back(this);
    }
    ENG_Dingus(ENG_Camera *camera, ENG_Texture *texture) : camera{camera},
                                                           texture{texture}
    {
        _instances().push_back(this);
    }
    ~ENG_Dingus()
    {
        std::vector<ENG_Dingus *> &v = _instances();
        v.erase(std::remove(v.begin(), v.end(), this), v.end());
    }

    ENG_Camera *camera = nullptr;
    ENG_Texture *texture = nullptr;
    Vector2<double> position = {0, 0};
    Vector2<double> velocity = {0, 0};
    double rotation = 0;
    double size = 1;
    Vector2<double> scale = {1, 1};
    ENG_Timer *timer = nullptr;
    bool active = true;
    bool physicsEnabled = true;
    double mass = 1;
    double damping = 0;
    bool fenceToWindow = false;
    bool collisionEnabled = false;
    int collisionLayer;
    int renderLayer;
    ENG_CollisionShape *collisionShape = nullptr;

    void AssignCollisionShape(ENG_CollisionShape *shape)
    {
        collisionShape = shape;
    }
    void AssignTexture(ENG_Texture *new_texture)
    {
        texture = new_texture;
    }
    void AssignCamera(ENG_Camera *new_camera)
    {
        camera = new_camera;
    }
    void AssignTimer(ENG_Timer *new_timer)
    {
        timer = new_timer;
    }
    void AssignClickEvent_R(std::function<void()> f)
    {
        _mouseDownRunner_R.AssignFunction(f);
    }
    void AssignClickEvent_L(std::function<void()> f)
    {
        _mouseDownRunner_L.AssignFunction(f);
    }
    void AssignClickEvent_M(std::function<void()> f)
    {
        _mouseDownRunner_M.AssignFunction(f);
    }
    void AssignHoverEvent(std::function<void()> f)
    {
        _mouseHoverRunner.AssignFunction(f);
    }

    void ApplyForce(Vector2<double> force)
    {
        _forceSum += force; // * timer->delta;
    }

    void Update()
    {
        if (physicsEnabled && timer != nullptr)
        {
            _PropagatePhysics();
        }

        if (texture != NULL && camera != nullptr)
        {
            ENG_DrawTools::DrawTexture(camera, texture, position, size, rotation);
        }

        if (fenceToWindow && camera != nullptr)
        {
            position.x = SDL_clamp(position.x, camera->window->size.x / -2, camera->window->size.x / 2);
            position.y = SDL_clamp(position.y, camera->window->size.y / -2, camera->window->size.y / 2);
        }

        if (camera != nullptr && collisionShape != nullptr && collisionShape->IfOverlapping(ENG_Input::GetMouseWorldPos(camera) - position))
        {
            // hover event
            _mouseHoverRunner.OnTrue(collisionShape->IfOverlapping(ENG_Input::GetMouseWorldPos(camera) - position));
            // click
            _mouseDownRunner_R.OnTrue(ENG_Input::Right);
            _mouseDownRunner_L.OnTrue(ENG_Input::Left);
            _mouseDownRunner_M.OnTrue(ENG_Input::Middle);

            _mouseUpRunner_R.OnFalse(ENG_Input::Right);
            _mouseUpRunner_L.OnFalse(ENG_Input::Left);
            _mouseUpRunner_M.OnFalse(ENG_Input::Middle);
        }
    }

    double GetDistanceToMouse()
    {
        if (camera == nullptr)
        {
            return -1;
        }
        return Vector2<double>::Distance(position, ENG_Input::GetMousePos(camera));
    }

    inline static void UpdateAll()
    {
        for (ENG_Dingus *d : _instances())
        {
            if (d->active)
            {
                d->Update();
            }
        }
    }
};

#endif