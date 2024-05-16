#include "modelcontroller.h"
#include <QRandomGenerator>
#include <qDebug>

// initialize the model controller and create the world
ModelController::ModelController(QObject *parent)
    : QObject{parent}
{

    // create the world and set the starting level
    world = new b2World(b2Vec2(0.0f, 200.0f));
    currentLevel = 1;

    // setup character and walls
    setAndDrawCharacter();
    setAndDrawWall();

    // start update timer for the world
    connect(&timer, &QTimer::timeout, this, &ModelController::updateWorld);
    timer.start(10);
}

// getter for the box2D world
b2World* ModelController::getWorld()
{
    return world;
}

// set up and draw the walls in the box2D world
void ModelController::setAndDrawWall()
{
    // Define the ground body.
    b2BodyDef groundBodyDef;
    groundBodyDef.position.Set(0.0f, 552.0f);

    // create the body in the world
    b2Body* groundBody = world->CreateBody(&groundBodyDef);

    // Define the ground box shape.
    b2PolygonShape groundBox;

    // The extents are the half-widths of the box.
    groundBox.SetAsBox(1200.0f, 50.0f);

    // Add the ground fixture to the ground body.
    groundBody->CreateFixture(&groundBox, 0.0f);

    // define ceiling
    b2BodyDef ceilingBodyDef;
    ceilingBodyDef.position.Set(0.0f, -30.0f);
    b2Body* ceilingBody = world->CreateBody(&ceilingBodyDef);
    b2PolygonShape ceilingBox;
    ceilingBox.SetAsBox(1200.0f, 5.0f);
    ceilingBody->CreateFixture(&ceilingBox, 0.0f);

    // define walls
    b2BodyDef wallBodyDefL;
    wallBodyDefL.position.Set(-25.0f, 0.0f);
    b2Body* wallBodyL = world->CreateBody(&wallBodyDefL);
    b2PolygonShape wallBoxL;
    wallBoxL.SetAsBox(1.0f, 600.0f);
    wallBodyL->CreateFixture(&wallBoxL, 0.0f);

    b2BodyDef wallBodyDefR;
    wallBodyDefR.position.Set(1190.0f, 500.0f);
    b2Body* wallBodyR = world->CreateBody(&wallBodyDefR);
    b2PolygonShape wallBoxR;
    wallBoxR.SetAsBox(10.0f, 600.0f);
    wallBodyR->CreateFixture(&wallBoxR, 0.0f);

    b2BodyDef answerBoxBodyDef;
    answerBoxBodyDef.position.Set(1000,412);
    b2Body* answerBoxBody = world->CreateBody(&answerBoxBodyDef);
    b2PolygonShape answerBox;
    answerBox.SetAsBox(20.0f, 75.0f);
    answerBoxBody->CreateFixture(&answerBox, 1.0f);
    answerBoxBody->SetUserData(this);

    // add all the walls to a QVector for ease of accessing
    walls.push_back(groundBody);
    walls.push_back(wallBodyL);
    walls.push_back(wallBodyR);
    walls.push_back(groundBody);
    walls.push_back(answerBoxBody);
}

// create and set the player character in the box2D world
void ModelController::setAndDrawCharacter()
{
    c = new Character(world, 500, 50);
}

// create the MathBoxes in the world
void ModelController::setMathBoxes(){

    // make the "correct" boxes from the generated equation
    for(QString temp: equation){
        MathBox mathbox;
        mathbox.init(world, temp);
        boxList.push_back(mathbox);
    }

    // make extra boxes
    for(int i = 0; i < 5; i++){

        quint32 randomInt = QRandomGenerator::global()->bounded(3);

        if(randomInt == 0){ // generate a random symbol

            quint32 rand = QRandomGenerator::global()->bounded(4);

            if(rand == 0){
                MathBox mathbox;
                mathbox.init(world, "+");
                boxList.push_back(mathbox);
            }
            else if(rand == 1){
                MathBox mathbox;
                mathbox.init(world, "-");
                boxList.push_back(mathbox);
            }
            else if(rand == 2){
                MathBox mathbox;
                mathbox.init(world, "*");
                boxList.push_back(mathbox);
            }
            else if(rand == 3){
                MathBox mathbox;
                mathbox.init(world, "/");
                boxList.push_back(mathbox);
            }
        }
        else{ // generate a random number
            quint32 rand = QRandomGenerator::global()->bounded(30);
            // If random number can be same as answer.
            double extraNumber = static_cast<int>(rand);

            if(extraNumber == answer){
                while(extraNumber != answer){
                    rand = QRandomGenerator::global()->bounded(30);
                }
            }

            MathBox mathbox;
            mathbox.init(world, "" + QString::number(rand));
            boxList.push_back(mathbox);
        }
    }
}

