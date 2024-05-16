#ifndef MODELCONTROLLER_H
#define MODELCONTROLLER_H

#include <QObject>
#include <Box2D/Box2D.h>
#include <QTimer>
#include <QKeyEvent>

#include "character.h"
#include <mathgenerator.h>
#include "mathbox.h"

class ModelController : public QObject
{
    Q_OBJECT
public:
    explicit ModelController(QObject *parent = nullptr);
    b2World* getWorld();
    b2World* world;
    Character* c;

    int currentLevel;
    QVector<QString> equation;
    QVector<QString> currAnswer;
    double answer;
    QVector<MathBox> boxList;
    MathGenerator math;

    int getLevel();
    void setLevel(int level);
public slots:
    void getStartGame();
    void updateWorld();
    void setEquationAndAnswer(QVector<QString> equation, double answer);
    void getVelocity(float x, float y);
    void pickupDropBoxSlot();
    void submitAnswer();
    void addTerm();
signals:
    void update();
    void sendGenerateEquation(int level);
    void sendAnswer(QString ans);
    void appendTerm(QString term);
    void clearSub();
    void sendCurrentLevel(int level);
    void sendEndGame();
    void sendAllList(QVector<b2Body*> wall, QVector<MathBox> b, Character* c, b2World* w);
private:
    void keyboardPressed(QKeyEvent * event);
    void keyboardReleased(QKeyEvent * event);
    void setAndDrawWall();
    void setAndDrawCharacter();
    void setAndDrawBox();
    void setMathBoxes();
    void clearAllLists();
    void collisionEvent();
    void levelProgress();

    float dir = 0.0;
    QTimer timer;
    QVector<b2Body*> walls;
    QVector<b2Body*> boxes;
    b2WeldJoint* boxCharWeldJoint;
    b2Body* answerbox;
    bool holdingBox = false;
};
#endif // MODELCONTROLLER_H
