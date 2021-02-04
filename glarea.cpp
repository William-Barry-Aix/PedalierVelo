// CC-BY Edouard.Thiel@univ-amu.fr - 22/01/2019

#include "glarea.h"
#include <math.h>       /* cos */
#include <QDebug>
#include <QSurfaceFormat>
#include <QMatrix4x4>
#include "cylindre.h"
#define PI 3.14159265

static const QString vertexShaderFile   = ":/basic.vsh";
static const QString fragmentShaderFile = ":/basic.fsh";


GLArea::GLArea(QWidget *parent) :
    QOpenGLWidget(parent)
{
    qDebug() << "init GLArea" ;

    QSurfaceFormat sf;
    sf.setDepthBufferSize(24);
    sf.setSamples(16);  // nb de sample par pixels : suréchantillonnage por l'antialiasing, en décalant à chaque fois le sommet
                        // cf https://www.khronos.org/opengl/wiki/Multisampling et https://stackoverflow.com/a/14474260
    setFormat(sf);
    qDebug() << "Depth is"<< format().depthBufferSize();

    setEnabled(true);  // événements clavier et souris
    setFocusPolicy(Qt::StrongFocus); // accepte focus
    setFocus();                      // donne le focus


    m_timer = new QTimer(this);
    m_timer->setInterval(50);  // msec
    connect (m_timer, SIGNAL(timeout()), this, SLOT(onTimeout()));
    connect (this, SIGNAL(radiusChanged(double)), this, SLOT(setRadius(double)));
}

GLArea::~GLArea()
{
    qDebug() << "destroy GLArea";

    delete m_timer;

    // Contrairement aux méthodes virtuelles initializeGL, resizeGL et repaintGL,
    // dans le destructeur le contexte GL n'est pas automatiquement rendu courant.
    makeCurrent();

    // ici destructions de ressources GL

    doneCurrent();
}


void GLArea::initializeGL()
{
    qDebug() << __FUNCTION__ ;
    initializeOpenGLFunctions();
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    // shaders
    m_program = new QOpenGLShaderProgram(this);
    m_program->addShaderFromSourceFile(QOpenGLShader::Vertex, vertexShaderFile);  // compile
    m_program->addShaderFromSourceFile(QOpenGLShader::Fragment, fragmentShaderFile);
    if (! m_program->link()) {  // édition de lien des shaders dans le shader program
        qWarning("Failed to compile and link shader program:");
        qWarning() << m_program->log();
    }

    // récupère identifiants de "variables" dans les shaders
    m_posAttr = m_program->attributeLocation("posAttr");
    m_colAttr = m_program->attributeLocation("colAttr");
    m_matrixUniform = m_program->uniformLocation("matrix");
}

void GLArea::resizeGL(int w, int h)
{
    qDebug() << __FUNCTION__ << w << h;

    // C'est fait par défaut
    glViewport(0, 0, w, h);

    m_ratio = (double) w / h;
    // doProjection();
}

