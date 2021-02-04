#include <QMap>
#include <QVector3D>
#include "forme.h"
#include <GL/glu.h>
Forme::Forme(){
    verticesCpt = 0;
    colorsCpt = 0;
}


void Forme::addVertice(QVector3D point){
    vertices[verticesCpt] =  point.x();
    vertices[verticesCpt+1] =  point.y();
    vertices[verticesCpt+2] = point.z();
    verticesCpt += 3;
}


void Forme::addColor(QVector3D color){
    colors[colorsCpt] =  color.x()/255;
    colors[colorsCpt+1] = color.y()/255;
    colors[colorsCpt+2] = color.z()/255;
    colorsCpt += 3;
}

void Forme::setColors(QVector3D colors){
    colorsCpt = 0;
    for (int i = 0; i < 6; i++){
        addColor(colors);
    }
    QVector3D colorFacette = QVector3D(colors.x()*0.8, colors.y()*0.8, colors.z()*0.8);
    for (int i = 0; i < 4; i++){
        addColor(colorFacette);
    }
}

Forme::~Forme(){
}

