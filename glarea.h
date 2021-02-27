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
#include <QList>
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

    void moveCylHautline(QMatrix4x4 *matrix, float offset);
    void moveCylBasline(QMatrix4x4 *matrix, float offset);
    void moveCylLeftCircle(QMatrix4x4 *matrix, float offset);
    void moveCylQRightCircle(QMatrix4x4 *matrix, float offset);

    void moveMaillHautline(QMatrix4x4 *matrix, float offset);
    void moveMaillBasline(QMatrix4x4 *matrix, float offset);
    void moveMaillLeftCircle(QMatrix4x4 *matrix, float offset);
    void moveMaillQRightCircle(QMatrix4x4 *matrix, float offset);

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
    //cylindre
    double list1Size = 7;
    QList<Cylindre> cylPosList1;            // cylindre en ligne droite en haut

    double list2Size = 7;
    QList<Cylindre> cylPosList2;            // cylindre en ligne droite en bas

    double list3Size = 12;
    QList<Cylindre> cylPosList3;            // cylindre autour de la grande roue

    double list4Size = 5;
    QList<Cylindre> cylPosList4;            // cylindr eautour de la petite roue

    //maillons
    double maillList1Size = 7;
    QList<Maillon> maillPosList1;            // cylindre en ligne droite en haut

    double maillList2Size = 7;
    QList<Maillon> maillPosList2;            // cylindre en ligne droite en bas

    double maillList3Size = 12;
    QList<Maillon> maillPosList3;            // cylindre autour de la grande roue

    double maillList4Size = 5;
    QList<Maillon> maillPosList4;            // cylindr eautour de la petite roue


public:
    QVector3D posHautGenMaillLine = QVector3D(2.5, 10, 0);   // points obtenue par le calcul, puis arrondi
    QVector3D posHautDestMaillLine = QVector3D(22, 5, 0);    // points obtenue par le calcul, puis arrondi
    QVector3D posBasGenMaillLine = QVector3D(2.5, -10, 0);   // points obtenue par le calcul, puis arrondi
    QVector3D posBasDestMaillLine = QVector3D(22, -5, 0);    // points obtenue par le calcul, puis arrondi

    QVector3D posHautGenMaillLine2 = QVector3D(1.4, 5, 0);   // points obtenue par le calcul, puis arrondi
    QVector3D posHautDestMaillLine2 = QVector3D(11, 2.5, 0);    // points obtenue par le calcul, puis arrondi
    QVector3D posBasGenMaillLine2 = QVector3D(2, -5, 0);   // points obtenue par le calcul, puis arrondi
    QVector3D posBasDestMaillLine2 = QVector3D(11, -2.5, 0);    // points obtenue par le calcul, puis arrondi
};

#endif // GLAREA_H
