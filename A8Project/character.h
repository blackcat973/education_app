#ifndef CHARACTER_H
#define CHARACTER_H

#include <QVector>
#include <QPair>
#include <Box2D/Box2D.h>

class Character
{
public:
    Character(b2World* w, float x, float y);

    b2Body* cBody;
private:
};

#endif // CHARACTER_H
