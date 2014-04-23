#include "cotxe.h"

Cotxe::Cotxe(vector<point4> vertexs, vector<Cara> cares, GLfloat mida, GLfloat xorig, GLfloat yorig, GLfloat zorig,
             double xrot, double yrot, double zrot,
             float xdirec, float ydirec, float zdirec) : Objecte(vertexs, cares, mida, xorig, yorig, zorig, xrot, yrot, zrot) {
    this->xdirec = xdirec;
    this->ydirec = 0;
    this->zdirec = zdirec;
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

    vec3 u;
    u[0] = this->xdirec;
    u[1] = this->zdirec;

    vec3 v;
    v[0] = -1;
    v[1] = 0;
    if(xdirec != 0 || zdirec != 0) {
        double prod = v[0]*u[0]+v[1]*u[1];
        double mod_u = sqrt(u[0]*u[0]+u[1]*u[1]);
        double mod_v = sqrt(v[0]*v[0]+v[1]*v[1]);
        this->rotateCotxe = acos(prod/(mod_u*mod_v)) * 180.0 / M_PI;
    }else {
        this->rotateCotxe = 0;
    }

    this->rotateRodes = 0;

    GLfloat scale = tam/capsa.max_size;
    mat4 m = Translate(xorig, scale*capsa.h/2.0, zorig)*RotateY(rotateCotxe)*Scale(scale,scale,scale)*Translate(-capsa.pmig);
    this->aplicaTG(m);
}

void Cotxe::addComponent(Objecte *obj) {
    /*
     *        __
     *       |  |
     *   1 |-|  |-| 0
     *       /  \
     *      /    \
     *     /      \
     *     |      |
     *     |      |
     *     |      |
     *     |      |
     * 3 |-|      |-| 2
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

void Cotxe::restorePoints() {
    for (int i = 0; i < 4; i++) {
        rodes[i]->restorePoints();
    }
    if(carrosseria != NULL)
        carrosseria->restorePoints();
}

void Cotxe::backupPoints() {
    for (int i = 0; i < 4; i++) {
        rodes[i]->backupPoints();
    }
    if(carrosseria != NULL)
        carrosseria->backupPoints();
}

void Cotxe::aplicaTG(mat4 m) {
    for (int i = 0; i < 4; i++) {
        rodes[i]->aplicaTG(m);
    }
    if(carrosseria != NULL)
        carrosseria->aplicaTG(m);
}

void Cotxe::aplicaTGCentrat(mat4 m) {
    Capsa3D capsa = calculCapsa3D();
     mat4 mo = m;
     mat4 mo2 = mo;
    if(rotateCotxe != 0)
        mo = RotateY(this->rotateCotxe)*m*RotateY(-this->rotateCotxe); //Rotem el cotxe al punt original
    if(rotateRodes != 0)
        mo2 = RotateY(this->rotateRodes)*mo*RotateY(-this->rotateRodes); //Rotem les rodes al punt original
    mat4 mo3 = Translate(capsa.pmig)*mo*Translate(-capsa.pmig); //apliquem els canvis respecte al centre del cotxe
    mat4 mo4 = Translate(capsa.pmig)*mo2*Translate(-capsa.pmig);
    rodes[0]->aplicaTG(mo4);
    rodes[1]->aplicaTG(mo4);
    rodes[2]->aplicaTG(mo3);
    rodes[3]->aplicaTG(mo3);
    if(carrosseria != NULL)
        carrosseria->aplicaTG(mo3);
}

void Cotxe::aplicaTGPoints(mat4 m) {
    for (int i = 0; i < 4; i++) {
        rodes[i]->aplicaTGPoints(m);
    }
    if(carrosseria != NULL)
        carrosseria->aplicaTGPoints(m);
}

void Cotxe::aplicaTGRodes(mat4 m) {
    mat4 mo = RotateY(this->rotateCotxe)*m*RotateY(-this->rotateCotxe);
    mat4 mo2 = RotateY(this->rotateRodes)*mo*RotateY(-this->rotateRodes);

    rodes[0]->aplicaTGCentrat(mo2);
    rodes[1]->aplicaTGCentrat(mo2);
    rodes[2]->aplicaTGCentrat(mo);
    rodes[3]->aplicaTGCentrat(mo);
}

void Cotxe::draw() {
    for (int i = 0; i < 4; i++) {
        rodes[i]->draw();
    }
    if(carrosseria != NULL)
        carrosseria->draw();
}

bool Cotxe::isCollision(Objecte *obj) {
    bool trobat = false;
    for (int i = 0; i < 4; i++) {
        trobat = rodes[i]->isCollision(obj);
        if(trobat) return true;
    }
    if(carrosseria != NULL)
        return carrosseria->isCollision(obj);
    return false;
}

void Cotxe::forward(){
    // Metode a implementar per fer el moviment del cotxe

    double vel = -0.06;
    aplicaTG(Translate(vel*cos(rotateCotxe*M_PI/180.0),0,-vel*sin(rotateCotxe*M_PI/180.0)));

    if(rotateRodes != 0) {
        float rot = rotateRodes > 0? 5:-5;
        aplicaTGCentrat(RotateY(rot));
        rotateCotxe += rot;
        rotateCotxe = (int)rotateCotxe%360;
    }

    /*Fer moure les rodes*/
     //aplicaTGRodes(RotateZ(10));
    aplicaTGRodes(RotateZ(10));
}

void Cotxe::backward(){
    // Metode a implementar per fer el moviment del cotxe

    double vel = 0.06;
    aplicaTG(Translate(vel*cos(rotateCotxe*M_PI/180.0),0,-vel*sin(rotateCotxe*M_PI/180.0)));

    if(rotateRodes != 0) {
        float rot = rotateRodes > 0? -5:5;
        aplicaTGCentrat(RotateY(rot));
        rotateCotxe += rot;
        rotateCotxe = (int)rotateCotxe%360;
    }

    /*Fer moure les rodes*/
    //aplicaTGRodes(RotateZ(-10));
    aplicaTGRodes(RotateZ(-10));
}

void Cotxe::turnleft(){
    // Metode a implementar per fer el moviment del cotxe
    rotateRodes += 5;
    if(rotateRodes<=25) {
        rodes[0]->aplicaTGCentrat(RotateY(5));
        rodes[1]->aplicaTGCentrat(RotateY(5));
    }else{
        rotateRodes = 25;
    }
    rotateRodes = (int)rotateRodes%360;
}

void Cotxe::turnright(){
    // Metode a implementar per fer el moviment del cotxe
    rotateRodes -= 5;
    if(rotateRodes>=-25) {
        rodes[0]->aplicaTGCentrat(RotateY(-5));
        rodes[1]->aplicaTGCentrat(RotateY(-5));
    }else{
        rotateRodes = -25;
    }
    rotateRodes = (int)rotateRodes%360;
}
