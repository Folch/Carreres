#include "obstacle.h"

Obstacle::Obstacle() : Objecte(NumVerticesF){
    vertexs.push_back(point4( -0.5, -0.5,  0.5, 1.0 ));
    vertexs.push_back(point4( -0.5,  0.5,  0.5, 1.0 ));
    vertexs.push_back(point4(  0.5,  0.5,  0.5, 1.0 ));
    vertexs.push_back(point4(  0.5, -0.5,  0.5, 1.0 ));
    vertexs.push_back(point4( -0.5, -0.5, -0.5, 1.0 ));
    vertexs.push_back(point4( -0.5,  0.5, -0.5, 1.0 ));
    vertexs.push_back(point4(  0.5,  0.5, -0.5, 1.0 ));
    vertexs.push_back(point4(  0.5, -0.5, -0.5, 1.0 ));

    mode = GL_TRIANGLES;
}

void Obstacle::make() {
    quad( 1, 0, 3, 2 );
    quad( 2, 3, 7, 6 );
    quad( 3, 0, 4, 7 );
    quad( 6, 5, 1, 2 );
    quad( 4, 5, 6, 7 );
    quad( 5, 4, 0, 1 );
    Objecte::make();
}


void Obstacle::quad(int a, int b, int c, int d) {
    cares.push_back(Cara(a,b,c));
    cares.push_back(Cara(a,c,d));
}
