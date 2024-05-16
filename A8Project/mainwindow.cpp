#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSignalMapper>


// set up the mainwindow and connect signals+slots
MainWindow::MainWindow(MathGenerator& math,ModelController& model,
                       QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    // setup the starting window
    ui->setupUi(this);
    this->installEventFilter(this);
    ui->gameWidget->hide();
    ui->endWidget->hide();
    ui->helpWidget->hide();


    // ============ MENU WIDGET ============
    // Start game
    connect(ui->startButton,
                &QPushButton::clicked,
                this,
                &MainWindow::openGameWidget);
    // Quit game
    connect(ui->quitButton, SIGNAL(clicked()), this, SLOT(close()));

    // ============ IN GAME MENU WIDGET ============
    connect(ui->backToMenuButton,
            &QPushButton::clicked,
            this,
            &MainWindow::openFirstGameMenuWidget);
    connect(ui->restartGameButton,
            &QPushButton::clicked,
            &model,
            &ModelController::getStartGame);
    connect(ui->submitButton,
            &QPushButton::clicked,
            &model,
            &ModelController::submitAnswer);
    connect(ui->addTermButton,
            &QPushButton::clicked,
            &model,
            &ModelController::addTerm);
    connect(ui->startButton_2,
                &QPushButton::clicked,
                this,
                &MainWindow::openGameWidget);
    connect(ui->backToMenuButton_2,
            &QPushButton::clicked,
            this,
            &MainWindow::openFirstGameMenuWidget);
    connect(ui->helpButton,
            &QPushButton::clicked,
            this,
            &MainWindow::openHelpMenuWidget);
    connect(ui->backToMenuFromHelp,
            &QPushButton::clicked,
            this,
            &MainWindow::openFirstGameMenuWidget);
    connect(ui->quitButton_2, SIGNAL(clicked()), this, SLOT(close()));

    // ============ FROM MAIN WINDOW ============
    connect(this,
            &MainWindow::sendKeyPressed,
            &model,
            &ModelController::getVelocity);
    connect(this,
            &MainWindow::sendKeyReleased,
            &model,
            &ModelController::getVelocity);
    connect(this,
            &MainWindow::sendQKeyPressed,
            &model,
            &ModelController::pickupDropBoxSlot);
    connect(this,
            &MainWindow::sendStartGame,
            &model,
            &ModelController::getStartGame);

    // ============ FROM MODEL CONTROLLER ============
    connect(&model,
            &ModelController::update,
            ui->gameWidget,
            &GameWorld::updateWorld);
    connect(&model,
            &ModelController::sendGenerateEquation,
            &math,
            &MathGenerator::formulaGenerator);
    connect(&model,
            &ModelController::sendAllList,
            ui->gameWidget,
            &GameWorld::getAllList);
    connect(&model,
            &ModelController::sendAnswer,
            this,
            &MainWindow::setAnswer);
    connect(&model,
            &ModelController::appendTerm,
            this,
            &MainWindow::addTerm);
    connect(&model,
            &ModelController::clearSub,
            this,
            &MainWindow::clearSubmission);
    connect(&model,
            &ModelController::sendEndGame,
            this,
            &MainWindow::showEndMenu);
    connect(&model,
            &ModelController::sendCurrentLevel,
            this,
            &MainWindow::setCurrentLevel);

    // ============ FROM MATH TO MODEL============
//    setMathEquation
    connect(&math,
            &MathGenerator::setMathEquation,
            &model,
            &ModelController::setEquationAndAnswer);
}

// destructor for MainWindow
MainWindow::~MainWindow()
{
    delete ui;
}

// slot to open the widget used to play the game
void MainWindow::openGameWidget()
{
    this->resize(1200,590);
    ui->gameWidget->show();
    ui->gameWidget->focusWidget();
    ui->gameWidget->grabKeyboard();
    ui->menuWidget->hide();
    ui->endWidget->hide();
    ui->helpWidget->hide();
    emit sendStartGame();
    hasControl = true;
}

// slot to open the games menu widget
void MainWindow::openFirstGameMenuWidget()
{
    this->resize(800, 600);
    ui->gameWidget->close();
    ui->endWidget->hide();
    ui->menuWidget->show();
    ui->helpWidget->hide();
    hasControl = false;
}

// slot used for controlling character movement and action
bool MainWindow::eventFilter(QObject *obj, QEvent* event)
{
    // if the player is currently allowed control, let them
    if (hasControl == true)
    {
        float xVel = 0.0;
        float yVel = 0.0;

        // if a player has just pressed or released a button, react accordingly
        if(event->type() == QEvent::KeyPress)
        {
            pressedKeys += ((QKeyEvent*)event)->key();
            if(pressedKeys.contains(Qt::Key_Right) && pressedKeys.contains(Qt::Key_Up))
            {
                xVel = spd;
                yVel = -(spd*2);
            }
            else if(pressedKeys.contains(Qt::Key_Left) && pressedKeys.contains(Qt::Key_Up))
            {
                xVel = -spd;
                yVel = -(spd*2);
            }
            else if(pressedKeys.contains(Qt::Key_Right))
            {
                xVel = spd;
            }
            else if(pressedKeys.contains(Qt::Key_Left))
            {
                xVel = -spd;
            }
            else if(pressedKeys.contains(Qt::Key_Up))
            {
                yVel = -(spd*2);
            }
            else if(pressedKeys.contains(Qt::Key_Q))
            {
                emit sendQKeyPressed();
            }
            emit sendKeyPressed(xVel,yVel);
        }
        else if(event->type() == QEvent::KeyRelease)
        {
            pressedKeys -= ((QKeyEvent*)event)->key();
        }
    }
    return false;
}

// slot to set the current answer for the player to solve
void MainWindow::setAnswer(QString ans)
{
    ui->answer->setText(ans);
}

// slot to set the current level of the game
void MainWindow::setCurrentLevel(int level)
{
    QString text = "Level " + QString::number(level) + "/12";
    ui->currentLevel->setText(text);
}

// slot to add a term to the players current attempt
void MainWindow::addTerm(QString term)
{
    ui->submitButton->setText(ui->submitButton->text() + term + " ");
}

// slot to clear the players current submission attempt
void MainWindow::clearSubmission()
{
    ui->submitButton->setText("");
}

// slot to show the end of game menu
void MainWindow::showEndMenu()
{
    ui->endWidget->show();
}

// slot to open the help menu
void MainWindow::openHelpMenuWidget()
{
    ui->helpWidget->show();
}
