#-------------------------------------------------
#
# Project created by QtCreator 2019-01-12T12:27:11
#
#-------------------------------------------------

QT += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++14

unix {
    LIBS += -lGLU
}
win32 {
    LIBS += -lGLU32 -lOpengl32
}

TARGET = TP3
TEMPLATE = app


SOURCES += main.cpp\
        chaine.cpp \
        cylindre.cpp \
        forme.cpp \
        frustumdialog.cpp \
        maillon.cpp \
        princ.cpp \
        glarea.cpp \
        roue.cpp

HEADERS  += princ.h \
        chaine.h \
        cylindre.h \
        forme.h \
        frustumdialog.h \
        glarea.h \
        maillon.h \
        roue.h

FORMS    += princ.ui \
    frustumdialog.ui

RESOURCES += \
    TP3.qrc

DISTFILES += \
    fragment.glsl \
    vertex.glsl
