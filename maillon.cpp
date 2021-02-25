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

    QVector3D A = QVector3D((x_width/2) - bevel_size, y_height/2, z_ep/2);

    QVector3D A = getVertex(0, 0, ep_cyl/2);//QVector3D(0,0,ep_cyl/2);
    QVector3D B = getVertex(0,r_cyl - h_dent/2, ep_cyl/2);//QVector3D(0,r_cyl - h_dent/2,-ep_cyl/2);


    QVector3D C = getVertex(alpha/4,r_cyl - h_dent/2, ep_cyl/2);//QVector3D(alpha/4,r_cyl - h_dent/2,ep_cyl/2);
    QVector3D D = getVertex((2*alpha)/4,r_cyl + h_dent/2, ep_cyl/2);//QVector3D((2*alpha)/4,r_roue + h_dent/2,-ep_cyl/2);

    QVector3D E = getVertex((3*alpha)/4, r_cyl + h_dent/2, ep_cyl/2);//QVector3D((2*alpha)/4,r_roue + h_dent/2,-ep_cyl/2);
    QVector3D F = getVertex(alpha, r_cyl - h_dent/2, ep_cyl/2);//QVector3D((2*alpha)/4,r_roue + h_dent/2,-ep_cyl/2);

    QVector3D G = getVertex(alpha, 0, ep_cyl/2);//QVector3D((2*alpha)/4,r_roue + h_dent/2,-ep_cyl/2);
    QVector3D H = getVertex((2*alpha)/4, 0, ep_cyl/2);//QVector3D((2*alpha)/4,r_roue + h_dent/2,-ep_cyl/2);


    QVector3D Ap = A;
    QVector3D Bp = B;
    QVector3D Cp = C;
    QVector3D Dp = D;
    QVector3D Ep = E;
    QVector3D Fp = F;
    QVector3D Gp = G;
    QVector3D Hp = H;

    Ap.setZ(-ep_cyl/2);
    Bp.setZ(-ep_cyl/2);
    Cp.setZ(-ep_cyl/2);
    Dp.setZ(-ep_cyl/2);
    Ep.setZ(-ep_cyl/2);
    Fp.setZ(-ep_cyl/2);
    Gp.setZ(-ep_cyl/2);
    Hp.setZ(-ep_cyl/2);

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
