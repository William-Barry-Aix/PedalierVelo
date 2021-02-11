#ifndef FORME_H
#include <QMap>
#include <QVector3D>
#include <QOpenGLShaderProgram>

#include <QKeyEvent>
#include <QTimer>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLFunctions>
#include <QOpenGLBuffer>
#define FORME_H

class Forme
{
public:
    Forme();
    ~Forme(){
        delete [] vertices;
        delete [] colors;

        vertices = nullptr;
        colors = nullptr;
    }
    void addVertice(QVector3D value);
    void addColor(QVector3D color);
    virtual void initPoints() = 0;
    void draw(QOpenGLShaderProgram* m_program, QMatrix4x4* matrix, int m_matrixUniform);
    void draw(QOpenGLShaderProgram* m_program, QMatrix4x4* cyleMat,  QOpenGLFunctions* glFuncs);
    void drawBlock();
    int verticesCpt = 0;
    int colorsCpt = 0;
    int nbPtsFace = 0;
    int nbPtsFacette = 0;
    GLfloat* vertices = nullptr;
    GLfloat* colors = nullptr;
    void setColors(QVector3D colors);

    QVector3D origin;

private:
    bool m_phong_shading = false;
    QOpenGLBuffer m_vbo;
};

#endif // FORME_H
