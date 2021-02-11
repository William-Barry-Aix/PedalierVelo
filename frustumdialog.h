#ifndef FRUSTUMDIALOG_H
#include <QDialog>
#include <QDoubleSpinBox>
#define FRUSTUMDIALOG_H


QT_BEGIN_NAMESPACE
namespace Ui { class FrustumDialog; }
QT_END_NAMESPACE

class FrustumDialog : public QDialog
{
    Q_OBJECT

public:
    FrustumDialog(QWidget *parent = nullptr);
    ~FrustumDialog();

    void fill(float dist_camera, float radius, float near, float far, float angle);

    QDoubleSpinBox *dist_camera;
    QDoubleSpinBox *radius;
    //QDoubleSpinBox *near;
    //QDoubleSpinBox *far;
    QDoubleSpinBox *angle;



private:
    Ui::FrustumDialog *ui;
};
#endif // FRUSTUMDIALOG_H
