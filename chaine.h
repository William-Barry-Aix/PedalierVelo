#ifndef CHAINE_H
#include "roue.h"
#define PI 3.14159265358979323846
#define CHAINE_H



class Chaine
{
public:
    Chaine(Roue *pignon, Roue *roue);
    Roue *pignon = nullptr;
    Roue *roue = nullptr;
    float pas;
    float cenrtaxe;
protected:
    void initPoints();
    void buildVertData(QVector<GLfloat> &data);
};


#endif // CHAINE_H
