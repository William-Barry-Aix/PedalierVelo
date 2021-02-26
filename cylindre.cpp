#include "cylindre.h"
#include <stdio.h>
#include <iostream>

#include <GL/glu.h>

#include <math.h>       /* cos */
#include <QVector3D>
#include <QMatrix4x4>
#include <QSurfaceFormat>
#include <QOpenGLFunctions>
#include <QDebug>
#define PI 3.14159265

Cylindre::Cylindre(double ep_cyl, double r_cyl, double nb_fac, float coul_r, float coul_v, float coul_b){
    this->ep_cyl = ep_cyl;
    this->r_cyl = r_cyl;
    this->nb_fac = nb_fac;

    this->coul_v = coul_v;
    this->coul_b = coul_b;
    this->coul_r = coul_r;

    initPoints();


    m_vbo.create();
    m_vbo.bind();
    QVector<GLfloat> vertData;
    buildVertData(vertData);
    m_vbo.allocate(vertData.constData(), vertData.count() * sizeof(GLfloat));
    m_vbo.release();
}

Cylindre::~Cylindre(){
    qDebug() << "detele cylindre";
    delete [] vertices;
    delete [] colors;

    vertices = nullptr;
    colors = nullptr;
}

void Cylindre::initPoints(){

    nbPtsFace = 6;
    nbPtsFacette = 4;

    this->vertices = new GLfloat[nbPtsFace*3 + nbPtsFacette*3];
    this->colors = new GLfloat[nbPtsFace*3 + nbPtsFacette*3];
    this->normals = new GLfloat[nbPtsFace*3 + nbPtsFacette*3];

    QVector3D A = QVector3D(0,0,ep_cyl/2);
    QVector3D B = QVector3D(0,0,-ep_cyl/2);

    QVector3D C = QVector3D(r_cyl,0,ep_cyl/2);
    QVector3D D = QVector3D(r_cyl,0,-ep_cyl/2);

    double angle = 360/nb_fac;
    float x = cos ( angle * PI / 180.0 )* r_cyl;
    float y = sin ( angle * PI / 180.0 )* r_cyl;
    QVector3D E = QVector3D(x,y,-ep_cyl/2);
    QVector3D F = QVector3D(x,y,ep_cyl/2);

    QVector3D color = QVector3D(coul_r, coul_v, coul_b);

    setColors(color);

    addVertice(A);
    addVertice(C);
    addVertice(F);
    QVector3D normal;
    normal = getNormal();
    for (int i = 0; i < 3; i++){
        addNormal(normal);
    }


    addVertice(B);
    addVertice(D);
    addVertice(E);
    normal.setZ(-normal.z());
    for (int i = 0; i < 3; i++){
        addNormal(normal);
    }

    addVertice(C);
    addVertice(D);
    addVertice(E);
    addVertice(F);
    normal = getNormal();
    normal = -normal;
    for (int i = 0; i < 4; i++){
        addNormal(normal);
    }

}

void Cylindre::buildVertData(QVector<GLfloat> &data){
    for (int i = 0; i < verticesCpt; i++){
        data.append(vertices[i]);
    }
    for (int i = 0; i < colorsCpt; i++){
        data.append(colors[i]);
    }
    for (int i = 0; i < normalCpt; i++){
        data.append(normals[i]);
    }
    qDebug() << data.length() << " " << verticesCpt+colorsCpt;
}

void Cylindre::draw(QOpenGLShaderProgram *m_program, QMatrix4x4 matrix, int m_matrixUniform){
    double angle = 360/nb_fac;
    for (int i = 0; i<nb_fac; i++){
        matrix.rotate(angle, 0, 0, 1);
        m_program->setUniformValue(m_matrixUniform, matrix);
        drawBlock();
    }
}

void Cylindre::draw(QOpenGLShaderProgram *m_program, QMatrix4x4 cyleMat, QMatrix4x4 cam_mat,  QOpenGLFunctions* glFuncs){
    double angle = 360/nb_fac;
    for (int i = 0; i<nb_fac; i++){
        cyleMat.rotate(angle, 0, 0, 1);
        m_program->setUniformValue("mvMatrix", cam_mat*cyleMat);
        drawBlock(m_program, glFuncs);
    }
}

void Cylindre::drawBlock(){
    glDrawArrays(GL_TRIANGLES, 0, nbPtsFace);
    glDrawArrays(GL_QUADS, nbPtsFace, nbPtsFacette);
}

void Cylindre::drawBlock(QOpenGLShaderProgram *program, QOpenGLFunctions *glFuncs)
{
    m_vbo.bind();

    program->setAttributeBuffer("posAttr",
        GL_FLOAT, 0 * sizeof(GLfloat), 3, 3 * sizeof(GLfloat));
    program->setAttributeBuffer("colAttr",
        GL_FLOAT, verticesCpt * sizeof(GLfloat), 3, 3 * sizeof(GLfloat));
    program->setAttributeBuffer("norAttr",
        GL_FLOAT, (verticesCpt+colorsCpt) * sizeof(GLfloat), 3, 3 * sizeof(GLfloat));
    program->enableAttributeArray("posAttr");
    program->enableAttributeArray("colAttr");
    program->enableAttributeArray("norAttr");

    // Pour des questions de portabilité, hors de la classe GLArea, tous les appels
    // aux fonctions glBidule doivent être préfixés par glFuncs->.
    glFuncs->glDrawArrays(GL_TRIANGLES, 0, nbPtsFace);
    glFuncs->glDrawArrays(GL_QUADS, nbPtsFace, nbPtsFacette);

    program->disableAttributeArray("posAttr");
    program->disableAttributeArray("colAttr");
    program->disableAttributeArray("norAttr");

    m_vbo.release();
}


