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
#include "ENG2D/Math.hpp"

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

        void _Update()
        {
            if (_physicsActive)
            {
                b2WorldTransform t = b2Body_GetTransform(_bodyID);
                _transform.position.x = t.p.x;
                _transform.position.y = t.p.y;
                _transform.angle = b2Rot_GetAngle(t.q) * -180.0 / M_PI;
            }
            if (texture != NULL && camera != nullptr)
            {
                DrawTools::DrawTexture(camera, texture, _transform.position, _transform.size, _transform.angle);
            }
            Update();
        }

        b2BodyId _bodyID;
        static inline b2WorldDef _worldDef = b2DefaultWorldDef();
        static inline b2WorldId _worldID = b2CreateWorld(&_worldDef);
        Transform2<float> _transform = Transform2<float>(0, 0, 1, 1, 0);
        CollisionShape collisionShape;

        bool _active = true;
        bool _physicsActive = false;

    public:
        Dingus(Camera *camera, Transform2<float> _transform) : _transform{_transform}
        {
            _instances().push_back(this);
        }
        Dingus(Camera *camera, Vector2<float> pos = Vector2<float>(0, 0), float angle = 0, Vector2<float> size = Vector2<float>(0, 0)) : camera{camera}
        {
            _transform.position = pos;
            _transform.size = size;
            _transform.angle = angle;
            _instances().push_back(this);
        }
        Dingus(Camera *camera, Texture *texture, Transform2<float> _transform) : camera{camera},
                                                                                 texture{texture},
                                                                                 _transform{_transform}
        {
            _instances().push_back(this);
        }
        Dingus(Camera *camera, Texture *texture, Vector2<float> pos = Vector2<float>(0, 0), float angle = 0, Vector2<float> size = Vector2<float>(0, 0)) : camera{camera},
                                                                                                                                                           texture{texture}
        {
            if (size = Vector2<float>(0, 0))
            {
                _transform.size = texture->size;
            }
            else
            {
                _transform.size = size;
            }
            _transform.position = pos;
            _transform.angle = angle;
            _instances().push_back(this);
        }
        ~Dingus()
        {
            std::vector<Dingus *> &v = _instances();
            v.erase(std::remove(v.begin(), v.end(), this), v.end());
        }
        Dingus(const Dingus&) = delete;
        Dingus& operator=(const Dingus&) = delete;

        Camera *camera = nullptr;
        Texture *texture = nullptr;

        // assign functions
        Dingus *AssignCollisionShape(CollisionShape collider, float density = 1.0f, float friction = 0.25f, float restitution = 0.1f)
        {
            if (_physicsActive)
            {
                return this;
            }

            collisionShape = collider;

            b2BodyDef bodyDef = b2DefaultBodyDef();
            bodyDef.type = b2_dynamicBody;
            bodyDef.position = _transform.position;
            _bodyID = b2CreateBody(_worldID, &bodyDef);
            b2ShapeDef shapeDef = b2DefaultShapeDef();
            shapeDef.density = density;
            shapeDef.material.friction = friction;
            shapeDef.material.restitution = restitution;

            switch (collisionShape.type)
            {
            case CollisionShape::Type::Box:
            {
                b2Polygon polygon = b2MakeBox(collisionShape.halfSize.x, collisionShape.halfSize.y);
                b2CreatePolygonShape(_bodyID, &shapeDef, &polygon);
                break;
            }
            case CollisionShape::Type::Circle:
            {
                b2Circle circle = {{0.0f, 0.0f}, collisionShape.radius};
                b2CreateCircleShape(_bodyID, &shapeDef, &circle);
                break;
            }
            case CollisionShape::Type::Capsule:
            {
                b2Capsule capsule = {collisionShape.pointA, collisionShape.pointB, collisionShape.radius};
                b2CreateCapsuleShape(_bodyID, &shapeDef, &capsule);
                break;
            }
            case CollisionShape::Type::Polygon:
            {
                if (collisionShape.vertices.size() < 3)
                {
                    b2DestroyBody(_bodyID);
                    _bodyID = b2_nullBodyId;
                    throw std::invalid_argument("Polygon needs at least 3 vertices");
                }
                b2Hull hull = b2ComputeHull(collisionShape.vertices.data(), static_cast<int>(collisionShape.vertices.size()));
                if (hull.count < 3)
                {
                    b2DestroyBody(_bodyID);
                    _bodyID = b2_nullBodyId;
                    throw std::invalid_argument("Invalid polygon geometry");
                }
                b2Polygon polygon = b2MakePolygon(&hull, 0.0f);
                b2CreatePolygonShape(_bodyID, &shapeDef, &polygon);
                break;
            }
            case CollisionShape::Type::Segment:
            {
                b2Segment segment = {collisionShape.segmentA, collisionShape.segmentB};
                b2CreateSegmentShape(_bodyID, &shapeDef, &segment);
                break;
            }
            }
            _physicsActive = true;

            return this;
        }

        // get functions
        Transform2<float> GetTransform()
        {
            return _transform;
        }
        b2BodyId *GetBodyID()
        {
            if (_physicsActive)
            {
                return &_bodyID;
            }
            else
            {
                return nullptr;
            }
        }
        Vector2<float> GetVelocity()
        {
            return Vector2<float>::B2D_to_ENG(b2Body_GetLinearVelocity(_bodyID));
        }

        // set functions
        Dingus *SetTransform(Transform2<float> t)
        {
            if (_physicsActive)
            {
                b2Body_SetTransform(_bodyID, t, t);
                _transform.size = t.size;
            }
            else
            {
                _transform = t;
            }
            return this;
        }
        Dingus *SetPosition(Vector2<float> p)
        {
            if (_physicsActive)
            {
                b2Body_SetTransform(_bodyID, p, _transform);
            }
            else
            {
                _transform.position = p;
            }
            return this;
        }
        Dingus *SetPosition(float x, float y)
        {
            if (_physicsActive)
            {
                b2Body_SetTransform(_bodyID, (b2Vec2){x, y}, _transform);
            }
            else
            {
                _transform.position.x = x;
                _transform.position.y = y;
            }
            return this;
        }
        Dingus *SetAngle(float a)
        {
            if (_physicsActive)
            {
                b2Body_SetTransform(_bodyID, _transform, b2MakeRot(Math::deg2rad(a)));
            }
            else
            {
                _transform.angle = a;
            }
            return this;
        }
        Dingus *SetStatic(bool isStatic = true)
        {
            if (_physicsActive)
            {
                b2Body_SetType(_bodyID, (isStatic) ? b2_staticBody : b2_dynamicBody);
            }
            return this;
        }
        Dingus *SetVelocity(float u, float v)
        {
            b2Body_SetLinearVelocity(_bodyID, {u, v});
            return this;
        }
        Dingus *SetVelocity(Vector2<float> v)
        {
            b2Body_SetLinearVelocity(_bodyID, v);
            return this;
        }

        // apply functions
        void ApplyForce(Vector2<float> f)
        {
            if (_physicsActive)
            {
                b2Body_ApplyForceToCenter(_bodyID, f, true);
            }
            else
            {
            }
        }
        void ApplyForceAt(Vector2<float> f, Vector2<float> p)
        {
            if (_physicsActive)
            {
                b2Body_ApplyForce(_bodyID, f, p, true);
            }
            else
            {
            }
        }
        void ApplyImpulse(Vector2<float> i)
        {
            if (_physicsActive)
            {
                b2Body_ApplyLinearImpulseToCenter(_bodyID, i, true);
            }
            else
            {
            }
        }
        void ApplyImpulseAt(Vector2<float> i, Vector2<float> p)
        {
            if (_physicsActive)
            {
                b2Body_ApplyLinearImpulse(_bodyID, i, p, true);
            }
            else
            {
            }
        }

        // virtual update function that you override (if you want) in any class that uses Dingus as a parent class
        virtual void Update()
        {
        }

        // update all function called by the engine, no touchie
        inline static void UpdateAll()
        {
            b2World_Step(_worldID, 1.0 / 60.0, 4);
            for (Dingus *d : _instances())
            {
                if (d->_active)
                {
                    d->_Update();
                }
            }
        }
    };
};

#endif