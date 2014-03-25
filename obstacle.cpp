#include "obstacle.h"

Obstacle::Obstacle(double xorig,double yorig, double zorig, double costat) : Objecte(NumVerticesF){
    
    this->xorig = xorig;
    this->yorig = yorig;
    this->zorig = zorig;
    double m_costat = costat/2.0;
    /*vertexs de base*/
    vertexs.push_back(point4(xorig+m_costat, yorig-m_costat, zorig+m_costat, 1.0));
    vertexs.push_back(point4(xorig+m_costat, yorig-m_costat, zorig-m_costat, 1.0));
    vertexs.push_back(point4(xorig-m_costat, yorig-m_costat, zorig-m_costat, 1.0));
    vertexs.push_back(point4(xorig-m_costat, yorig-m_costat, zorig+m_costat, 1.0));

    /*vertexs superiors*/
    vertexs.push_back(point4(xorig+m_costat, yorig+m_costat, zorig+m_costat, 1.0));
    vertexs.push_back(point4(xorig+m_costat, yorig+m_costat, zorig-m_costat, 1.0));
    vertexs.push_back(point4(xorig-m_costat, yorig+m_costat, zorig-m_costat, 1.0));
    vertexs.push_back(point4(xorig-m_costat, yorig+m_costat, zorig+m_costat, 1.0));

    tam = costat;

    mode = GL_TRIANGLES;
}

Obstacle::Obstacle(vector<point4> vertexs, vector<Cara> cares) : Objecte(vertexs,cares){
    mode = GL_TRIANGLES;
}

void Obstacle::make() {
    quad(0,1,2,3);
    quad(4,5,6,7);
    quad(0,4,7,3);
    quad(0,1,5,4);
    quad(1,2,6,5);
    quad(2,3,7,6);

    Objecte::make();

    Capsa3D capsa = calculCapsa3D();

    //translate to (0,0,0)
    //scale
    //translate (xorig,yorig,zorig)

    GLfloat scale = tam/capsa.max_size;
    mat4 m = Translate(xorig, scale*capsa.h/2.0, zorig)*Scale(scale,scale,scale)*Translate(-capsa.pmig.x, -capsa.pmig.y, -capsa.pmig.z);
    this->aplicaTG(m);
}


void Obstacle::quad(int a, int b, int c, int d) {
    cares.push_back(Cara(a,b,c));
    cares.push_back(Cara(a,c,d));
}
