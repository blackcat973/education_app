#ifndef MATHGENERATOR_H
#define MATHGENERATOR_H
#include <QVector>
#include<QObject>
#include <QWidget>

class MathGenerator : public QObject
{
    Q_OBJECT
public:
    ~MathGenerator();

    explicit MathGenerator(QObject *parent = nullptr);


    QVector<QString> equation;
    QVector<QString> operations;

    void firstEquation();
    void addition();
    void multipleEquation(int equationLength);
    double calculator(QVector<QString> equation);


public slots:
    void formulaGenerator(int level);

signals:
    void setMathEquation(QVector<QString> equation, double answer);
};

#endif // MATHGENERATOR_H
