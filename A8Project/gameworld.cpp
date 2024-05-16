#include "gameworld.h"

#include <QPainter>
#include <QDebug>
#include <QVector>

// default construct the GameWorld, initializing images to their locations
GameWorld::GameWorld(QWidget *parent) : QWidget(parent),
    characterR(":/sprites/characterR.png"),
    background(":/background/background.png"),
    box(":/sprites/box.png")
{
}

void GameWorld::getAllList(QVector<b2Body*> wall, QVector<MathBox> b, Character* c, b2World* w){
    world = w;
    characterObj = c;
    walls = wall;
    boxList = b;
}

// event to draw the current world state
void GameWorld::paintEvent(QPaintEvent *) {

    // Create a painter and draw the background
    QPainter painter(this);
    painter.drawImage(0, 0, background);

    // draw the ground box and answer entering box
    painter.drawRect(0, 527, 3000, 200);
    painter.drawRect(1005, 362, 40, 165);

    // draw the mathBoxes that the player interacts with
    for(unsigned int i = 0; i < boxList.size(); i++){
        b2Vec2 position = boxList[i].getBody()->GetPosition();
        painter.drawImage((int)(position.x), (int)(position.y), box);
        painter.drawRect((int)(position.x), (int)(position.y),50,50);
        painter.drawText((int)(position.x)+20, (int)(position.y)+28, boxList[i].mathTerm);
    }

    // draw the player character
    painter.drawImage(characterObj->cBody->GetPosition().x-25, characterObj->cBody->GetPosition().y-25, characterR);

    painter.end();
}

// event to update the world
void GameWorld::updateWorld() {
    update();
}
