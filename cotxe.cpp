#include "cotxe.h"

Cotxe::Cotxe(vector<point4> vertexs, vector<Cara> cares, GLfloat mida, GLfloat xorig, GLfloat yorig, GLfloat zorig,
             double xrot, double yrot, double zrot,
             float xdirec, float ydirec, float zdirec) : Objecte(vertexs, cares, mida, xorig, yorig, zorig, xrot, yrot, zrot) {
    this->direction[0] = -1;
    this->direction[1] = 0;
    this->direction[2] = 0;
    this->direction[3] = 0;

    // El seguent codi escala el cotxe entre 0 i 1 i el situa el seu centre  0,0,0. aixo fa que es vegi en la primera visualització
    //
    // Cal modificar el codi seguent


}

void Cotxe::make() {
    for (int i = 0; i < 4; i++) {
        rodes[i]->make();
    }
    if(carrosseria != NULL)
        carrosseria->make();

    double escalaX = 1.0 / 4.6;
    mat4 m= Translate(-1.93*escalaX, (+0.26)*escalaX, -2.16*escalaX)*Scale(escalaX, escalaX, escalaX)*Translate(+1.93, -0.26, 2.16);

    aplicaTG(m);
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
}

void Cotxe::backward(){
    // Metode a implementar per fer el moviment del cotxe

}

void Cotxe::turnleft(){
    // Metode a implementar per fer el moviment del cotxe

}

void Cotxe::turnright(){
    // Metode a implementar per fer el moviment del cotxe

}
