#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QActionGroup>
#include <QMessageBox>
#include <QPixmap>
#include <QDebug>
#include <QPainter>
#include <QPaintEvent>
#include <QMenuBar>
#include <QAction>
#include <QWidget>
#include <QString>
#include <QTimer>
#include <QIcon>
#include "dialog.h"
#include "game.h"

const int blocksize = 32;
const int emojisize = 48;
const int interval = 10;
const int numberheight = 46;
const int numberwidth = 26;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

    Ui::MainWindow *ui;
    QTimer *fTimer;
    QAction *newGameAction,*exitAction,*beginner,*intermediate,*expert,*custom;
    QActionGroup *group;
    game *gm;
private:
    void newgame();
    void setBeginner();
    void setIntermediate();
    void setExpert();
    void setCustom();
    void aboutBox();
private slots:
    void updateTimer();
    void setcustom(int,int,int);
protected:
    void mousePressEvent(QMouseEvent *) override;
    void mouseReleaseEvent(QMouseEvent *) override;
    void paintEvent(QPaintEvent *) override;
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;
};

#endif // MAINWINDOW_H
