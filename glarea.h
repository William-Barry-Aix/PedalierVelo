#ifndef GLAREA_H
#include "roue.h"
#include "cylindre.h"
#include "maillon.h"
#include <QKeyEvent>
#include <QTimer>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QVector3D>
#include <cmath>
#define GLAREA_H


class GLArea : public QOpenGLWidget,
               protected QOpenGLFunctions
{
    Q_OBJECT

public:
    explicit GLArea(QWidget *parent = 0);
    ~GLArea();

    double m_distCam = 3;
    double m_radius = 0.5;
    double m_near = 0.5;
    double m_far = 7;
    double m_angle = 0;


public slots:
    void setRadius(double radius);

signals:  // On ne les implémente pas, elles seront générées par MOC ;
          // les paramètres seront passés aux slots connectés.
    void radiusChanged(double newRadius);

protected slots:
    void onTimeout();

protected:
    void initializeGL() override;
    void doProjection();
    void resizeGL(int w, int h) override;
    void paintGL() override;
    void keyPressEvent(QKeyEvent *ev) override;
    void keyReleaseEvent(QKeyEvent *ev) override;
    void mousePressEvent(QMouseEvent *ev) override;
    void mouseReleaseEvent(QMouseEvent *ev) override;
    void mouseMoveEvent(QMouseEvent *ev) override;
    void teardownGL();
    void makeGLObjects();
    void tearGLObjects();

    void moveCylHautline(QMatrix4x4 *matrix);
    void moveCylBasline(QMatrix4x4 *matrix);
    void moveCylLeftCircle(QMatrix4x4 *matrix);
    void moveCylQRightCircle(QMatrix4x4 *matrix);

private:
    QTimer *m_timer = nullptr;
    double m_anim = 0;
    double m_ratio = 1;
    double m_alpha = 0;
    double vitesse = 0.01;
    bool shiftPress = false;
    bool lockPress = false;

    // Pour utiliser les shaders
    QOpenGLShaderProgram *m_program;
    int m_posAttr;
    int m_colAttr;
    int m_matrixUniform;
    Roue *roue1 = nullptr;// = Roue(1, 1, 20, -1, 255, 255, 255);
    Roue *roue2 = nullptr;// = Roue(1, 0.5, 10, 2, 255, 255, 255);


    void setTransforms(QMatrix4x4 &cam_mat, QMatrix4x4 &shape_mat);
    Cylindre *cyl = nullptr;
    Cylindre *cyl1 = nullptr;
    Cylindre *cyl2 = nullptr;
    Cylindre *cyl3 = nullptr;

    Maillon *maillon = nullptr;

public:
    QVector3D posHautGenMaillLine = QVector3D(2.8, 10, 0);   // points obtenue par le calcul, puis arrondi
    QVector3D posHautDestMaillLine = QVector3D(22, 5, 0);    // points obtenue par le calcul, puis arrondi
    QVector3D posBasGenMaillLine = QVector3D(4, -10, 0);   // points obtenue par le calcul, puis arrondi
    QVector3D posBasDestMaillLine = QVector3D(22, -5, 0);    // points obtenue par le calcul, puis arrondi
};

#endif // GLAREA_H
