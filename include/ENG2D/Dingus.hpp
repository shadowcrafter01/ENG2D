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
#include <box2d/base.h>
#include <box2d/box2d.h>

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

        void _Init()
        {
        }

        // Vector2<double> _forceSum = {0, 0};

        void _PropagatePhysics()
        {
            //_forceSum += velocity.Scale(-damping, true);
            // velocity += (_forceSum / mass) * timer->delta * 0.5;
            // transform.position += velocity * timer->delta;
            // velocity += (_forceSum / mass) * timer->delta * 0.5;
            //_forceSum = Vector2<double>(0, 0);
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
            _Init();
            _instances().push_back(this);
        }
        Dingus(Camera *camera) : camera{camera}
        {
            _Init();
            _instances().push_back(this);
        }
        Dingus(Camera *camera, Texture *texture) : camera{camera},
                                                   texture{texture}
        {
            _Init();
            _instances().push_back(this);
        }
        ~Dingus()
        {
            std::vector<Dingus *> &v = _instances();
            v.erase(std::remove(v.begin(), v.end(), this), v.end());
        }
        // Dingus(const Dingus&) = delete;
        // Dingus& operator=(const Dingus&) = delete;

        Camera *camera = nullptr;
        Texture *texture = nullptr;
        // Vector2<double> position = {0, 0};
        Transform2<double> transform;
        // Vector2<double> velocity = {0, 0};
        // double rotation = 0;
        // double size = 1;
        // Vector2<double> scale = {1, 1};
        Timer *timer = nullptr;
        // double mass = 1;
        // double damping = 0;
        // int collisionLayer;
        // int renderLayer;
        CollisionShape *collisionShape = nullptr;

        struct Flags
        {
            bool fenceToWindow = false;
            bool collisionEnabled = false;
            bool active = true;
            bool physicsEnabled = true;
        };
        Flags flags;

        struct B2D_Container
        {
            b2BodyDef def;
            b2BodyId ID;
            b2Polygon polygon;
            b2ShapeDef shape;
            static inline b2WorldDef worldDef = b2DefaultWorldDef();
            static inline b2WorldId worldID = b2CreateWorld(&worldDef);
        };
        B2D_Container B2D;

        void SetPosition(Vector2<double> pos)
        {
            b2Body_SetTransform(B2D.ID, pos, b2Body_GetRotation(B2D.ID));
        }
        void SetRotation(double angle)
        {
            b2Body_SetTransform(B2D.ID, b2Body_GetPosition(B2D.ID), b2MakeRot(angle * 180 / M_PI));
        }
        Vector2<double> GetPosition()
        {
            b2Pos pos = b2Body_GetPosition(B2D.ID);
            return Vector2<double>(pos.x, pos.y);
        }
        double GetAngle(bool degrees = true)
        {
            if (degrees)
            {
                return b2Rot_GetAngle(b2Body_GetRotation(B2D.ID)) * 180 / M_PI;
            }
            else
            {
                return b2Rot_GetAngle(b2Body_GetRotation(B2D.ID));
            }
        }
        void SetState(bool dynamic)
        {
            if (dynamic)
            {
                b2Body_SetType(B2D.ID, b2_dynamicBody);
            }
            else
            {
                b2Body_SetType(B2D.ID, b2_staticBody);
            }
        }

        void ApplyForce(Vector2<double> force)
        {
            b2Body_ApplyForceToCenter(B2D.ID, force, true);
        }
        void ApplyForceAt(Vector2<double> force, Vector2<double> pos)
        {
            b2Body_ApplyForce(B2D.ID, force, pos, true);
        }

        void AssignPhysicsBody()
        {
            B2D.def = b2DefaultBodyDef();
            B2D.def.type = b2_dynamicBody;
            B2D.def.position = {0, 0};

            B2D.ID = b2CreateBody(B2D_Container::worldID, &B2D.def);

            if (texture != NULL)
            {
                B2D.polygon = b2MakeBox(texture->size.x * 0.5f, texture->size.y * 0.5f);
            }
            B2D.shape = b2DefaultShapeDef();
            B2D.shape.density = 1.0f;
            B2D.shape.material.friction = 0.3f;
            B2D.shape.material.restitution = 0.1f;

            b2CreatePolygonShape(B2D.ID, &B2D.shape, &B2D.polygon);
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
            //_forceSum += force; // * timer->delta;
        }

        void Update()
        {
            if (flags.physicsEnabled && timer != nullptr)
            {
                _PropagatePhysics();
            }
            b2WorldTransform t = b2Body_GetTransform(B2D.ID);
            transform.position.x = t.p.x;
            transform.position.y = t.p.y;
            transform.angle = b2Rot_GetAngle(t.q) * -180.0 / M_PI;

            if (texture != NULL && camera != nullptr)
            {
                DrawTools::DrawTexture(camera, texture, transform.position, transform.size, transform.angle);
            }

            if (flags.fenceToWindow && camera != nullptr)
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
            b2World_Step(B2D_Container::worldID, 1.0f / 60.0f, 4);
            for (Dingus *d : _instances())
            {
                if (d->flags.active)
                {
                    d->Update();
                }
            }
        }
    };

};

#endif