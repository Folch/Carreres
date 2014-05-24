#include "material.h"

Material::Material()
{
}

void Material::toGPU(QGLShaderProgram *program) {
    gl_IdMaterial m;

    m.ka =program->uniformLocation("material.ambient");
    m.ks =program->uniformLocation("material.specular");
    m.kd =program->uniformLocation("material.diffuse");

    m.shineness = program->uniformLocation("material.shineness");

    glUniform3fv(m.ka,1,this->ka);
    glUniform3fv(m.ks,1,this->ks);
    glUniform3fv(m.kd,1,this->kd);

    glUniform1f(m.shineness,this->shineness);
}
