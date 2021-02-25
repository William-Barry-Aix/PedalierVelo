#include "maillon.h"
#include <stdio.h>
#include <iostream>

#include <GL/glu.h>

#include <math.h>
#include <QVector3D>
#include <QMatrix4x4>
#include <QSurfaceFormat>
#include <QOpenGLFunctions>
#define PI 3.14159265

Maillon::Maillon(double z_ep, double x_width, double y_height, double bevel_size, float origin, float coul_r, float coul_v, float coul_b)
{
    this->z_ep = z_ep;
    this->x_width = x_width;
    this->y_height = y_height;
    this->bevel_size = bevel_size;
    this->origin = origin;
    this->coul_r = coul_r;
    this->coul_v = coul_v;
    this->coul_b = coul_b;

    initPoints();

    m_vbo.create();
    m_vbo.bind();
    QVector<GLfloat> vertData;
    buildVertData(vertData);
    m_vbo.allocate(vertData.constData(), vertData.count() * sizeof(GLfloat));
    m_vbo.release();
}


void Maillon::initPoints(){

    nbPtsFace = 16;         //ce hasard : les roues c'est le meme nb de points que les maillons ^^
    nbPtsFacette = 16;

    this->vertices = new GLfloat[nbPtsFace*3 + nbPtsFacette*3];
    this->colors = new GLfloat[nbPtsFace*3 + nbPtsFacette*3];

    QVector3D A = QVector3D((x_width/2) - bevel_size, -y_height/2, z_ep/2);
    QVector3D B = QVector3D(x_width/2, -(y_height/2) + bevel_size, z_ep/2);

    QVector3D C = QVector3D(x_width/2, (y_height/2) - bevel_size, z_ep/2);
    QVector3D D = QVector3D((x_width/2) - bevel_size, y_height/2, z_ep/2);

    QVector3D E = QVector3D((-x_width/2) + bevel_size, y_height/2, z_ep/2);
    QVector3D F = QVector3D(-x_width/2, (y_height/2) - bevel_size, z_ep/2);

    QVector3D G = QVector3D(-x_width/2, (-y_height/2) + bevel_size, z_ep/2);
    QVector3D H = QVector3D((-x_width/2) + bevel_size, -y_height/2, z_ep/2);


    QVector3D Ap = A;
    QVector3D Bp = B;
    QVector3D Cp = C;
    QVector3D Dp = D;
    QVector3D Ep = E;
    QVector3D Fp = F;
    QVector3D Gp = G;
    QVector3D Hp = H;

    Ap.setZ(z_ep/2);
    Bp.setZ(z_ep/2);
    Cp.setZ(z_ep/2);
    Dp.setZ(z_ep/2);
    Ep.setZ(z_ep/2);
    Fp.setZ(z_ep/2);
    Gp.setZ(z_ep/2);
    Hp.setZ(z_ep/2);

    QVector3D color = QVector3D(coul_r, coul_v, coul_b);

    setColors(color);

    addVertice(A);
    addVertice(B);
    addVertice(C);
    addVertice(D);
    addVertice(E);
    addVertice(F);
    addVertice(G);
    addVertice(H);

    addVertice(Ap);
    addVertice(Bp);
    addVertice(Cp);
    addVertice(Dp);
    addVertice(Ep);
    addVertice(Fp);
    addVertice(Gp);
    addVertice(Hp);


    addVertice(B);
    addVertice(C);
    addVertice(Cp);
    addVertice(Bp);

    addVertice(C);
    addVertice(D);
    addVertice(Dp);
    addVertice(Cp);

    addVertice(D);
    addVertice(E);
    addVertice(Ep);
    addVertice(Dp);

    addVertice(E);
    addVertice(F);
    addVertice(Fp);
    addVertice(Ep);
}

void Maillon::buildVertData(QVector<GLfloat> &data)
{
    for (int i = 0; i < verticesCpt; i++){
        data.append(vertices[i]);
    }
    for (int i = 0; i < colorsCpt; i++){
        data.append(colors[i]);
    }
    qDebug() << data.length() << " " << verticesCpt+colorsCpt;
}


void Maillon::draw(QOpenGLShaderProgram *m_program, QMatrix4x4 matrix, int m_matrixUniform){
    matrix.rotate(0, 0, 0, 1);
    m_program->setUniformValue(m_matrixUniform, matrix);
    drawBlock();
}


void Maillon::drawBlock(){
    glDrawArrays(GL_POLYGON, 0, nbPtsFace/2);
    glDrawArrays(GL_POLYGON, nbPtsFace/2, nbPtsFace/2);
    glDrawArrays(GL_QUADS, nbPtsFacette, nbPtsFacette);
}

void Maillon::drawBlock(QOpenGLShaderProgram *program, QOpenGLFunctions *glFuncs)
{
    m_vbo.bind();

    program->setAttributeBuffer("posAttr",
        GL_FLOAT, 0 * sizeof(GLfloat), 3, 3 * sizeof(GLfloat));
    program->setAttributeBuffer("colAttr",
        GL_FLOAT, verticesCpt * sizeof(GLfloat), 3, 3 * sizeof(GLfloat));
    program->enableAttributeArray("posAttr");
    program->enableAttributeArray("colAttr");

    // Pour des questions de portabilité, hors de la classe GLArea, tous les appels
    // aux fonctions glBidule doivent être préfixés par glFuncs->.
    glFuncs->glDrawArrays(GL_POLYGON, 0, nbPtsFace/2);
    glFuncs->glDrawArrays(GL_POLYGON, nbPtsFace/2, nbPtsFace/2);
    glFuncs->glDrawArrays(GL_QUADS, nbPtsFacette, nbPtsFacette);

    program->disableAttributeArray("posAttr");
    program->disableAttributeArray("colAttr");

    m_vbo.release();
}
