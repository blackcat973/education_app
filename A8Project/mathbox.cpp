#include "mathbox.h"
#include <QRandomGenerator>
#include <QDebug>

// empty constructor and destructor
MathBox::MathBox(){
}
MathBox::~MathBox(){
}

// method to intialize a MathBox object, which contains a box2D body and a QString to represent the math term drawn in the box
void MathBox::init(b2World* world, QString expression){

        // randomly generate an X and Y offset so that all the boxes dont spawn on top of eachother
        quint32 randOffsetX = QRandomGenerator::global()->bounded(400);
        quint32 randOffsetY = QRandomGenerator::global()->bounded(50);

        // define the boxy of a box and set its position
        b2BodyDef newBody;
        newBody.type = b2_dynamicBody;
        newBody.position.Set(200.0f+randOffsetX, 300.0f+randOffsetY);

        // set a variety of box properties and create it in the world
        newBody.fixedRotation = true;
        body = world->CreateBody(&newBody);
        b2PolygonShape dynamicBox;
        dynamicBox.SetAsBox(25.0f,25.0f);
        b2FixtureDef boxFixture;
        boxFixture.shape = &dynamicBox;
        boxFixture.density = 1.0f;
        boxFixture.friction = 0.7f;
        boxFixture.restitution = 0.9;
        fixture = body->CreateFixture(&boxFixture);
        b2MassData massData;
        massData.mass = 0.5f;
        body->SetMassData(&massData);

        // set the math term in the box
        mathTerm = expression;

}

// get a reference to the box2D body object
b2Body* MathBox::getBody(){
    return body;
}

// get a reference to the box2D fixture object
b2Fixture* MathBox::getFixture(){
    return fixture;
}

// get the math term stored in the box
QString* MathBox::getExpression(){
    QString* data = (QString*) fixture->GetUserData();
    return data;
}
