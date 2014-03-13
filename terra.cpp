#include "terra.h"


Terra::Terra() : Objecte(NumVerticesF) {
    init(1, 0, 1);
}

Terra::Terra(float x, float z, float y):Objecte(NumVerticesF) {
    init(x, y, z);
}

void Terra::init(float x, float y, float z) {
    Index = 0;

    xorig = 0;
    yorig = y;
    zorig = 0;

    xRot = 0;
    yRot = 0;
    zRot = 0;


    vertexs.push_back(point4(xorig-(x/2.0f), y, zorig-(z/2.0f), 1.0f));
    vertexs.push_back(point4(xorig-(x/2.0f), y, zorig+(z/2.0f), 1.0f));
    vertexs.push_back(point4(xorig+(x/2.0f), y, zorig+(z/2.0f), 1.0f));
    vertexs.push_back(point4(xorig+(x/2.0f), y, zorig-(z/2.0f), 1.0f));

    colors[0] = color4(0,0,1,1); //Blau
    colors[1] = color4(1,0,0,1); //Vermell
    colors[2] = color4(1,1,0,0); //Groc
    colors[3] = color4(0,0,1,1); //Blau
    colors[4] = color4(1,1,0,0); //Groc
    colors[5] = color4(0,1,0,1); //Verd



    quad(0,1,2,3);
}

void Terra::quad(int a, int b, int c, int d) {
    points[Index] = vertexs[a]; Index++;
    points[Index] = vertexs[b]; Index++;
    points[Index] = vertexs[c]; Index++;
    points[Index] = vertexs[a]; Index++;
    points[Index] = vertexs[c]; Index++;
    points[Index] = vertexs[d]; Index++;

}
