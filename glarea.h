// CC-BY Edouard.Thiel@univ-amu.fr - 22/01/2019

#ifndef GLAREA_H
#include "roue.h"
#include <QKeyEvent>
#include <QTimer>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#define GLAREA_H


class GLArea : public QOpenGLWidget,
               protected QOpenGLFunctions
{
    Q_OBJECT

public:
    explicit GLArea(QWidget *parent = 0);
    ~GLArea();

    double m_distCam = 0;
    double m_radius = 0.5;
    double m_near = 1;
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


private:
    QTimer *m_timer = nullptr;
    double m_anim = 0;
    double m_ratio = 1;
    double m_alpha = 0;

    // Pour utiliser les shaders
    QOpenGLShaderProgram *m_program;
    int m_posAttr;
    int m_colAttr;
    int m_matrixUniform;
    Roue *cyl = nullptr;
    Roue *roue1 = nullptr;// = Roue(1, 1, 20, -1, 255, 255, 255);
    Roue *roue2 = nullptr;// = Roue(1, 0.5, 10, 2, 255, 255, 255);


    void setTransforms(QMatrix4x4 &cam_mat, QMatrix4x4 &shape_mat);
    //Roue  = Roue(1, 0.5, 15, 255, 255, 255);
};

#endif // GLAREA_H
