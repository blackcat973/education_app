#include "mathgenerator.h"
#include "QDebug"
#include <QtGlobal>
#include <QRandomGenerator>
#include <QJSEngine>
#include <cmath>

// initialize a MathGenerator, making a QVector that stores possible math operators for later randomization
MathGenerator::MathGenerator(QObject *parent) : QObject{parent}
{
    operations.push_back("+");
    operations.push_back("-");
    operations.push_back("*");
    operations.push_back("/");
}

// basic destructor
MathGenerator::~MathGenerator(){
}

// method to decide which type of formula needs to be generated based on the current level
void MathGenerator::formulaGenerator(int level){

    // for the first 5 levels, generate a simple equation
    if(level >= 1 && level <= 5){
        firstEquation();
    }
    // for the next 5 levels, generate something for difficult
    else if(level >= 5 && level <= 10){
        multipleEquation(5);
    }
    // for the last level, generate the hardest equation
    else{
        multipleEquation(7);
    }
}

// generate a math equation and answer for the first levels of the game
void MathGenerator::firstEquation(){
    // reset the equation and set up random generators
    equation.clear();
    quint32 randomInt = QRandomGenerator::global()->bounded(4);
    QString randomSeed = this->operations.at(randomInt);

    // if the problem was randomly determined to have division, create an equation with division
    if(randomSeed == "/"){
        quint32 v1 = QRandomGenerator::global()->bounded(1,10);
        quint32 v2 = QRandomGenerator::global()->bounded(1,10);
        quint32 v3 = v1*v2;
        equation.push_back( QString::number(v3));
        equation.push_back( randomSeed);
        equation.push_back( QString::number(v1));

    }
    // if there is no division, generate an equation in a simpler way
    else{
        quint32 v1 = QRandomGenerator::global()->bounded(30);
        quint32 v2 = QRandomGenerator::global()->bounded(30);

        equation.push_back(QString::number(v1));
        equation.push_back(randomSeed);
        equation.push_back(QString::number(v2));
    }

    double answer = this->calculator(equation);

    // used to check for situations where a divide by 0 occurs
    if(std::isnan(answer)){
        qDebug() << "NOoo";
    }else{
        emit setMathEquation(equation, answer);
    }
}

// generate a longer math equation based on the current difficulty level
void MathGenerator::multipleEquation(int equationLength){
    //reset equation
    equation.clear();

    // generate a longer equation
    for(int i=0; i<equationLength; i++){
        if(i%2 != 0 & i != equationLength){
            equation.push_back(this->operations.at(QRandomGenerator::global()->bounded(4)));
        }else{
            equation.push_back(QString::number(QRandomGenerator::global()->bounded(1,10)));
        }
    }
    // uncomment the line below to cheat and be able to see answers beforehand
    //qDebug() << equation;

    double answer = this->calculator(equation);

    // check for divide by 0
    if(std::isnan(answer)){
        qDebug() << "NO";
    }else{
        emit setMathEquation(equation, answer);
    }
}

// calculate the users submitted equation
double MathGenerator::calculator(QVector<QString> equation){

    //qDebug() << equation;
    QString answer = "";
    for(int i=0; i<equation.size(); i++){
        answer.append(QString(equation.at(i)));
    }
    QJSEngine engine;
    return engine.evaluate(answer).toNumber();
}




