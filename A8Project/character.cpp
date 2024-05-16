#include "character.h"

// constructor to create a character in the world at a given x and y
Character::Character(b2World* w, float x, float y)
{
    // define the characters box2D bodydef
    b2BodyDef cBodyDef;
    cBodyDef.type = b2_dynamicBody;

    // define the characters shape and size in the box2D world
    b2PolygonShape polygonShape;
    polygonShape.SetAsBox(50,50);

    // create a box2D fixture for the characters body
    b2FixtureDef cFixtureDef;
    cFixtureDef.shape = &polygonShape;
    cFixtureDef.density = 1;

    // set the characters position and create them in the world
    cBodyDef.position.Set(x,y);
    cBody = w->CreateBody(&cBodyDef);
    cBody->CreateFixture(&cFixtureDef);
}
