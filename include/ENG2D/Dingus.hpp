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

#include <memory>
#include <utility>
#include <algorithm>

namespace ENG
{

    class Dingus
    {
    private:
        // static std::vector<std::unique_ptr<Dingus *>> _instances()
        //{
        //     static std::vector<std::unique_ptr<Dingus *>> v;
        //     return v;
        // }
        inline static std::vector<std::unique_ptr<Dingus>> _instances;

        void _Update()
        {
            if (_physicsActive)
            {
                b2WorldTransform t = b2Body_GetTransform(_bodyID);
                _transform.position.x = t.p.x;
                _transform.position.y = t.p.y;
                _transform.angle = b2Rot_GetAngle(t.q) * -180.0 / M_PI;
            }
            if (_texture != NULL && _camera != nullptr)
            {
                DrawTools::DrawTexture(_camera, _texture, _transform.position, _transform.size, _transform.angle);
            }
            Update();
        }

        inline static size_t _zeroIndex = 0;
        static void _UpdateZeroIndex()
        {
            auto i = std::lower_bound(
                _instances.begin(),
                _instances.end(),
                0.0f,
                [](const std::unique_ptr<Dingus> &object, float layer)
                {
                    return object->_renderLayer < layer;
                });

            _zeroIndex = static_cast<size_t>(std::distance(_instances.begin(), i));
        }

        static void _ResortInstances()
        {
            std::stable_sort(_instances.begin(),
                             _instances.end(),
                             [](const std::unique_ptr<Dingus> &a, const std::unique_ptr<Dingus> &b)
                             {
                                 return a->_renderLayer < b->_renderLayer;
                             });
        }

        b2BodyId _bodyID;
        static inline b2WorldDef _worldDef = b2DefaultWorldDef();
        static inline b2WorldId _worldID = b2CreateWorld(&_worldDef);
        Transform2<float> _transform = Transform2<float>(0, 0, 1, 1, 0);
        CollisionShape collisionShape;
        Camera *_camera = nullptr;
        Texture *_texture = nullptr;
        float _renderLayer = 0;

        bool _active = true;
        bool _physicsActive = false;

        // constructors
        Dingus(Camera *camera, Transform2<float> _transform) : _transform{_transform}
        {
        }
        Dingus(Camera *camera, Vector2<float> pos = Vector2<float>(0, 0), float angle = 0, Vector2<float> size = Vector2<float>(0, 0)) : _camera{camera}
        {
            _transform.position = pos;
            _transform.size = size;
            _transform.angle = angle;
        }
        Dingus(Camera *camera, Texture *texture, Transform2<float> _transform) : _camera{camera},
                                                                                 _texture{texture},
                                                                                 _transform{_transform}
        {
        }
        Dingus(Camera *camera, Texture *texture, Vector2<float> pos = Vector2<float>(0, 0), float angle = 0, Vector2<float> size = Vector2<float>(0, 0)) : _camera{camera},
                                                                                                                                                           _texture{texture}
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
        }

