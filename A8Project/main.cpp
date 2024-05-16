
#include "mainwindow.h"
#include "mathgenerator.h"
#include "gameworld.h"
#include <QApplication>

// main method to run the game
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MathGenerator math;
    ModelController model;
    MainWindow w(math, model);
    w.show();
    return a.exec();
}
