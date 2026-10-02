#ifndef ENG_BOX2DHANDLER_HPP
#define ENG_BOX2DHANDLER_HPP

#include <box2d/base.h>
#include <box2d/box2d.h>

#include "ENG2D/Vector.hpp"
#include "ENG2D/CollisionShape.hpp"

namespace ENG
{

    class Box2DHandler
    {
    private:
        /* data */
    public:
        Box2DHandler(/* args */);

        inline static b2BodyId CreateBox(b2WorldId world, Vector2<float> position, float w, float h, bool fixed, float density = 1.0f, float friction = 0.1f)
        {
            b2BodyDef bodyDef = b2DefaultBodyDef();
            bodyDef.type = (fixed) ? b2_staticBody : b2_dynamicBody;
            bodyDef.position = (b2Vec2){position.x, position.y};

            b2BodyId bodyId = b2CreateBody(world, &bodyDef);

            b2Polygon dynamicBox = b2MakeBox(w, h);
            b2ShapeDef shapeDef = b2DefaultShapeDef();

            shapeDef.density = density;
            shapeDef.material.friction = friction;

            b2CreatePolygonShape(bodyId, &shapeDef, &dynamicBox);

            return bodyId;
        }
        inline static b2BodyId CreateCircle(b2WorldId world, Vector2<float> position, float r, bool fixed, float density = 1.0f, float friction = 0.1f)
        {
            b2BodyDef bodyDef = b2DefaultBodyDef();
            bodyDef.type = (fixed) ? b2_staticBody : b2_dynamicBody;
            bodyDef.position = (b2Vec2){position.x, position.y};

            b2BodyId bodyId = b2CreateBody(world, &bodyDef);

            b2ShapeDef shapeDef = b2DefaultShapeDef();

            shapeDef.density = density;
            shapeDef.material.friction = friction;

            b2Circle circle;
            circle.radius = r;

            b2CreateCircleShape(bodyId, &shapeDef, &circle);

            return bodyId;
        }
    };
};
#endif