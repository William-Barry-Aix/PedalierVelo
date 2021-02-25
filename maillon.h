#ifndef MAILLON_H
#define MAILLON_H
#include "forme.h"
#include <QVector3D>
#include <GL/glu.h>

class Maillon : public Forme
{
public:
    Maillon(double z_ep, double x_width, double y_height, double bevel_size, float origin, float coul_r, float coul_v, float coul_b);

    void initPoints();
    void buildVertData(QVector<GLfloat> &data);

    void draw(QOpenGLShaderProgram* m_program, QMatrix4x4 matrix, int m_matrixUniform);
    void draw(QOpenGLShaderProgram* m_program, QMatrix4x4 cyleMat, QMatrix4x4 cam_mat,  QOpenGLFunctions* glFuncs);
    void drawBlock();
    void drawBlock(QOpenGLShaderProgram *program, QOpenGLFunctions *glFuncs);
    QVector3D getVertex(float o, float h, float z);

    double z_ep;
    double x_width;
    double y_height;
    double bevel_size;
    float origin;
    float pas;


protected:
    int coul_r, coul_v, coul_b;
};

#endif // MAILLON_H

