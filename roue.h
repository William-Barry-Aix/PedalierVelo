#ifndef ROUE_H
#define ROUE_H
#include "forme.h"
#include <QVector3D>
#include <GL/glu.h>

class Roue : public Forme
{
public:
    Roue(double ep_cyl, double r_cyl, double nb_fac, float coul_r, float coul_v, float coul_b);
    ~Roue(){
        delete [] vertices;
        delete [] colors;

        vertices = nullptr;
        colors = nullptr;
    }
    void initPoints();

    void draw(QOpenGLShaderProgram* m_program, QMatrix4x4 matrix, int m_matrixUniform);
    void drawBlock();
    QVector3D getVertex(float o, float h, float z);

protected:
    double ep_cyl;
    double r_cyl;
    double nb_dents;
    double h_dent = 0.15;
    int coul_r, coul_v, coul_b;
};

#endif // ROUE_H
