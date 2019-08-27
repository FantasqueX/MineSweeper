#include "dialog.h"

Dialog::Dialog(QWidget *parent)
    :QDialog (parent)
{
    setWindowTitle("Custom Feild");
    qhlheight = new QHBoxLayout;
    qhlwidth = new QHBoxLayout;
    qhlmines = new QHBoxLayout;
    labelheight = new QLabel("Height");
    labelwidth = new QLabel("Width");
    labelmines = new QLabel("Mines");
    qsbheight = new QSpinBox;
    qsbwidth = new QSpinBox;
    qsbmines = new QSpinBox;
    qsbheight->setMaximum(24);
    qsbwidth->setMaximum(30);
    qsbmines->setMaximum(667);
    qhlheight->addWidget(labelheight);
    qhlheight->addWidget(qsbheight);
    qhlwidth->addWidget(labelwidth);
    qhlwidth->addWidget(qsbwidth);
    qhlmines->addWidget(labelmines);
    qhlmines->addWidget(qsbmines);
    qvl1 = new QVBoxLayout;
    qvl1->addLayout(qhlheight);
    qvl1->addLayout(qhlwidth);
    qvl1->addLayout(qhlmines);
    qvl2 = new QVBoxLayout;
    btnOK = new QPushButton("Ok");
    btnCancel = new QPushButton("Cancel");
    qvl2->addWidget(btnOK);
    qvl2->addWidget(btnCancel);
    qhl = new QHBoxLayout;
    qhl->addLayout(qvl1);
    qhl->addLayout(qvl2);
    setLayout(qhl);

    connect(btnOK,&QPushButton::clicked,this,&Dialog::accept);
    connect(btnCancel,&QPushButton::clicked,this,&Dialog::reject);
    void (QSpinBox::*myValueChanged)(int) = &QSpinBox::valueChanged;
    connect(qsbheight,myValueChanged,this,&Dialog::setheight);
    connect(qsbwidth,myValueChanged,this,&Dialog::setwidth);
    connect(qsbmines,myValueChanged,this,&Dialog::setmines);
    connect(btnOK,&QPushButton::clicked,this,&Dialog::send);
}

void Dialog::setheight(int x)
{
    height = x;
}

void Dialog::setwidth(int x)
{
    width = x;
}

void Dialog::setmines(int x)
{
    mines = x;
}

void Dialog::send()
{
    emit mysignal(height,width,mines);
}

Dialog::~Dialog()
{
}
