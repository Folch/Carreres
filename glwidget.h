#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <escena.h>
#include <stdlib.h>
#include "llum.h"

#include <QtOpenGL>
#include <QGLWidget>
#include <QtGui>

#define PROGRAM_VERTEX_ATTRIBUTE 0
#define PROGRAM_NORMAL_ATTRIBUTE 1

class QGLShaderProgram;

class GLWidget : public QGLWidget
{
    Q_OBJECT

public:
    GLWidget(QWidget *parent = 0);
    ~GLWidget();

    QSize minimumSizeHint() const;
    QSize sizeHint() const;

    void newObstacle(int nombre);
    void newCotxe(QString fichero, float xorig, float zorig, float mida, float xdirec, float ydirec, float zdirec);
    void newTerra(float amplaria, float profunditat, float y);

    void resetView();

protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);

    void setXRotation(int angle);
    void setYRotation(int angle);
    void setZRotation(int angle);
    void Pan(int, int);
    void Zoom(int);

private:
    escena *esc;
    QPoint lastPos;
    QColor qtGreen;
    QColor qtPurple;
    mat4   modScalat;
    float  a, p, h;

// Programa de la GPU
    vector<QGLShaderProgram*> programs;

    //  Metode per a carregar de fitxers el vertex i el fragment shader
    void InitShader( const char* vertexShaderFile,
                       const char* fragmentShaderFile,
                       QGLShaderProgram* program );
    // Metode per inicialitzar els shaders de l'aplicacio
    void initShadersGPU();

    // Metode que carrega un nou objecte a l'escena
    void newObjecte(Objecte * obj);

    // Metode per adaptar l'escena al tamany del widget
    void adaptaObjecteTamanyWidget(Objecte *obj);
};


#endif // GLWIDGET_H