// clear all the lists in modelController to reset game state
void ModelController::clearAllLists(){

    for(MathBox temp: boxList){
        world->DestroyBody(temp.getBody());
    }
    //  eqution from mathgenerator
    equation.clear();
    //  equation from user
    currAnswer.clear();
    //  mathbox clear.
    boxList.clear();

    emit clearSub();
}

// get the players velocity
void ModelController::getVelocity(float x, float y)
{
    b2Vec2 v = c->cBody->GetLinearVelocity();
    v.x = x;
    v.y = y;

    c->cBody->SetLinearVelocity(v);
}

// update the world and emit signals accordingly
void ModelController::updateWorld() {
    // It is generally best to keep the time step and iterations fixed.
    world->Step(1.0/60.0, 6, 2);
    emit sendAllList(walls, boxList, c, world);
    emit update();
}

// slot for the player to pick up and drop boxes
void ModelController::pickupDropBoxSlot(){
    if(!holdingBox){
        // trying to pick up
        QVectorIterator<MathBox> boxIter(boxList);
        b2Vec2 charPos = c->cBody->GetPosition();
        for (MathBox thisBox: boxList) {
            b2Body* boxBody = thisBox.getBody();
            b2Vec2 boxPos = boxBody->GetPosition();
            if(abs((charPos.x)-(boxPos.x)) < 80){
                // the character is next to this box so we want to pick it up

                // place the selected box above the character
                boxBody->SetTransform(b2Vec2(charPos.x+40, charPos.y - 50), 0);

                // create a b2WeldJoint anchoring the box above the character
                b2WeldJointDef boxCharWeldJointDef;
                boxCharWeldJointDef.Initialize(c->cBody, boxBody, b2Vec2(charPos.x, charPos.y - 60));
                boxCharWeldJoint = (b2WeldJoint*)world->CreateJoint(&boxCharWeldJointDef);

                holdingBox = true;
                return;
            }
        }
    }
    else{
        // trying to drop a box
        // need to destroy the character box weld joint
           world->DestroyJoint(boxCharWeldJoint);
           holdingBox = false;
    }
}

// get the current level
int ModelController::getLevel(){
    return currentLevel;
}

// set the current level
void ModelController::setLevel(int level){
    currentLevel = level;
}

// set the equation and the answer that the player needs to solve
void ModelController::setEquationAndAnswer(QVector<QString> eq, double ans){
    equation = eq;
    answer = ans;

    setMathBoxes();
    //decimal
    emit sendAnswer(QString::number(answer, 'f',1));
}

// progress to to the next level
void ModelController::levelProgress()
{
    if(currentLevel >= 12)
    {
        clearAllLists();
        emit sendEndGame();
    }
    else
    {
        currentLevel++;
        clearAllLists();
        emit sendGenerateEquation(currentLevel);
        emit sendCurrentLevel(currentLevel);
    }
}

// start the game again
void ModelController::getStartGame()
{
    if(currentLevel != 1)
    {
        setLevel(1);
    }
    clearAllLists();
    emit sendCurrentLevel(1);
    emit sendGenerateEquation(currentLevel);
}

// add a term to the players current answer submission
void ModelController::addTerm(){

    // check for a box to be in the submission zone and if there is one remove it and place in the submission
    for (int i = 0; i < boxList.size(); i++){

        if(boxList[i].getBody()->GetPosition().x >= 1050.0f){
            currAnswer.append(boxList[i].mathTerm);
            world->DestroyBody(boxList[i].getBody());
            emit appendTerm(boxList[i].mathTerm);
            boxList.removeAt(i);
            return;
        }
    }
}

// slot to submit the players answer
void ModelController::submitAnswer(){

    double userAnswer = this->math.calculator(currAnswer);

    // evaluate answer
    if(answer == userAnswer){ // answer submits correctly
        this->levelProgress();
    }
    else{ // answer is wrong

        for(QString term : currAnswer){
            MathBox mathbox;
            mathbox.init(world, term);
            boxList.push_back(mathbox);
        }
        currAnswer.clear();
        emit clearSub();
    }
}
