#ifndef ROUE_H
#include "forme.h"
#include <QVector3D>
#include <GL/glu.h>
#define ROUE_H

class Roue : public Forme
{
public:
    Roue(double ep_cyl, double r_cyl, double nb_dents, float origin, float coul_r, float coul_v, float coul_b);

    void initPoints();

    void draw(QOpenGLShaderProgram* m_program, QMatrix4x4 matrix, int m_matrixUniform);
    void draw(QOpenGLShaderProgram* m_program, QMatrix4x4 cyleMat,  QOpenGLFunctions* glFuncs);
    void drawBlock();
    QVector3D getVertex(float o, float h, float z);
    double ep_cyl;
    double r_cyl;
    double nb_dents;
    double h_dent = 0.15;
    float pas;
    float origin;

protected:
    int coul_r, coul_v, coul_b;
};

#endif // ROUE_H
