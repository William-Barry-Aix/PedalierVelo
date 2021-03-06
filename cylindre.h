#ifndef CYLINDRE_H
#define CYLINDRE_H
#include "forme.h"
#include <QVector3D>
#include <GL/glu.h>

class Cylindre : public Forme
{
public:
    Cylindre(double ep_cyl, double r_cyl, double nb_fac, float coul_r, float coul_v, float coul_b);
    ~Cylindre();
    void initPoints();
    void buildVertData(QVector<GLfloat> &data);

    void draw(QOpenGLShaderProgram* m_program, QMatrix4x4 matrix, int m_matrixUniform);
    void draw(QOpenGLShaderProgram *m_program, QMatrix4x4 cyleMat, QMatrix4x4 cam_mat,  QOpenGLFunctions* glFuncs);
    void drawBlock();
    void drawBlock(QOpenGLShaderProgram *program, QOpenGLFunctions *glFuncs);

public:
    double ep_cyl;
    double r_cyl;
    double nb_fac;
    int coul_r, coul_v, coul_b;
    int state = 1;   // 1 : plaqué sur la grande roue / 2 : ligne entre grande et petite roue
                     // 3 : plaqué sur la petite roue / 4 : ligne entre petite et grande roue
};

#endif // CYLINDRE_H
