#include "frustumdialog.h"
#include "ui_frustumdialog.h"

FrustumDialog::FrustumDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::FrustumDialog)
{
    ui->setupUi(this);
    dist_camera = this->findChild<QDoubleSpinBox*>("box_dist_camera");
    radius = this->findChild<QDoubleSpinBox*>("box_radius");
    near = this->findChild<QDoubleSpinBox*>("box_near");
    far = this->findChild<QDoubleSpinBox*>("box_far");
    angle = this->findChild<QDoubleSpinBox*>("box_angle");
}

FrustumDialog::~FrustumDialog()
{
    delete ui;

    delete dist_camera;
    delete radius;
    delete near;
    delete far;
    delete angle;

}

void FrustumDialog::fill(float dist_camera, float radius, float near, float far, float angle){
    this->dist_camera->setValue(dist_camera);
    this->radius->setValue(radius);
    this->near->setValue(near);
    this->far->setValue(far);
    this->angle->setValue(angle);
}




