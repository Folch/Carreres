#include <escena.h>

using namespace std;

escena::escena()

{
    // Capsa minima contenidora provisional: S'ha de fer un recorregut dels objectes de l'escenes
    capsaMinima.pmin[0] = 0; capsaMinima.pmin[1] = 0; capsaMinima.pmin[2]=0;
    capsaMinima.a = 1; capsaMinima.h = 1; capsaMinima.p = 1;

}


escena::~escena()
{
    printf("DESTRUCTOR!");

}

void escena::addObjecte(Objecte *obj) {
    //if (dynamic_cast<Cotxe*>(obj)) Així es fa un cast
    vObjectes.push_back(obj);
}


void escena::CapsaMinCont3DEscena()
{
    // Metode a implementar
}

void escena::aplicaTG(mat4 m) {
    for (unsigned int i = 0; i < vObjectes.size(); ++i) {
        vObjectes[i]->aplicaTG(m);
    }
}

void escena::aplicaTGCentrat(mat4 m) {
    for (unsigned int i = 0; i < vObjectes.size(); ++i) {
        vObjectes[i]->aplicaTGCentrat(m);
    }
}

void escena::draw() {
    for (unsigned int i = 0; i < vObjectes.size(); ++i) {
        vObjectes[i]->draw();
    }
}

void escena::reset() {
    for (unsigned int i = 0; i < vObjectes.size(); ++i) {
        vObjectes[i]->make();
    }
}

