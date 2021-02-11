// CC-BY Edouard.Thiel@univ-amu.fr - 22/01/2019

#ifndef PRINC_H
#include "ui_princ.h"
#define PRINC_H


class Princ : public QMainWindow, private Ui::Princ
{
    Q_OBJECT

public:
    explicit Princ(QWidget *parent = 0);

public slots:
    void setSliderRadius(double radius);
    void openFrustumDial();

protected slots:
    void onSliderRadius(int value);
};

#endif // PRINC_H
