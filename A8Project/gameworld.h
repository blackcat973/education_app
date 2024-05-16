#ifndef GAMEWORLD_H
#define GAMEWORLD_H

#include <QWidget>
#include <Box2D/Box2D.h>
#include <QTimer>
#include <QVector>
#include "mathbox.h"
#include "Character.h"

class GameWorld : public QWidget
{
    Q_OBJECT
public:
    explicit GameWorld(QWidget *parent = nullptr);
    void paintEvent(QPaintEvent *);
signals:

public slots:
    void updateWorld();
    void getAllList(QVector<b2Body*> wall, QVector<MathBox> b, Character* c, b2World* w);

private:
    void addBox(QString s);
    QPair<float, float> getBodyWidthAndHeight(b2Body* obj);

    b2World* world;
    Character* characterObj;
    QVector<b2Body*> walls;
    QVector<MathBox> boxList;

    QTimer timer;
    QImage background;
    QImage characterR;
    QImage box;
};

#endif // GAMEWORLD_H
