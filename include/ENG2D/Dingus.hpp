#ifndef ENG_DINGUS_HPP
#define ENG_DINGUS_HPP

#include "ENG2D/Window.hpp"
#include "ENG2D/Texture.hpp"
#include "ENG2D/Camera.hpp"
#include "ENG2D/DrawTools.hpp"
#include "ENG2D/Timer.hpp"
#include "ENG2D/Vector.hpp"
#include <vector>
#include "ENG2D/CollisionShape.hpp"
#include <functional>
#include "ENG2D/Stopwatch.hpp"
#include "ENG2D/RunOnce.hpp"
#include "ENG2D/Transform.hpp"

namespace ENG
{

    class Dingus
    {
    private:
        static std::vector<Dingus *> &_instances()
        {
            static std::vector<Dingus *> v;
            return v;
        }

        Vector2<double> _forceSum = {0, 0};

        void _PropagatePhysics()
        {
            _forceSum += velocity.Scale(-damping, true);

            velocity += (_forceSum / mass) * timer->delta * 0.5;
            transform.position += velocity * timer->delta;
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
        Dingus()
        {
            _instances().push_back(this);
        }
        Dingus(Camera *camera) : camera{camera}
        {
            _instances().push_back(this);
        }
        Dingus(Camera *camera, Texture *texture) : camera{camera},
                                                   texture{texture}
        {
            _instances().push_back(this);
        }
        ~Dingus()
        {
            std::vector<Dingus *> &v = _instances();
            v.erase(std::remove(v.begin(), v.end(), this), v.end());
        }

        Camera *camera = nullptr;
        Texture *texture = nullptr;
        //Vector2<double> position = {0, 0};
        Transform2<double> transform;
        Vector2<double> velocity = {0, 0};
        double rotation = 0;
        //double size = 1;
        //Vector2<double> scale = {1, 1};
        Timer *timer = nullptr;
        bool active = true;
        bool physicsEnabled = true;
        double mass = 1;
        double damping = 0;
        bool fenceToWindow = false;
        bool collisionEnabled = false;
        int collisionLayer;
        int renderLayer;
        CollisionShape *collisionShape = nullptr;

        void AssignCollisionShape(CollisionShape *shape)
        {
            collisionShape = shape;
        }
        void AssignTexture(Texture *new_texture)
        {
            texture = new_texture;
        }
        void AssignCamera(Camera *new_camera)
        {
            camera = new_camera;
        }
        void AssignTimer(Timer *new_timer)
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
                DrawTools::DrawTexture(camera, texture, transform.position, transform.size, rotation);
            }

            if (fenceToWindow && camera != nullptr)
            {
                transform.position.x = SDL_clamp(transform.position.x, camera->window->size.x / -2, camera->window->size.x / 2);
                transform.position.y = SDL_clamp(transform.position.y, camera->window->size.y / -2, camera->window->size.y / 2);
            }

            // if (camera != nullptr && collisionShape != nullptr && collisionShape->IfOverlapping(Input::GetMouseWorldPos(camera) - position))
            //{
            //     // hover event
            //     _mouseHoverRunner.OnTrue(collisionShape->IfOverlapping(Input::GetMouseWorldPos(camera) - position));
            //     // click
            //     _mouseDownRunner_R.OnTrue(Input::Right);
            //     _mouseDownRunner_L.OnTrue(Input::Left);
            //     _mouseDownRunner_M.OnTrue(Input::Middle);
            //     _mouseUpRunner_R.OnFalse(Input::Right);
            //     _mouseUpRunner_L.OnFalse(Input::Left);
            //     _mouseUpRunner_M.OnFalse(Input::Middle);
            // }
        }

        double GetDistanceToMouse()
        {
            if (camera == nullptr)
            {
                return -1;
            }
            return Vector2<double>::Distance(transform.position, Input::GetMousePos(camera));
        }

        inline static void UpdateAll()
        {
            for (Dingus *d : _instances())
            {
                if (d->active)
                {
                    d->Update();
                }
            }
        }
    };

};

#endif