    public:
        static Dingus *Create(Camera *camera, Transform2<float> transform)
        {
            auto dngs = std::unique_ptr<Dingus>(new Dingus(camera, transform));
            dngs->_renderLayer = 0.0f;
            Dingus *ptr = dngs.get();
            _instances.insert(_instances.begin() + _zeroIndex, std::move(dngs));
            ++_zeroIndex;
            return ptr;
        }
        static Dingus *Create(Camera *camera, Vector2<float> pos = Vector2<float>(0, 0), float angle = 0, Vector2<float> size = Vector2<float>(0, 0))
        {
            auto dngs = std::unique_ptr<Dingus>(new Dingus(camera, pos, angle, size));
            dngs->_renderLayer = 0.0f;
            Dingus *ptr = dngs.get();
            _instances.insert(_instances.begin() + _zeroIndex, std::move(dngs));
            ++_zeroIndex;
            return ptr;
        }
        static Dingus *Create(Camera *camera, Texture *texture, Transform2<float> transform)
        {
            auto dngs = std::unique_ptr<Dingus>(new Dingus(camera, texture, transform));
            dngs->_renderLayer = 0.0f;
            Dingus *ptr = dngs.get();
            _instances.insert(_instances.begin() + _zeroIndex, std::move(dngs));
            ++_zeroIndex;
            return ptr;
        }
        static Dingus *Create(Camera *camera, Texture *texture, Vector2<float> pos = Vector2<float>(0, 0), float angle = 0, Vector2<float> size = Vector2<float>(0, 0))
        {
            auto dngs = std::unique_ptr<Dingus>(new Dingus(camera, texture, pos, angle, size));
            dngs->_renderLayer = 0.0f;
            Dingus *ptr = dngs.get();
            _instances.insert(_instances.begin() + _zeroIndex, std::move(dngs));
            ++_zeroIndex;
            return ptr;
        }
        ~Dingus()
        {
            if (_physicsActive)
            {
                b2DestroyBody(_bodyID);
                _bodyID = b2_nullBodyId;
            }
        }
        Dingus(const Dingus &) = delete;
        Dingus &operator=(const Dingus &) = delete;
        static void Destroy(Dingus *dngs)
        {
            for (auto i = _instances.begin(); i != _instances.end(); ++i)
            {
                if (i->get() == dngs)
                {
                    _instances.erase(i);
                    return;
                }
            }
        }
        void Destroy()
        {
            for (auto i = _instances.begin(); i != _instances.end(); ++i)
            {
                if (i->get() == this)
                {
                    _instances.erase(i);
                    return;
                }
            }
            _ResortInstances();
        }
        static void DestroyAll()
        {
            _instances.clear();
        }

