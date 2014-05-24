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
#include <camera.h>
#include "conjuntllums.h"

using namespace std;

class escena
{
public:
    escena(Camera*);
    ~escena();

    void addObjecte(Objecte *obj);
    void addLand(Terra *t);
    void aplicaTG(mat4 m);

    /*For Camera*/
    void addCamera(Camera*);
    void applyCamera(QString);
    void actualitzaCameraPanoramica(bool);
    void resetCameraPanoramica();
    void actualitzaCameraThirdPerson();
    void iniLookAtCotxe();

    void aplicaTGCentrat(mat4 m);
    void reset();

    void draw();
    Capsa3D CapsaMinCont3DEscena();
    bool isCollision(Objecte*);

    //Camares de la escena
    Camera *camera; //Camera actual
    QHash<QString, Camera*> cameras; //Llista de cameras
    ConjuntLlums *llums;

    // Objectes de l'escena: a modificar. Ara nomes té un objecte. Cal afegir un conjunt d'objectes
    vector<Objecte*> vObjectes;
    Cotxe *myCar;
    Terra *terra;

};


#endif // ESCENA_H
