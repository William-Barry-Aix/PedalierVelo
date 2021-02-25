#include "glarea.h"
#include <math.h>
#include <QDebug>
#include <QSurfaceFormat>
#include <QMatrix4x4>
#include "roue.h"
#define PI 3.14159265

static const QString vertexShaderFile   = ":/vertex.glsl";
static const QString fragmentShaderFile = ":/fragment.glsl";


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

    //cylindre

}

GLArea::~GLArea()
{
    qDebug() << "destroy GLArea";

    delete m_timer;

    //delete cyl;
    //delete cyl1;
    //delete cyl2;
    //delete cyl3;

    // Contrairement aux méthodes virtuelles initializeGL, resizeGL et repaintGL,
    // dans le destructeur le contexte GL n'est pas automatiquement rendu courant.
    makeCurrent();

    // ici destructions de ressources GL
    tearGLObjects();

    doneCurrent();
}


void GLArea::initializeGL()
{
    //qDebug() << __FUNCTION__ ;

    initializeOpenGLFunctions();

    glEnable(GL_DEPTH_TEST);
    makeGLObjects();

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
    qDebug() << m_ratio;
    m_ratio = (double) w / h;
    // doProjection();
}

void GLArea::paintGL()
{
    //qDebug() << __FUNCTION__ ;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    QOpenGLFunctions *glFuncs = context()->functions();  // cf initializeGL

    m_program->bind(); // active le shader program

    // Projection
    QMatrix4x4 proj_mat;
    GLfloat hr = m_radius, wr = hr * m_ratio;            // = glFrustum
    qDebug() << hr << " " << wr;
    proj_mat.frustum(-wr, wr, -hr, hr, m_near, m_far);

    // Caméra
    QMatrix4x4 cam_mat;
    cam_mat.translate(0, 0, -m_distCam);

    // Rotation de la scène pour l'animation
    QMatrix4x4 matrix;
    matrix.translate(0, 0, -1.5);
    matrix.rotate(m_angle, 0, 1, 0);

    QMatrix3x3 normal_mat = matrix.normalMatrix();

    m_program->setUniformValue("projMatrix", proj_mat);
    m_program->setUniformValue("mvMatrix", matrix*cam_mat);
    m_program->setUniformValue("norMatrix", normal_mat);

    QMatrix4x4 cyleMat;


    // Roue 1
    cyleMat = matrix;
    cyleMat.translate(0,0,0);
    cyleMat.rotate(-m_alpha*360, 0, 0, 1);
    roue1->draw(m_program, cyleMat, cam_mat, glFuncs);


    // Roue 2
    cyleMat = matrix;
    cyleMat.translate(2,0,0);
    cyleMat.rotate(-m_alpha*180, 0, 0, 1);
    roue2->draw(m_program, cyleMat, cam_mat, glFuncs);

    //cylindre

    cyleMat = matrix;
    //cyleMat.translate(0,0,0.5);
    cyleMat.scale(0.2,0.2,0.25);

    moveCylHautline(&cyleMat);

    cyl->draw(m_program, cyleMat, cam_mat, glFuncs);


    //cylindre1

    cyleMat = matrix;
    cyleMat.scale(0.2,0.2,0.25);

    moveCylBasline(&cyleMat);

    cyl1->draw(m_program, cyleMat, cam_mat, glFuncs);

    //cylindre2

    cyleMat = matrix;
    cyleMat.scale(0.2,0.2,0.25);

    moveCylLeftCircle(&cyleMat);

    cyl2->draw(m_program, cyleMat, cam_mat, glFuncs);

    //cylindre3

    cyleMat = matrix;
    cyleMat.scale(0.2,0.2,0.25);

    moveCylQRightCircle(&cyleMat);

    cyl3->draw(m_program, cyleMat, cam_mat, glFuncs);

    //maillon

    cyleMat = matrix;
    cyleMat.scale(0.2,0.2,0.2);

    maillon->draw(m_program, cyleMat, cam_mat, glFuncs);


    //end
    m_program->release();

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
        case Qt::Key_D :
            if (!m_timer->isActive())
                m_timer->start();
            break;
        case Qt::Key_S :
            if (m_timer->isActive())
                m_timer->stop();
            break;
        case Qt::Key_A :
            if (shiftPress || lockPress)
                vitesse += 0.01;
            else
                vitesse -= 0.01;

            if (vitesse < 0) { vitesse = 0; }
            break;
        case Qt::Key_R :
            if (ev->text() == "r")
                 setRadius(m_radius-0.05);
            else setRadius(m_radius+0.05);
            break;
        case Qt::Key_Shift :
            shiftPress = true;
            break;
        case Qt::Key_CapsLock :
            lockPress = true;
            break;
    }
}

void GLArea::keyReleaseEvent(QKeyEvent *ev)
{
    qDebug() << __FUNCTION__ << ev->text();

    switch(ev->key()) {
        case Qt::Key_Shift :
            shiftPress = false;
            break;
        case Qt::Key_CapsLock :
            lockPress = false;
            break;

    }
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
    //qDebug() << __FUNCTION__ ;
    m_alpha += vitesse;
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

void GLArea::setTransforms(QMatrix4x4 &cam_mat, QMatrix4x4 &shape_mat)
{
    QMatrix4x4 mv_mat = cam_mat*shape_mat;
    m_program->setUniformValue("mvMatrix", mv_mat);

    QMatrix3x3 nor_mat = shape_mat.normalMatrix();
    m_program->setUniformValue("norMatrix", nor_mat);
}

void GLArea::makeGLObjects()
{
    roue1 = new Roue(0.1, 1, 20, -1, 255, 255, 255);
    roue2 = new Roue(0.1, 0.5, 10, 2, 255, 255, 255);


    cyl = new Cylindre(1, 1, 30, 255, 0, 255);
    cyl1 = new Cylindre(1, 1, 30, 255, 0, 255);
    cyl2 = new Cylindre(1, 1, 30, 255, 0, 255);
    cyl3 = new Cylindre(1, 1, 30, 255, 0, 255);

    maillon = new Maillon(1, 2, 1, 0.1, 0, 0, 255, 0); //Maillon(double z_ep, double x_width, double y_height, double bevel_size, float origin, float coul_r, float coul_v, float coul_b);
}


void GLArea::tearGLObjects()
{
    //delete roue1;
    //delete roue2;
}
void GLArea::moveCylHautline(QMatrix4x4 *matrix) {
    QVector3D newPos = QVector3D((posHautGenMaillLine*(abs(1-m_alpha)) + posHautDestMaillLine*m_alpha));
    matrix->translate(newPos);
}

void GLArea::moveCylBasline(QMatrix4x4 *matrix) {
    QVector3D newPos = QVector3D((posBasGenMaillLine*m_alpha + posBasDestMaillLine*(abs(1-m_alpha))));
    matrix->translate(newPos);
}

void GLArea::moveCylLeftCircle(QMatrix4x4 *matrix) {
    QVector3D newPos = QVector3D(-5*sin(m_alpha*M_PI*1.2) + posHautGenMaillLine.x()-3, -5*cos(m_alpha*M_PI*1.2),posHautGenMaillLine.z());
    matrix->translate(newPos);
}

void GLArea::moveCylQRightCircle(QMatrix4x4 *matrix) {
    QVector3D newPos = QVector3D(2.5*sin(m_alpha*M_PI*1.2) + posHautDestMaillLine.x()+1, 2.5*cos(m_alpha*M_PI*1.2),posHautGenMaillLine.z());
    matrix->translate(newPos);
}
