#ifndef CONJUNTLLUMS_H
#define CONJUNTLLUMS_H

#include <vector>
#include <Common.h>
#include "llum.h"

using namespace std;

class ConjuntLlums {
public:
    ConjuntLlums();

    vector<Llum*> llums;

    vec3 ka;
    vec3 Ia;

    void addLlum(Llum*);
    void setAmbientToGPU(QGLShaderProgram*);
    void toGPU(QGLShaderProgram*);

};

#endif // CONJUNTLLUMS_H