        // assign functions
        Dingus *AssignCollisionShape(CollisionShape collider, float density = 1.0f, float friction = 0.25f, float restitution = 0.1f)
        {
            if (this == nullptr)
            {
                return nullptr;
            }
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
        Dingus *AssignTexture(Texture *tex)
        {
            if (this == nullptr)
            {
                return nullptr;
            }
            _texture = tex;
            return this;
        }
        Dingus *AssignCamera(Camera *cam)
        {
            if (this == nullptr)
            {
                return nullptr;
            }
            _camera = cam;
            return this;
        }

        // get functions
        Transform2<float> GetTransform()
        {
            if (this == nullptr)
            {
                return Transform2<float>();
            }
            return _transform;
        }
        Vector2<float> GetPosition()
        {
            if (this == nullptr)
            {
                return Vector2<float>();
            }
            return _transform.position;
        }
        b2BodyId *GetBodyID()
        {
            if (this == nullptr)
            {
                return nullptr;
            }
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
            if (this == nullptr)
            {
                return Vector2<float>();
            }
            return Vector2<float>::B2D_to_ENG(b2Body_GetLinearVelocity(_bodyID));
        }
        Texture *GetTexture()
        {
            if (this == nullptr)
            {
                return nullptr;
            }
            return _texture;
        }
        Camera *GetCamera()
        {
            if (this == nullptr)
            {
                return nullptr;
            }
            return _camera;
        }
        float GetRenderLayer()
        {
            if (this == nullptr)
            {
                return 0;
            }
            return _renderLayer;
        }
        static Dingus *GetNearest(Vector2<float> position)
        {
            Dingus *nearestDingus = nullptr;
            float nearestDistance = std::numeric_limits<float>::max();

            for (const auto &dngs : _instances)
            {
                float dist = Vector2<float>::Distance(position, dngs->GetPosition());
                if (dist < nearestDistance)
                {
                    nearestDistance = dist;
                    nearestDingus = dngs.get();
                }
            }
            return nearestDingus;
        }
        static const std::vector<std::unique_ptr<Dingus>> &GetAllInstances()
        {
            return _instances;
        }
        static b2WorldId GetWorld()
        {
            return _worldID;
        }

        // set functions
        Dingus *SetTransform(Transform2<float> t)
        {
            if (this == nullptr)
            {
                return nullptr;
            }
            if (_physicsActive)
            {
                b2Body_SetTransform(_bodyID, t, t);
                _transform.size = t.size;
                b2Body_SetAwake(_bodyID, true);
            }
            else
            {
                _transform = t;
            }
            return this;
        }
        Dingus *SetPosition(Vector2<float> p)
        {
            if (this == nullptr)
            {
                return nullptr;
            }
            if (_physicsActive)
            {
                b2Body_SetTransform(_bodyID, p, _transform);
                b2Body_SetAwake(_bodyID, true);
            }
            else
            {
                _transform.position = p;
            }
            return this;
        }
        Dingus *SetPosition(float x, float y)
        {
            if (this == nullptr)
            {
                return nullptr;
            }
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
        Dingus *SetAngle(float deg)
        {
            if (this == nullptr)
            {
                return nullptr;
            }
            if (_physicsActive)
            {
                b2Body_SetTransform(_bodyID, _transform, b2MakeRot(Math::deg2rad(deg)));
            }
            else
            {
                _transform.angle = deg;
            }
            return this;
        }
        Dingus *SetStatic(bool isStatic = true)
        {
            if (this == nullptr)
            {
                return nullptr;
            }
            if (_physicsActive)
            {
                b2Body_SetType(_bodyID, (isStatic) ? b2_staticBody : b2_dynamicBody);
            }
            return this;
        }
        Dingus *SetVelocity(float u, float v)
        {
            if (this == nullptr)
            {
                return nullptr;
            }
            b2Body_SetLinearVelocity(_bodyID, {u, v});
            return this;
        }
        Dingus *SetVelocity(Vector2<float> v)
        {
            if (this == nullptr)
            {
                return nullptr;
            }
            b2Body_SetLinearVelocity(_bodyID, v);
            return this;
        }
        Dingus *SetRenderLayer(float layer)
        {
            if (this == nullptr)
            {
                return nullptr;
            }
            if (_renderLayer == layer)
            {
                return this;
            }
            _renderLayer = layer;
            _ResortInstances();
        }
        static void SetWorldGravity(Vector2<float> v)
        {
            b2World_SetGravity(_worldID, v);
        }

        // apply functions
        Dingus *ApplyForce(Vector2<float> f)
        {
            if (this == nullptr)
            {
                return nullptr;
            }
            if (_physicsActive)
            {
                b2Body_ApplyForceToCenter(_bodyID, f, true);
            }
            else
            {
            }
            return this;
        }
        Dingus *ApplyForceAt(Vector2<float> f, Vector2<float> p)
        {
            if (this == nullptr)
            {
                return nullptr;
            }
            if (_physicsActive)
            {
                b2Body_ApplyForce(_bodyID, f, p, true);
            }
            else
            {
            }
            return this;
        }
        Dingus *ApplyImpulse(Vector2<float> i)
        {
            if (this == nullptr)
            {
                return nullptr;
            }
            if (_physicsActive)
            {
                b2Body_ApplyLinearImpulseToCenter(_bodyID, i, true);
            }
            else
            {
            }
            return this;
        }
        Dingus *ApplyImpulseAt(Vector2<float> i, Vector2<float> p)
        {
            if (this == nullptr)
            {
                return nullptr;
            }
            if (_physicsActive)
            {
                b2Body_ApplyLinearImpulse(_bodyID, i, p, true);
            }
            else
            {
            }
            return this;
        }
        Dingus *ApplyTorque(float t)
        {
            if (this == nullptr)
            {
                return nullptr;
            }
            b2Body_ApplyTorque(_bodyID, t, true);
            return this;
        }

        // virtual update function that you override (if you want) in any class that uses Dingus as a parent class
        virtual void Update()
        {
        }

        // update all function called by the engine, no touchie
        inline static void UpdateAll()
        {
            b2World_Step(_worldID, 1.0 / 60.0, 4);
            for (auto i = _instances.begin(); i != _instances.end(); ++i)
            {
                if (i->get()->_active)
                {
                    i->get()->_Update();
                }
            }
        }
    };
};

#endif