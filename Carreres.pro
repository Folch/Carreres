#-------------------------------------------------
#
# Project created by QtCreator 2013-03-05T16:12:33
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Carreres
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    glwidget.cpp \
    objecte.cpp \
    Common.cpp \
    cara.cpp \
    escena.cpp \
    cotxe.cpp \
    newcotxedialog.cpp \
    newobstacledialog.cpp \
    newterradialog.cpp \
    terra.cpp \
    readobject.cpp \
    roda.cpp \
    carrosseria.cpp \
    obstacle.cpp \
    camera.cpp \
    material.cpp \
    llum.cpp \
    conjuntllums.cpp

HEADERS  += mainwindow.h \
    glwidget.h \
    objecte.h \
    Common.h \
    cara.h \
    vec.h \
    mat.h \
    escena.h \
    cotxe.h \
    newcotxedialog.h \
    newobstacledialog.h \
    newterradialog.h \
    readfile.h \
    terra.h \
    readobject.h \
    roda.h \
    carrosseria.h \
    obstacle.h \
    camera.h \
    material.h \
    llum.h \
    conjuntllums.h

FORMS    += mainwindow.ui \
    newcotxedialog.ui \
    newterradialog.ui \
    newobstacledialog.ui

OTHER_FILES += \
    vshadercotxe.glsl \
    fshadercotxe.glsl \
    vshadergeneral.glsl \
    fshadergeneral.glsl
