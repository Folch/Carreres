#ifndef COTXE_H
#define COTXE_H

#include <Common.h>
#include <objecte.h>
#include <roda.h>
#include <carrosseria.h>
#include <iostream>

using namespace std;

class Cotxe: public Objecte
{
public:
    Cotxe(vector<point4> vertexs, vector<Cara> cares, GLfloat, GLfloat,GLfloat,GLfloat,double, double, double, float,float,float);

    virtual void make();
    virtual void draw();
    virtual Capsa3D calculCapsa3D();
    virtual void restorePoints();
    virtual void backupPoints();
    virtual void aplicaTG(mat4);
    virtual void aplicaTGCentrat(mat4);
    virtual void aplicaTGPoints(mat4);
    void aplicaTGRodes(mat4);
    virtual void toGPU(QGLShaderProgram*);
    virtual bool isCollision(Objecte *);

    void forward();
    void backward();
    void turnleft();
    void turnright();
    float xdirec, ydirec, zdirec;
    void addComponent(Objecte *obj);

private:
    Roda* rodes[4];
    Carrosseria *carrosseria;
    float rotateCotxe;
    float rotateRodes;

};

#endif // COTXE_H
