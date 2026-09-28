#ifndef ENG_BOX2DHANDLER_HPP
#define ENG_BOX2DHANDLER_HPP

#include <box2d/base.h>
#include <box2d/box2d.h>

#include "Vector2.hpp"
#include "ENG_CollisionShape.hpp"

class ENG_Box2DHandler
{
private:
    /* data */
public:
    ENG_Box2DHandler(/* args */);

    inline static b2BodyId CreateBox(b2WorldId world, Vector2<double> position, double w, double h, bool fixed, double density = 1, double friction = 0.1)
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
    inline static b2BodyId CreateCircle(b2WorldId world, Vector2<double> position, double r, bool fixed, double density = 1, double friction = 0.1)
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

#endif