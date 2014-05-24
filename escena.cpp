#include <escena.h>

using namespace std;

escena::escena(Camera *c) {
    camera = c;
    cameras[c->name] = c;

    myCar = NULL;
    terra = NULL;
}


escena::~escena() {
    printf("DESTRUCTOR!");
}

/*
 * Método para añadir una nueva cámara
 */

void escena::addCamera(Camera *c) {
    this->cameras[c->name] = c;
}

/*
 * Con esta función cambiaremos la cámara.
 * Hay que poner el nombre que tiene la cámara, para cambiarla
 *
 */
void escena::applyCamera(QString name) {
    if (cameras.contains(name)) {
        this->camera = cameras.value(name);
    }
    //Si no, se queda la camara que ya había.
}

void escena::actualitzaCameraPanoramica(bool clip) {
    if(clip) {
        camera->CalculWindowAmbRetallat();
    }
    camera->CalculaMatriuProjection();
}

void escena::resetCameraPanoramica() {
    Camera *c = cameras["Paralela"];
    Capsa3D capsa = CapsaMinCont3DEscena();
    c->piram.proj = PARALLELA;

    c->piram.d = sqrt(capsa.a*capsa.a+capsa.h*capsa.h+capsa.p*capsa.p);
    c->piram.dant = capsa.max_size*0.4f;
    c->piram.dpost = c->piram.d+capsa.max_size/2.0;//piram.dant+capsa.max_size;
    c->vs.angx = -90;
    c->vs.angy = 0;
    c->setVRP(capsa);
    c->setOBS(capsa);
}

void escena::actualitzaCameraThirdPerson() {
    if(myCar == NULL) return;
    /*Si la camara no es en tercera persona, no ha de seguir al cotxe*/
    if(camera->name != "Tercera") return;
    Capsa3D capsa = myCar->calculCapsa3D();
    camera->vs.angy = myCar->rotateCotxe+90;
    camera->setVRP(capsa);
    camera->setOBS(capsa);
}

void escena::iniLookAtCotxe() {
    Camera *c = cameras["Tercera"];
    /*set params of camera*/
    Capsa3D capsa = myCar->calculCapsa3D();
    c->piram.proj = PERSPECTIVA;
    c->piram.d = capsa.max_size*0.6f;
    c->piram.dant = capsa.max_size*0.2f;
    c->piram.dpost = c->piram.d+capsa.max_size/2.0;//piram.dant+capsa.max_size;
    c->vs.angx = -20;
    c->vs.angy = myCar->rotateCotxe+90;
    c->setVRP(capsa);
    c->setOBS(capsa);
    //c->AmpliaWindow(70);
    //c->CalculaMatriuProjection();
}

void escena::addLand(Terra *t) {
    if(this->terra!=NULL)
        delete this->terra;
    this->terra = t;
}

void escena::addObjecte(Objecte *obj) {
    /* El primer cotxe que entri per aquí, serà el cotxe que controlaré mitjançant el pad */
    if(dynamic_cast<Cotxe*>(obj))
        myCar = (Cotxe*)obj;
    vObjectes.push_back(obj);
}

Capsa3D escena::CapsaMinCont3DEscena() {
    // Metode a implementar
    GLfloat a_min = std::numeric_limits<int>::max(),
            a_max = std::numeric_limits<int>::min(),
            h_min = std::numeric_limits<int>::max(),
            h_max = std::numeric_limits<int>::min(),
            p_min = std::numeric_limits<int>::max(),
            p_max = std::numeric_limits<int>::min();

    point4 *points = new point4[2];
    //vec3(a,h,p)
    int mes_u = (terra!=NULL) ? 1:0;
    for (int j = 0; j < vObjectes.size()+mes_u; j++) {
        Capsa3D c;
        if(j != vObjectes.size())
            c = vObjectes[j]->calculCapsa3D();
        else
            c = terra->calculCapsa3D();
        points[0] = c.pmin;
        points[1] = c.pmax;
        for (int i = 0; i < 2; i++) {
            if(points[i].x>a_max)
                a_max = points[i].x;
            if(points[i].x<a_min)
                a_min = points[i].x;
            if(points[i].y>h_max)
                h_max = points[i].y;
            if(points[i].y<h_min)
                h_min = points[i].y;
            if(points[i].z>p_max)
                p_max = points[i].z;
            if(points[i].z<p_min)
                p_min = points[i].z;
        }
    }
    delete points;

    Capsa3D capsa;
    capsa.pmin = vec3(a_min, h_min, p_min);
    capsa.pmax = vec3(a_max,h_max,p_max);
    capsa.a = a_max - a_min;
    capsa.h = h_max - h_min;
    capsa.p = p_max - p_min;
    capsa.pmig = vec3(a_min+capsa.a/2.0f,h_min+capsa.h/2.0f,p_min+capsa.p/2.0f);
    capsa.max_size = max(capsa.a, max(capsa.h, capsa.p));

    return capsa;
}

void escena::aplicaTG(mat4 m) {
    for(unsigned int i=0; i<vObjectes.size(); i++) {
        vObjectes[i]->aplicaTG(m);
    }
    if(terra!=NULL)
        terra->aplicaTG(m);
}

void escena::aplicaTGCentrat(mat4 m) {
    for(unsigned int i=0; i<vObjectes.size(); i++) {
        vObjectes[i]->aplicaTGCentrat(m);
    }
    if(terra!=NULL)
        terra->aplicaTGCentrat(m);
}

void escena::draw() {
    for(unsigned int i=0; i<vObjectes.size(); i++) {
        vObjectes[i]->program->bind();
        camera->toGPU(vObjectes[i]->program);
        llums->toGPU(vObjectes[i]->program);
        vObjectes[i]->draw();
    }
    if(terra!=NULL){
        terra->program->bind();
        camera->toGPU(terra->program);
        llums->toGPU(terra->program);
        terra->draw();
    }
}

void escena::reset() {
    resetCameraPanoramica();
    for(unsigned int i=0; i<vObjectes.size(); i++) {
        vObjectes[i]->make();
    }
    if(terra!=NULL)
        terra->make();
}

bool escena::isCollision(Objecte* obj){
    bool trobat;
    for(unsigned int i=0; i<vObjectes.size(); i++) {
        if(obj != vObjectes[i]) {
            trobat = vObjectes[i]->isCollision(obj);
            if (trobat) return true;
        }
    }
    return false;
}
