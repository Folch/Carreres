#include "carrosseria.h"
#include "cara.h"
Carrosseria::Carrosseria(vector<point4> vertexs, vector<Cara*> cares) : Objecte(vertexs, cares) {
    m = new Material();

    m->ka = vec3(0.0f);
    m->ks = vec3(0.7f,0.6f,0.6f);
    m->kd = vec3(0.5f,0.0f,0.0f);

    m->shineness = 0.25f*128;
}
