#include "princ.h"
#include "frustumdialog.h"
#include <QDebug>

Princ::Princ(QWidget *parent) :
    QMainWindow(parent)
{
    setupUi(this);

    connect (glarea, SIGNAL(radiusChanged(double)), this, SLOT(setSliderRadius(double)));
    connect (sli_radius, SIGNAL(valueChanged(int)), this, SLOT(onSliderRadius(int)));
    connect (dial_frustom_btn, SIGNAL(clicked(bool)), this, SLOT(openFrustumDial()));
}


void Princ::setSliderRadius(double radius)
{
    qDebug() << __FUNCTION__ << radius << sender();
    int value = radius*20;
    if (sli_radius->value() != value) {
        qDebug() << "  sli_radius->setvalue()";
        sli_radius->setValue(value);
    }
}

void Princ::onSliderRadius(int value)
{
    qDebug() << __FUNCTION__ << value << sender();
    qDebug() << "  emit radiusChanged()";
    emit glarea->radiusChanged(value/20.0);
}

void Princ::openFrustumDial(){
    FrustumDialog chooseFrustum;
    chooseFrustum.fill(
                glarea->m_distCam,
                glarea->m_radius,
                glarea->m_near,
                glarea->m_far,
                glarea->m_angle);
    if (chooseFrustum.exec()){
        glarea->m_distCam = chooseFrustum.dist_camera->value();
        glarea->m_radius = chooseFrustum.radius->value();
        //glarea->m_near = chooseFrustum.near->value();
        //glarea->m_far = chooseFrustum.far->value();
        glarea->m_angle = chooseFrustum.angle->value();
    }else{

    }

}
