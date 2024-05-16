#ifndef MATHBOX_H
#define MATHBOX_H
#include <Box2D/Box2D.h>
#include <QObject>


class MathBox
{
public:
    MathBox();
    ~MathBox();
    QString mathTerm;

    void init(b2World* world,QString expression);
    b2Body* getBody();
    b2Fixture* getFixture();
    QString* getExpression();

private:
    b2Body* body = nullptr;
    b2Fixture* fixture = nullptr;
};

#endif // MATHBOX_H
