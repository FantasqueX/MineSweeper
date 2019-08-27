#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    resize(2*interval+blocksize*9,blocksize*9+3*interval+emojisize+menuBar()->height());
    setWindowTitle("MineSweeper");
    setWindowIcon(QIcon(":/icon/icon"));
    gm = new game();

    newGameAction = new QAction("New",this);
    connect(newGameAction,&QAction::triggered,this,&MainWindow::newgame);
    QMenu *game = menuBar()->addMenu("Game");
    game->addAction(newGameAction);
    QMenu *about = menuBar()->addMenu("Help");
    QAction *aboutAction = new QAction("About",this);
    about->addAction(aboutAction);

    group = new QActionGroup(this);
    group->setExclusive(true);
    beginner = new QAction("Beginner",group);
    beginner->setCheckable(true);
    beginner->setChecked(true);
    game->addAction(beginner);
    intermediate = new QAction("Intermediate",group);
    intermediate->setCheckable(true);
    game->addAction(intermediate);
    expert = new QAction("Expert",group);
    expert->setCheckable(true);
    game->addAction(expert);
    custom = new QAction("Custom...",group);
    custom->setCheckable(true);
    game->addAction(custom);

    connect(beginner,&QAction::triggered,this,&MainWindow::setBeginner);
    connect(intermediate,&QAction::triggered,this,&MainWindow::setIntermediate);
    connect(expert,&QAction::triggered,this,&MainWindow::setExpert);
    connect(custom,&QAction::triggered,this,&MainWindow::setCustom);
    connect(aboutAction,&QAction::triggered,this,&MainWindow::aboutBox);

    exitAction = new QAction("Exit",this);
    connect(exitAction,&QAction::triggered,this,&MainWindow::close);
    game->addAction(exitAction);

    fTimer = new QTimer(this);
    connect(fTimer,SIGNAL(timeout()),this,SLOT(updateTimer()));
    fTimer->setInterval(1000);
    fTimer->start();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete gm;
    delete fTimer;
    delete newGameAction;
    delete exitAction;
    delete beginner;
    delete intermediate;
    delete expert;
    delete custom;
    delete group;
}

void MainWindow::aboutBox()
{
    QMessageBox qmb(this);
    qmb.about(this,"About","MineSweeper\nBased on Qt 5.13.0(MinGW 64-bit)\nAuthor:FantasqueX");
}

void MainWindow::setBeginner()
{
    gm->setModel(9,9,10);
    resize(2*interval+blocksize*gm->width,blocksize*gm->height+3*interval+emojisize+menuBar()->height());
    update();
}

void MainWindow::setIntermediate()
{
    gm->setModel(16,16,40);
    resize(2*interval+blocksize*gm->width,blocksize*gm->height+3*interval+emojisize+menuBar()->height());
    update();
}

void MainWindow::setExpert()
{
    gm->setModel(16,30,99);
    resize(2*interval+blocksize*gm->width,blocksize*gm->height+3*interval+emojisize+menuBar()->height());
    update();
}

void MainWindow::setCustom()
{
    Dialog *dialog;
    dialog = new Dialog;
    connect(dialog,&Dialog::mysignal,this,&MainWindow::setcustom);
    if(dialog->exec())
    {
        gm->setModel(gm->height,gm->width,gm->mineNum);
        resize(2*interval+blocksize*gm->width,blocksize*gm->height+3*interval+emojisize+menuBar()->height());
        update();
    }
}

void MainWindow::setcustom(int hght,int wdth,int mns)
{
    gm->setModel(hght,wdth,mns);
    resize(2*interval+blocksize*gm->width,blocksize*gm->height+3*interval+emojisize+menuBar()->height());
    update();
}

void MainWindow::newgame()
{
    gm->restart();
    update();
}

void MainWindow::updateTimer()
{
    if(gm->Timer != 999)
        ++gm->Timer;
    update();
}

