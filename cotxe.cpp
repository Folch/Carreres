#include "cotxe.h"

Cotxe::Cotxe(vector<point4> vertexs, vector<Cara> cares, GLfloat mida, GLfloat xorig, GLfloat yorig, GLfloat zorig,
             double xrot, double yrot, double zrot,
             float xdirec, float ydirec, float zdirec) : Objecte(vertexs, cares, mida, xorig, yorig, zorig, xrot, yrot, zrot) {
    this->direction[0] = -1;
    this->direction[1] = 0;
    this->direction[2] = 0;
    this->direction[3] = 0;
}

Capsa3D Cotxe::calculCapsa3D(){
    points = new point4[10];
    numPoints = 10;
    Index = 10;
    Capsa3D capsa;
    for (int i = 0; i < 8; i+=2) {
        capsa = rodes[i/2]->calculCapsa3D();
        points[i] = point4(capsa.pmin.x, capsa.pmin.y, capsa.pmin.z, 1.0f);
        points[i+1] = point4(capsa.pmin.x+capsa.a, capsa.pmin.y+capsa.h, capsa.pmin.z+capsa.p, 1.0f);
    }

    if(carrosseria != NULL) {
        capsa = carrosseria->calculCapsa3D();
        points[8] = point4(capsa.pmin.x, capsa.pmin.y, capsa.pmin.z, 1.0f);
        points[9] = point4(capsa.pmin.x+capsa.a, capsa.pmin.y+capsa.h, capsa.pmin.z+capsa.p, 1.0f);
    }

    return Objecte::calculCapsa3D();
}

void Cotxe::make() {
    for (int i = 0; i < 4; i++) {
        rodes[i]->make();
    }
    if(carrosseria != NULL)
        carrosseria->make();

    Capsa3D capsa = calculCapsa3D();

    //translate to (0,0,0)
    //scale
    //translate (xorig,yorig,zorig)

    GLfloat escale = tam/capsa.max_size;
    mat4 m= Translate(xorig, yorig, zorig)*Scale(escale,escale,escale)*Translate(-capsa.pmig_xz.x, -capsa.pmig_xz.y, -capsa.pmig_xz.z);
    this->aplicaTG(m);
}

void Cotxe::addComponent(Objecte *obj) {
    /*
     *        __
     *       |  |
     *   0 |-|  |-| 1
     *       /  \
     *      /    \
     *     /      \
     *     |      |
     *     |      |
     *     |      |
     *     |      |
     * 2 |-|      |-| 3
     *     |______|
     *
     *
     */
    if(dynamic_cast<Roda*>(obj)){
        if(((Roda*)obj)->type == ESQUERRA_POSTERIOR){
            rodes[2] = (Roda*)obj;
        }else if(((Roda*)obj)->type == DRETA_POSTERIOR){
            rodes[3] = (Roda*)obj;
        }else if(((Roda*)obj)->type == ESQUERRA_DAVANTERA){
            rodes[0] = (Roda*)obj;
        }else if(((Roda*)obj)->type == DRETA_DAVANTERA){
            rodes[1] = (Roda*)obj;
        }
    }else
        carrosseria =(Carrosseria*) obj; //potser s'ha de borrar l'anterior carrosseria
}

void Cotxe::toGPU(QGLShaderProgram *p) {
    for (int i = 0; i < 4; i++) {
        rodes[i]->toGPU(p);
    }
    if(carrosseria != NULL)
        carrosseria->toGPU(p);
}

void Cotxe::aplicaTG(mat4 m) {
    for (int i = 0; i < 4; i++) {
        rodes[i]->aplicaTG(m);
    }
    if(carrosseria != NULL)
        carrosseria->aplicaTG(m);
}

void Cotxe::aplicaTGCentrat(mat4 m) {
    for (int i = 0; i < 4; i++) {
        rodes[i]->aplicaTGCentrat(m);
    }
    if(carrosseria != NULL)
        carrosseria->aplicaTGCentrat(m);
}

void Cotxe::aplicaTGPoints(mat4 m) {
    for (int i = 0; i < 4; i++) {
        rodes[i]->aplicaTGPoints(m);
    }
    if(carrosseria != NULL)
        carrosseria->aplicaTGPoints(m);
}

void Cotxe::draw() {
    for (int i = 0; i < 4; i++) {
        rodes[i]->draw();
    }
    if(carrosseria != NULL)
        carrosseria->draw();
}

void Cotxe::forward(){
    // Metode a implementar per fer el moviment del cotxe

    /*Fer moure les rodes*/
    for(int i = 0; i<4; i++) {
        Capsa3D capsa = rodes[i]->calculCapsa3D();
        mat4 m = Translate(capsa.pmig.x, capsa.pmig.y, capsa.pmig.z)*RotateZ(10)*Translate(-capsa.pmig.x, -capsa.pmig.y, -capsa.pmig.z);
        rodes[i]->aplicaTG(m);
    }
}

void Cotxe::backward(){
    // Metode a implementar per fer el moviment del cotxe

    /*Fer moure les rodes*/
    for(int i = 0; i<4; i++) {
        Capsa3D capsa = rodes[i]->calculCapsa3D();
        mat4 m = Translate(capsa.pmig.x, capsa.pmig.y, capsa.pmig.z)*RotateZ(-10)*Translate(-capsa.pmig.x, -capsa.pmig.y, -capsa.pmig.z);
        rodes[i]->aplicaTG(m);
    }
}

void Cotxe::turnleft(){
    // Metode a implementar per fer el moviment del cotxe

}

void Cotxe::turnright(){
    // Metode a implementar per fer el moviment del cotxe

}
