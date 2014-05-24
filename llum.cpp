#include "llum.h"

Llum::Llum(QString name) {
    this->name = name;
    this->on = 1;
}

void Llum::toGPU(QGLShaderProgram* program) {

    gl_IdLlum.posicio =program->uniformLocation(name+".posicio");
    gl_IdLlum.dir =program->uniformLocation(name+".dir");
    gl_IdLlum.on =program->uniformLocation(name+".on");
    gl_IdLlum.angle = program->uniformLocation(name+".angle");

    gl_IdLlum.a =program->uniformLocation(name+".a");
    gl_IdLlum.b =program->uniformLocation(name+".b");
    gl_IdLlum.c =program->uniformLocation(name+".c");

    gl_IdLlum.ambient =program->uniformLocation(name+".ambient");
    gl_IdLlum.diffuse =program->uniformLocation(name+".diffuse");
    gl_IdLlum.specular =program->uniformLocation(name+".specular");


    glUniform4fv(gl_IdLlum.posicio,1,position);//posicioLlum és un vec4
    glUniform4fv(gl_IdLlum.dir,1,dir);
    glUniform1i(gl_IdLlum.on, on);
    glUniform1f(gl_IdLlum.angle,angle);// coef_a és un Glfloat

    glUniform1f(gl_IdLlum.a,a);// coef_a és un Glfloat
    glUniform1f(gl_IdLlum.b,b);
    glUniform1f(gl_IdLlum.c,c);

    glUniform3fv(gl_IdLlum.ambient,1,ambient);
    glUniform3fv(gl_IdLlum.diffuse,1,diffuse);// difusa és un vec3
    glUniform3fv(gl_IdLlum.specular,1,specular);
}
