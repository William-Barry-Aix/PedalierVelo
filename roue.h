#ifndef CYLINDRE_H
#define CYLINDRE_H
#include "forme.h"
#include <QVector3D>
#include <GL/glu.h>

class Cylindre : public Forme
{
public:
    Cylindre(double ep_cyl, double r_cyl, double nb_fac, float coul_r, float coul_v, float coul_b);
    ~Cylindre(){
        delete [] vertices;
        delete [] colors;

        vertices = nullptr;
        colors = nullptr;
    }
    void initPoints();

    void draw(QOpenGLShaderProgram* m_program, QMatrix4x4 matrix, int m_matrixUniform);
    void drawBlock();

protected:
    double ep_cyl;
    double r_cyl;
    double nb_fac;
    int coul_r, coul_v, coul_b;
};

#endif // CYLINDRE_H
