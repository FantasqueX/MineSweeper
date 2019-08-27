#include "game.h"

void game::init(const int &hght,const int &wdth,const int &minenum)
{
    mineNum = minenum;
    height = hght;
    width = wdth;
    tot = height * width;
    lftblock = height*width-mineNum;
    lftmine = minenum;
    status = 0;
    emoji = 0;
    Timer = 0;
    p = new block[tot];
    for(int i=0;i<tot;++i)
    {
        (p+i)->cnt = 0;
        (p+i)->mine = 0;
        (p+i)->digged = 0;
        (p+i)->mark = 0;
        (p+i)->hint = 0;
        (p+i)->left = 0;
        (p+i)->right = 0;
        (p+i)->both = 0;
    }
    //初始化布雷
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
    int minecnt = mineNum,x;
    while(minecnt--)
    {
        x = qrand()%tot;
        while((p+x)->mine)
            x = qrand()%tot;
        (p+x)->mine = true;
    }
    //计数
    for(int i=0;i<tot;++i)
        if((p+i)->mine)
        {
            if(i%width != width - 1)
                ++(p+i+1)->cnt;
            if(i%width != 0)
                ++(p+i-1)->cnt;
            if(i>=width)
                ++(p+i-width)->cnt;
            if(i<tot-width)
                ++(p+i+width)->cnt;
            if(i%width !=0 && i >=width)
                ++(p+i-width-1)->cnt;
            if(i%width !=0 && i < tot-width)
                ++(p+i+width-1)->cnt;
            if(i%width !=width-1 && i >=width)
                ++(p+i-width +1)->cnt;
            if(i%width !=width-1 && i <tot-width)
                ++(p+i+width +1)->cnt;
        }
}

game::game()
{
    init(9,9,10);
}

void game::restart()
{
    delete [] p;
    init(height,width,mineNum);
}

void game::setModel(int hght,int wdth,int minenum)
{
    height = hght;
    width = wdth;
    mineNum = minenum;
    restart();
}

void game::win()
{
    status = 1;
    lftmine = 0;
}

void game::lose()
{
    status = -1;
}

void game::mark(int pos)
{
    if(status == -1 || status == 1)
        return ;
    if(!(p+pos)->digged)
    {
        if((p+pos)->mark == 1)
            ++lftmine;
        (p+pos)->mark += 1;
        (p+pos)->mark %= 3;
        if((p+pos)->mark == 1)
            --lftmine;
    }
}

void game::dig(int pos)
{
    if(status == 1 || status == -1 || (p+pos)->digged || (p+pos)->mark == 1)
        return ;
    (p+pos)->digged = true;
    if((p+pos)->mine)
    {
        status = -1;
        lose();
        return ;
    }
    --lftblock;
    if(!lftblock)
        win();
    if((p+pos)->cnt > 0)
        return ;
    if(pos%width != width - 1)
        dig(pos+1);
    if(pos%width !=0 && pos >=width)
        dig(pos-width-1);
    if(pos%width != 0)
        dig(pos-1);
    if(pos%width !=width-1 && pos >=width)
        dig(pos-width +1);
    if(pos>=width)
        dig(pos-width);
    if(pos%width !=0 && pos < tot-width)
        dig(pos+width-1);
    if(pos<tot-width)
        dig(pos+width);
    if(pos%width !=width-1 && pos <tot-width)
        dig(pos +width+1);
}

void game::doublepress(int pos)
{
    if(pos%width !=width-1)
        (p+pos+1)->hint ^= 1;
    if(pos % width != 0 && pos >= width)
        (p+pos-width-1)->hint ^= 1;
    if(pos%width != 0)
        (p+pos-1)->hint ^= 1;
    if(pos % width != width-1 && pos>= width)
        (p+pos-width+1)->hint ^= 1;
    if(pos>=width)
        (p+pos-width)->hint ^= 1;
    if(pos %width !=0 && pos <tot-width)
        (p+pos+width-1)->hint ^= 1;
    if(pos<tot-width)
        (p+pos+width)->hint ^= 1;
    if(pos % width != width-1 && pos <tot-width)
        (p+pos+width+1)->hint ^= 1;
}
void game::doublerelease(int pos)
{
    if((p+pos)->digged)
    {
        int count = 0;
        if(pos%width !=width-1 && (p+pos +1)->mark == 1)
            ++count;
        if(pos % width != 0 && pos >= width && (p+pos-width-1)->mark == 1)
            ++count;
        if(pos%width != 0 && (p+pos-1)->mark == 1)
            ++count;
        if(pos % width != width-1 && pos>= width && (p+pos-width+1)->mark == 1)
            ++count;
        if(pos>=width && (p+pos-width)->mark == 1)
            ++count;
        if(pos %width !=0 && pos <tot-width && (p+pos+width-1)->mark == 1)
            ++count;
        if(pos<tot-width && (p+pos+width)->mark == 1)
            ++count;
        if(pos % width != width-1 && pos <tot-width && (p+pos+width+1)->mark== 1)
            ++count;
        if(count == (p+pos)->cnt)
        {
            if(pos%width !=width-1 && (p+pos +1)->mark != 1)
                dig(pos+1);
            if(pos % width != 0 && pos >= width && (p+pos-width-1)->mark != 1)
                dig(pos-width-1);
            if(pos%width != 0 && (p+pos-1)->mark != 1)
                dig(pos-1);
            if(pos % width != width-1 && pos>= width && (p+pos-width+1)->mark != 1)
                dig(pos-width+1);
            if(pos>=width && (p+pos-width)->mark != 1)
                dig(pos-width);
            if(pos %width !=0 && pos <tot-width && (p+pos+width-1)->mark != 1)
                dig(pos+width-1);
            if(pos<tot-width && (p+pos+width)->mark != 1)
                dig(pos+width);
            if(pos % width != width-1 && pos <tot-width && (p+pos+width+1)->mark!= 1)
                dig(pos+width+1);
        }
    }
    doublepress(pos);
}

void game::hint(int pos)
{
    (p+pos)->hint ^= 1;
}

void game::changeStatus()
{
    if(status == -1)
        return ;
    if(status == -2)
        status = 0;
    else if(status == 0)
        status = -2;
}

void game::changeEmoji()
{
    emoji ^= 1;
}

game::~game()
{
    delete [] p;
}
