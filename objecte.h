#ifndef OBJECTE_H
#define OBJECTE_H

#define FLAT 0
#define GOURAUD 1

#include <QObject>

#include <vector>

#include <Common.h>
#include <cara.h>
#include <limits>
#include "material.h"

#include <QGLShaderProgram>

typedef Common::vec4  color4;
typedef Common::vec4  point4;

using namespace std;

class Objecte : public QObject
{
    Q_OBJECT
protected:

    // Sistema de coordenades d'un objecte: punt origen i eixos de rotació
    float xRot;
    float yRot;
    float zRot;

    GLfloat tam; // Escala de l'objecte plicada al fitxer d'entrada

    short tipusNormal;

    // Programa de shaders de la GPU

    GLuint buffer; // Buffer de comunicacio amb la GPU

    // Estructures de vertexs i colors per passar a la GPU
    int     numPoints;
    point4 *points;
    point4 *pointsTmp;
    point4 *normals;
    //color4 *colors;
    int Index; // index de control del numero de vertexs a posar a la GPU


public:

    GLfloat xorig, yorig, zorig;
    vector<Cara*> cares; // cares de l'objecte
    vector<point4> vertexs; // vertexs de l'objecte sense repetits
    QHash<int, vector<Cara*> > caresHash;
    QGLShaderProgram *program;

    Material *m;

    float getYOrig();
    // Capsa mínima contenidora de l'objecte
    Capsa3D capsa;

    GLenum mode;

    //explicit Objecte(QObject *parent = 0);
    Objecte(vector<point4> vertexsz, vector<Cara*>,GLfloat, GLfloat,GLfloat,GLfloat,double, double, double);
    Objecte(vector<point4> vertexs, vector<Cara*>);

    Objecte(const int npoints);
    Objecte(const int npoints,GLfloat, GLfloat,GLfloat,GLfloat,double, double, double);

    void init(GLfloat,GLfloat,GLfloat,GLfloat,double, double, double);
    ~Objecte();

    // make(): omple l'estructura de points i colors de l'objecte, inicialitza NumVertices
    // Si l'objecte es construeix procedimentalment es sobrecarrega el make
    virtual void make();

    virtual bool isCollision(Objecte*);

    // Pas generic de vertexs i colors a la GPU
    virtual void toGPU(QGLShaderProgram *p);
    // Pintat amb la GPU
    virtual void draw();

    // Calcula la capsa 3D contenidora de l'objecte
    virtual Capsa3D calculCapsa3D();

    virtual void restorePoints();
    virtual void backupPoints();
    // Aplica una TG qualsevol a un objecte
    virtual void aplicaTG(mat4 m);
    virtual void aplicaTGPoints(mat4 m);
    // Aplica una TG centrada en el punt central de la capsa de l'objecte a un objecte
    virtual void aplicaTGCentrat(mat4 m);

private:
    void construeix_cara ( char **words, int nwords);
    void bindCares();
    void calculaNormals();
    void normalsFlatShading();
    void aplicaTGNormals(mat4 m);

    vec4 getNormalFlatShading(vector<Cara*> cares);
    vec4 getNormalGourond(vector<Cara*> cares);
};



#endif // OBJECTE_H
