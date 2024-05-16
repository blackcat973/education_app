#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTime>
#include <QDialog>
#include <QSet>
#include "mathgenerator.h"
#include "modelcontroller.h"
#include "gameworld.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    MainWindow(MathGenerator& math,ModelController& model, QWidget *parent = nullptr);
    ~MainWindow();

signals:
    void sendKeyPressed(float x, float y);
    void sendKeyReleased(float x, float y);
    void sendQKeyPressed();
    void sendStartGame();

public slots:
    void openGameWidget();
    void openFirstGameMenuWidget();
    void openHelpMenuWidget();
    bool eventFilter(QObject* obj, QEvent* event);
    void setAnswer(QString ans);
    void addTerm(QString term);
    void clearSubmission();
    void setCurrentLevel(int level);
    void showEndMenu();

private:
    Ui::MainWindow *ui;
    QSet<int> pressedKeys;
    bool hasControl = false;
    float spd = 250.f;
};
#endif // MAINWINDOW_H