void MainWindow::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    QPixmap BlockInitial,Block[9],BlockFlag,BlockQuestion,BlockWrong,BlockMine,Emoji,BlockExplode,BlockQuestion2,Number[9],NumberMinus,NumberBlank;
    BlockExplode.load(":/block/explode");
    BlockInitial.load(":/block/initial");
    BlockWrong.load(":/block/wrong");
    BlockMine.load(":/block/mine");
    BlockFlag.load(":/block/flag");
    BlockQuestion.load(":/block/question");
    BlockQuestion2.load(":/block/question2");
    for(int i=0;i<=8;++i)
        (Block+i)->load(QString(":/block/")+QString(char(i+'0')));
    NumberBlank.load(":/number/blank");
    NumberMinus.load(":/number/minus");
    for(int i=0;i<=9;++i)
        (Number+i)->load(QString(":/number/")+QString(char(i+'0')));
    int posx = 2*interval,posy = interval + (emojisize-numberheight)/2+menuBar()->height(),n0,n1,n2;
    if(gm->lftmine>=0)
    {
        n0 = gm->lftmine%10;
        n1 = gm->lftmine/10%10;
        n2 = gm->lftmine/100%10;
        painter.drawPixmap(posx,posy,*(Number+n2));
        posx += numberwidth;
        painter.drawPixmap(posx,posy,*(Number+n1));
        posx += numberwidth;
        painter.drawPixmap(posx,posy,*(Number+n0));
    }
    else {
        int tmp = -gm->lftmine;
        painter.drawPixmap(posx,posy,NumberMinus);
        n0 = tmp%10;
        n1 = tmp/10%10;
        posx += numberwidth;
        painter.drawPixmap(posx,posy,*(Number+n1));
        posx += numberwidth;
        painter.drawPixmap(posx,posy,*(Number+n0));
    }
    posx = gm->width*blocksize-3*numberwidth;
    n0 = gm->Timer%10;
    n1 = gm->Timer/10%10;
    n2 = gm->Timer/100%10;
    painter.drawPixmap(posx,posy,*(Number+n2));
    posx += numberwidth;
    painter.drawPixmap(posx,posy,*(Number+n1));
    posx += numberwidth;
    painter.drawPixmap(posx,posy,*(Number+n0));
    if(gm->status == 0) {
        posx = interval + blocksize/2 * gm->width -emojisize/2;
        posy = interval + menuBar()->height();
        if(gm->emoji)
            Emoji.load(":/emoji/laugh2");
        else
            Emoji.load(":/emoji/laugh");
        painter.drawPixmap(posx,posy,Emoji);
        for(int i=0;i<gm->height;++i)
            for(int j=0;j<gm->width;++j)
            {
                posx = j*blocksize + interval;
                posy = i*blocksize + 2*interval + emojisize + menuBar()->height();
                if((gm->p+i*gm->width+j)->digged)
                    painter.drawPixmap(posx,posy,*(Block+(gm->p+i*gm->width+j)->cnt));
                else {
                    if((gm->p+i*gm->width+j)->mark == 1)
                        painter.drawPixmap(posx,posy,BlockFlag);
                    else if((gm->p+i*gm->width+j)->mark == 2)
                        painter.drawPixmap(posx,posy,BlockQuestion);
                    else
                        painter.drawPixmap(posx,posy,BlockInitial);
                }
            }
    }
    else if (gm->status == 1) {
        fTimer->stop();
        posx = interval + blocksize/2 * gm->width -emojisize/2;
        posy = interval + menuBar()->height();
        if(gm->emoji)
            Emoji.load(":/emoji/laugh2");
        else
            Emoji.load(":/emoji/win");
        painter.drawPixmap(posx,posy,Emoji);
        for(int i=0;i<gm->height;++i)
            for(int j=0;j<gm->width;++j)
            {
                posx = j*blocksize + interval;
                posy = i*blocksize + 2*interval + emojisize + menuBar()->height();
                if(!(gm->p+i*gm->width+j)->mine)
                    painter.drawPixmap(posx,posy,*(Block+(gm->p+i*gm->width+j)->cnt));
                else
                    painter.drawPixmap(posx,posy,BlockFlag);
            }
    }
    else if(gm->status == -2) {
        posx = interval + blocksize/2 * gm->width -emojisize/2;
        posy = interval + menuBar()->height();
        if(gm->emoji)
            Emoji.load(":/emoji/laugh2");
        else
            Emoji.load(":/emoji/warn");
        painter.drawPixmap(posx,posy,Emoji);
        for(int i=0;i<gm->height;++i)
            for(int j=0;j<gm->width;++j)
            {
                posx = j*blocksize + interval;
                posy = i*blocksize + 2*interval + emojisize + menuBar()->height();
                if((gm->p+i*gm->width+j)->digged)
                    painter.drawPixmap(posx,posy,*(Block+(gm->p+i*gm->width+j)->cnt));
                else {
                    if((gm->p+i*gm->width+j)->mark == 1)
                        painter.drawPixmap(posx,posy,BlockFlag);
                    else if((gm->p+i*gm->width+j)->mark == 2)
                    {
                        if((gm->p+i*gm->width+j)->hint)
                            painter.drawPixmap(posx,posy,BlockQuestion2);
                        else
                            painter.drawPixmap(posx,posy,BlockQuestion);
                    }
                    else
                    {
                        if((gm->p+i*gm->width+j)->hint)
                            painter.drawPixmap(posx,posy,*Block);
                        else
                            painter.drawPixmap(posx,posy,BlockInitial);
                    }
                }

            }
    }
    else {
        fTimer->stop();
        posx = interval + blocksize/2 * gm->width -emojisize/2;
        posy = interval + menuBar()->height();
        if(gm->emoji)
            Emoji.load(":/emoji/laugh2");
        else
            Emoji.load(":/emoji/lose");
        painter.drawPixmap(posx,posy,Emoji);
        for(int i=0;i<gm->height;++i)
            for(int j=0;j<gm->width;++j)
            {
                posx = j*blocksize + interval;
                posy = i*blocksize + 2*interval + emojisize + menuBar()->height();
                if((gm->p+i*gm->width+j)->digged)
                {
                    if(!(gm->p+i*gm->width+j)->mine)
                        painter.drawPixmap(posx,posy,*(Block+(gm->p+i*gm->width+j)->cnt));
                    else
                        painter.drawPixmap(posx,posy,BlockExplode);
                }
                else
                {
                    if(!(gm->p+i*gm->width+j)->mark)
                    {
                        if(!(gm->p+i*gm->width+j)->mine)
                            painter.drawPixmap(posx,posy,BlockInitial);
                        else
                            painter.drawPixmap(posx,posy,BlockMine);
                    }
                    else if((gm->p+i*gm->width+j)->mark == 1)
                    {
                        if((gm->p+i*gm->width+j)->mine)
                            painter.drawPixmap(posx,posy,BlockFlag);
                        else
                            painter.drawPixmap(posx,posy,BlockWrong);
                    }
                    else
                    {
                        if(!(gm->p+i*gm->width+j)->mine)
                            painter.drawPixmap(posx,posy,BlockQuestion);
                        else
                            painter.drawPixmap(posx,posy,BlockMine);
                    }
                }
            }
    }
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    int x = event->x(),y = event->y(),pos = ((y - 2*interval - emojisize - menuBar()->height())/blocksize)*gm->width+(x-interval)/blocksize;
    if(event->button() == Qt::LeftButton)
    {
        if(x>interval + blocksize/2 * gm->width -emojisize/2 && x<=interval + blocksize/2 * gm->width+emojisize/2 && y>interval + menuBar()->height() && y<=interval + emojisize + menuBar()->height())
            gm->changeEmoji();
        else {
            gm->changeStatus();
            if(x>interval && x <= gm->width*blocksize + interval && y >2*interval + emojisize + menuBar()->height() && y <= gm->height*blocksize +2*interval + emojisize + menuBar()->height())
            {
                gm->hint(pos);
                (gm->p+pos)->left = 1;
                if((gm->p+pos)->right)
                {
                    (gm->p+pos)->both = 1;
                    (gm->p+pos)->left = (gm->p+pos)->right = 0;
                    gm->doublepress(pos);
                }
            }
        }
    }
    else {
        if(x>interval && x <= gm->width*blocksize + interval && y >2*interval + emojisize + menuBar()->height() && y <= gm->height*blocksize +2*interval + emojisize + menuBar()->height())
        {
            (gm->p+pos)->right = 1;
            if((gm->p+pos)->left)
            {
                (gm->p+pos)->both = 1;
                (gm->p+pos)->left = (gm->p+pos)->right = 0;
                gm->doublepress(pos);
            }
            else {
                gm->mark(pos);
            }
        }
    }
    update();
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    int x = event->x(),y = event->y(),pos= ((y-2*interval-emojisize - menuBar()->height())/blocksize)*gm->width+(x-interval)/blocksize;
    if(event->button() == Qt::LeftButton)
    {
        if(x>interval + blocksize/2 * gm->width -emojisize/2 && x<=interval + blocksize/2*gm->width+emojisize/2 && y>interval + menuBar()->height() && y<=interval + emojisize + menuBar()->height())
        {
            gm->changeEmoji();
            fTimer->start();
            gm->restart();
        }
        else {
            gm->changeStatus();
            if(x>interval && x <= gm->width*blocksize + interval && y >2*interval + emojisize + menuBar()->height() && y <= gm->height*blocksize +2*interval + emojisize + menuBar()->height())
            {
                if((gm->p+pos)->left)
                {
                    gm->dig(pos);
                    (gm->p+pos)->left = 0;
                }
                if((gm->p+pos)->both)
                {
                    gm->doublerelease(pos);
                    (gm->p+pos)->both = 0;
                }
            }
        }
    }
    else {
        if(x>interval && x <= gm->width*blocksize + interval && y >2*interval + emojisize + menuBar()->height() && y <= gm->height*blocksize +2*interval + emojisize + menuBar()->height())
        {
            if((gm->p+pos)->both)
            {
                gm->doublerelease(pos);
                (gm->p+pos)->both = 0;
            }
            if((gm->p+pos)->right)
                (gm->p+pos)->right = 0;
        }
    }
    update();
}
