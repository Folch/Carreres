#ifndef MATERIAL_H
#define MATERIAL_H

#include <Common.h>
#include <QGLShaderProgram>

typedef Common::vec3  point3;

using namespace std;

struct gl_IdMaterial{
    GLuint ka;
    GLuint kd;
    GLuint ks;
    GLuint shineness;
};

class Material {
    public:
        Material();

        vec3 ka;
        vec3 kd;
        vec3 ks;

        float shineness;

        void toGPU(QGLShaderProgram*);
};

#endif // MATERIAL_H
