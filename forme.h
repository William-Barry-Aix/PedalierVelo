#ifndef FORME_H
#define FORME_H
#include <QMap>
#include <QVector3D>
#include <QOpenGLShaderProgram>

#include <QKeyEvent>
#include <QTimer>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLFunctions>

class Forme
{
public:
    Forme();
    ~Forme();
    void addVertice(QVector3D value);
    void addColor(QVector3D color);
    void draw(QOpenGLShaderProgram &m_program, QMatrix4x4 &matrix, int m_matrixUniform);
    void drawBlock();
    int verticesCpt = 0;
    int colorsCpt = 0;
    GLfloat* vertices = nullptr;
    GLfloat* colors = nullptr;
    void setColors(QVector3D colors);

protected:
};

#endif // FORME_H
