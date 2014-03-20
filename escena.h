#ifndef ESCENA_H
#define ESCENA_H

#include <cmath>
#include <iostream>
#include <stdlib.h>

#include <list>
#include <vector>
#include <string>
#include <stdio.h>


#include <Common.h>


#include <objecte.h>
#include <cotxe.h>
#include <terra.h>
#include <obstacle.h>

using namespace std;

class escena
{
public:
    escena();
    ~escena();

    void addObjecte(Objecte *obj);
    void addLand(Terra *t);
    void aplicaTG(mat4 m);

    void aplicaTGCentrat(mat4 m);
    void reset();

    void draw();
    void CapsaMinCont3DEscena();

    // Capsa contenedora de l'escena
    Capsa3D capsaMinima;

    // Objectes de l'escena: a modificar. Ara nomes té un objecte. Cal afegir un conjunt d'objectes
    vector<Objecte*> vObjectes;
    Cotxe *myCar;
    Terra *terra;

};


#endif // ESCENA_H
