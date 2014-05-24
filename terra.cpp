#include "terra.h"


Terra::Terra() : Objecte(NumVerticesF) {
    init(1, 0, 1);
}

Terra::Terra(float x, float z, float y):Objecte(NumVerticesF) {
    init(x, 0, z);
}

void Terra::init(float x, float y, float z) {
    vertexs.push_back(point4(xorig-(x/2.0f), y, zorig-(z/2.0f), 1.0f)); //0
    vertexs.push_back(point4(xorig-(x/2.0f), y, zorig+(z/2.0f), 1.0f)); //1
    vertexs.push_back(point4(xorig+(x/2.0f), y, zorig+(z/2.0f), 1.0f)); //2
    vertexs.push_back(point4(xorig+(x/2.0f), y, zorig-(z/2.0f), 1.0f)); //3

    /*vertexs.push_back(point4(xorig-(x/2.0f), y, zorig-(z/2.0f), 1.0f)); //3
    vertexs.push_back(point4(xorig+(x/2.0f), y, zorig+(z/2.0f), 1.0f)); //4
    */



    this->mode = GL_TRIANGLES;

    m = new Material();

    m->ka = vec3(0.0f,0.05f,0.0f);
    m->kd = vec3(0.4f,0.5f,0.4f);
    m->ks = vec3(0.04f,0.7f,0.04f);

    m->shineness = 0.078125f*128;
}

void Terra::make() {
    cares.clear();
    cares.push_back(new Cara(0,1,2));
    cares.push_back(new Cara(0,2,3));

    Objecte::make();
}