void GLArea::paintGL()
{
    qDebug() << __FUNCTION__ ;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    QVector3D G = QVector3D(0,0,0);
    QVector3D K = QVector3D(-2,0,0);
    QVector3D J = QVector3D(0,0,0);
    QVector3D H = QVector3D(0,0,0);

    float GH = 0.5;
    float HJ = GH+0.3;


    float x = cos ( m_alpha *360 * PI / 180.0 )* GH;
    float y = sin ( m_alpha *360 * PI / 180.0 )* GH;
    H.setX(x);
    H.setY(y);

    float beta = (H.y()/HJ)*180/PI;
    x = cos ( beta * PI / 180.0 )* HJ;
    J.setX(H.x()-x);

    float KJ = K.x()-J.x();


    m_program->bind(); // active le shader program

    QMatrix4x4 matrix;
    GLfloat hr = m_radius, wr = hr * m_ratio;            // = glFrustum
    matrix.frustum(-wr, wr, -hr, hr, m_near-m_distCam, m_far-m_distCam);
    //matrix.perspective(60.0f, m_ratio, 0.1f, 100.0f);  // = gluPerspective

    // Remplace gluLookAt (0, 0, 3.0, 0, 0, 0, 0, 1, 0);
    matrix.translate(0, 0, -3.0);

    // Rotation de la scène pour l'animation
    matrix.rotate(m_angle, 0, 1, 0);

    m_program->setUniformValue(m_matrixUniform, matrix);
    y = 0.9-m_anim;

    Cylindre *cyl = new Cylindre(1, 1, 30, 255, 0, 255);
    glVertexAttribPointer(m_posAttr, 3, GL_FLOAT, GL_FALSE, 0, cyl->vertices);

    glEnableVertexAttribArray(m_posAttr);  // rend le VAO accessible pour glDrawArrays
    glEnableVertexAttribArray(m_colAttr);

    cyl->setColors(QVector3D(0,255,255));
    glVertexAttribPointer(m_colAttr, 3, GL_FLOAT, GL_FALSE, 0, cyl->colors);
    QMatrix4x4 cyleMat = matrix;
    cyleMat.scale(GH+0.2,GH+0.2,0.25);
    cyleMat.rotate(-m_alpha*360, 0, 0, 1);
    cyl->draw(m_program, cyleMat,  m_matrixUniform);

    // Axe O
    cyleMat = matrix;
    cyleMat.translate(0,0,-0.3);
    cyleMat.scale(1,1,0.25);
    cyleMat.scale(0.1,0.1,4);
    cyleMat.rotate(-m_alpha*360, 0, 0, 1);
    cyl->setColors(QVector3D(255,0,255));
    glVertexAttribPointer(m_colAttr, 3, GL_FLOAT, GL_FALSE, 0, cyl->colors);
    cyl->draw(m_program, cyleMat,  m_matrixUniform);

    //H
    cyleMat = matrix;
    cyleMat.translate(H.x(),H.y(), 0.25);
    cyleMat.scale(1,1,0.25);
    cyleMat.scale(0.08,0.08, 1);
    cyl->setColors(QVector3D(0,255,255));
    glVertexAttribPointer(m_colAttr, 3, GL_FLOAT, GL_FALSE, 0, cyl->colors);
    cyl->draw(m_program, cyleMat,  m_matrixUniform);

    //Bout
    cyl->setColors(QVector3D(0,255,0));
    glVertexAttribPointer(m_colAttr, 3, GL_FLOAT, GL_FALSE, 0, cyl->colors);

    QMatrix4x4 boutMat = cyleMat;
    boutMat.scale(2, 2, 0.7);

    cyl->draw(m_program, boutMat,  m_matrixUniform);

    //Bras
    cyl->setColors(QVector3D(0,255,0));
    glVertexAttribPointer(m_colAttr, 3, GL_FLOAT, GL_FALSE, 0, cyl->colors);
    cyleMat = matrix;
    cyleMat.translate(H.x(),H.y(), 0.25);
    cyleMat.rotate(90,0,1,0);
    cyleMat.rotate(-beta, 1, 0, 0);
    cyleMat.translate(0,0,-HJ/2);
    cyleMat.scale(0.08,0.08,HJ);
    cyl->draw(m_program, cyleMat,  m_matrixUniform);

    cyleMat = matrix;
    cyleMat.translate(J.x(),J.y(), 0.35);
    cyleMat.scale(1,1,0.4);
    cyleMat.scale(0.08,0.08, 1);

    cyl->draw(m_program, cyleMat,  m_matrixUniform);

    //Bout
    boutMat = cyleMat;
    boutMat.scale(2, 2, 0.4);
    boutMat.translate(0, 0, -0.65);

    cyl->draw(m_program, boutMat,  m_matrixUniform);


    cyl->setColors(QVector3D(220, 63, 220));
    glVertexAttribPointer(m_colAttr, 3, GL_FLOAT, GL_FALSE, 0, cyl->colors);
    boutMat.translate(0,0,1.3);
    cyl->draw(m_program, boutMat,  m_matrixUniform);

    cyleMat = matrix;
    cyleMat.translate(K.x()-KJ/2, 0, 0.45);
    cyleMat.rotate(90, 0, 1, 0);
    cyleMat.scale(0.08,0.08,KJ);
    cyl->draw(m_program, cyleMat,  m_matrixUniform);

    cyleMat = matrix;
    cyleMat.rotate(-90, 0, 1, 0);
    cyleMat.translate(0.5, 0, -K.x()+0.5);
    cyleMat.scale(0.25, 0.25, 1);
    cyl->draw(m_program, cyleMat,  m_matrixUniform);


    glDisableVertexAttribArray(m_posAttr);
    glDisableVertexAttribArray(m_colAttr);

    m_program->release();



    cyl = nullptr;

}


void GLArea::keyPressEvent(QKeyEvent *ev)
{
    qDebug() << __FUNCTION__ << ev->text();

    switch(ev->key()) {
        case Qt::Key_Space :
            m_angle += 1;
            if (m_angle >= 360) m_angle -= 360;
            update();
            break;
        case Qt::Key_A :
            if (m_timer->isActive())
                m_timer->stop();
            else m_timer->start();
            break;
        case Qt::Key_R :
            if (ev->text() == "r")
                 setRadius(m_radius-0.05);
            else setRadius(m_radius+0.05);
            break;
    }
}

void GLArea::keyReleaseEvent(QKeyEvent *ev)
{
    qDebug() << __FUNCTION__ << ev->text();
}

void GLArea::mousePressEvent(QMouseEvent *ev)
{
    qDebug() << __FUNCTION__ << ev->x() << ev->y() << ev->button();
}

void GLArea::mouseReleaseEvent(QMouseEvent *ev)
{
    qDebug() << __FUNCTION__ << ev->x() << ev->y() << ev->button();
}

void GLArea::mouseMoveEvent(QMouseEvent *ev)
{
    qDebug() << __FUNCTION__ << ev->x() << ev->y();
}

void GLArea::onTimeout()
{
    qDebug() << __FUNCTION__ ;
    m_alpha += 0.02;
    if (m_alpha > 1) m_alpha = 0;
    update();
}

void GLArea::setRadius(double radius)
{
    qDebug() << __FUNCTION__ << radius << sender();
    if (radius != m_radius && radius > 0.01 && radius <= 10) {
        m_radius = radius;
        qDebug() << "  emit radiusChanged()";
        emit radiusChanged(radius);
        update();
    }
}




