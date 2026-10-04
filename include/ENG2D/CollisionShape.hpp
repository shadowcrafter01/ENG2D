#ifndef ENG_COLLISIONSHAPE_HPP
#define ENG_COLLISIONSHAPE_HPP

#include "ENG2D/Vector.hpp"
#include <vector>

namespace ENG
{

    class CollisionShape
    {
    private:
    public:
        CollisionShape()
        {
        }

        enum class Type
        {
            Box,
            Circle,
            Capsule,
            Polygon,
            Segment
        };

        Type type = Type::Box;

        // box collider params
        b2Vec2 halfSize = {0.5f, 0.5f};

        // circle collider params
        float radius = 0.5f;

        // capsule collider params
        b2Vec2 pointA = {-0.5f, 0.0f};
        b2Vec2 pointB = {0.5f, 0.0f};

        // custom collision shape params
        std::vector<b2Vec2> vertices;

        // line collider params
        b2Vec2 segmentA = {-0.5f, 0.0f};
        b2Vec2 segmentB = {0.5f, 0.0f};

        static CollisionShape Box(float width, float height)
        {
            CollisionShape s;
            s.type = Type::Box;
            s.halfSize = {width * 0.5f, height * 0.5f};
            return s;
        }
        static CollisionShape Box(Vector2<float> size)
        {
            CollisionShape s;
            s.type = Type::Box;
            s.halfSize = {size.x * 0.5f, size.y * 0.5f};
            return s;
        }

        static CollisionShape Circle(float radius)
        {
            CollisionShape s;
            s.type = Type::Circle;
            s.radius = radius;
            return s;
        }

        static CollisionShape Capsule(b2Vec2 a, b2Vec2 b, float radius)
        {
            CollisionShape s;
            s.type = Type::Capsule;
            s.pointA = a;
            s.pointB = b;
            s.radius = radius;
            return s;
        }

        static CollisionShape Polygon(std::vector<b2Vec2> points)
        {
            CollisionShape s;
            s.type = Type::Polygon;
            s.vertices = std::move(points);
            return s;
        }

        static CollisionShape Segment(b2Vec2 a, b2Vec2 b)
        {
            CollisionShape s;
            s.type = Type::Segment;
            s.segmentA = a;
            s.segmentB = b;
            return s;
        }
    };
};
#endif