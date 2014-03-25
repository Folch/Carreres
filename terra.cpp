#include "terra.h"


Terra::Terra() : Objecte(NumVerticesF) {
    init(1, 0, 1);
}

Terra::Terra(float x, float z, float y):Objecte(NumVerticesF) {
    init(x, y, z);
}

void Terra::init(float x, float y, float z) {
    vertexs.push_back(point4(xorig-(x/2.0f), y, zorig-(z/2.0f), 1.0f));
    vertexs.push_back(point4(xorig-(x/2.0f), y, zorig+(z/2.0f), 1.0f));
    vertexs.push_back(point4(xorig+(x/2.0f), y, zorig+(z/2.0f), 1.0f));
    vertexs.push_back(point4(xorig+(x/2.0f), y, zorig-(z/2.0f), 1.0f));

    this->mode = GL_TRIANGLES;
}

void Terra::make() {
    cares.clear();
    cares.push_back(Cara(0,1,2));
    cares.push_back(Cara(0,2,3));

    Objecte::make();
}
