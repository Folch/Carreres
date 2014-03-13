#include <escena.h>

using namespace std;

escena::escena() {
    // Capsa minima contenidora provisional: S'ha de fer un recorregut dels objectes de l'escenes
    capsaMinima.pmin[0] = 0; capsaMinima.pmin[1] = 0; capsaMinima.pmin[2]=0;
    capsaMinima.a = 1; capsaMinima.h = 1; capsaMinima.p = 1;

    terra = NULL;
}


escena::~escena() {
    printf("DESTRUCTOR!");
}

void escena::addLand(Terra *t) {
    if(this->terra!=NULL)
        delete this->terra;
    this->terra = t;
}

void escena::addObjecte(Objecte *obj) {
    vObjectes.push_back(obj);
}

void escena::CapsaMinCont3DEscena() {
    // Metode a implementar
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
        vObjectes[i]->draw();
    }
    if(terra!=NULL)
        terra->draw();
}

void escena::reset() {
    for(unsigned int i=0; i<vObjectes.size(); i++) {
        vObjectes[i]->make();
    }
    if(terra!=NULL)
        terra->make();
}

