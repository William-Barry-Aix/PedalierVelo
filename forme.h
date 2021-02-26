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

        m_vbo.destroy();
    }
    void addVertice(QVector3D value);
    void addColor(QVector3D color);
    virtual void initPoints() = 0;
    virtual void buildVertData(QVector<GLfloat> &data) = 0;
    void draw(QOpenGLShaderProgram* m_program, QMatrix4x4* matrix, int m_matrixUniform);
    void draw(QOpenGLShaderProgram* m_program, QMatrix4x4* cyleMat,  QOpenGLFunctions* glFuncs);
    void drawBlock();
    QVector3D getPoint(int id);
    QVector3D getNormal();
    void addNormal(QVector3D normal);
    int verticesCpt = 0;
    int colorsCpt = 0;
    int normalCpt = 0;
    int nbPtsFace = 0;
    int nbPtsFacette = 0;
    GLfloat* vertices = nullptr;
    GLfloat* colors = nullptr;
    GLfloat* normals = nullptr;
    void setColors(QVector3D colors);

    QVector3D origin;
    QOpenGLBuffer m_vbo;

private:
    bool m_phong_shading = false;
};

#endif // FORME_H
