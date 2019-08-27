#ifndef GAME_H
#define GAME_H

#include <QTime>

struct block
{
    int cnt,mark;
    bool mine,digged,hint,left,right,both;
};

class game
{
    void init(const int&,const int&,const int&);
public:
    int height,width,mineNum,tot;
    int lftblock,lftmine;
    int Timer;
    int status;//0 gaming 1 win -1 lose -2 hint
    bool emoji;
    block *p;
    void dig(int pos);
    void mark(int pos);
    void hint(int pos);
    game();
    void restart();
    void lose();
    void win();
    void setModel(int,int,int);
    void doublepress(int);
    void doublerelease(int);
    void changeStatus();
    void changeEmoji();
    ~game();
};

#endif // GAME_H
