#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QLabel>
#include <QSpinBox>
#include <QBoxLayout>
#include <QPushButton>

class Dialog:public QDialog
{
    Q_OBJECT
private:
    int height,width,mines;
    QLabel *labelheight,*labelwidth,*labelmines;
    QPushButton *btnOK,*btnCancel;
    QHBoxLayout *qhlheight,*qhlwidth,*qhlmines,*qhl;
    QSpinBox *qsbheight,*qsbwidth,*qsbmines;
    QVBoxLayout *qvl1,*qvl2;
public:
    Dialog(QWidget *parent = nullptr);
    ~Dialog();
signals:
    void mysignal(int,int,int);
private slots:
    void send();
    void setheight(int);
    void setwidth(int);
    void setmines(int);
};

#endif // DIALOG_H
