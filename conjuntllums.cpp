#include "conjuntllums.h"

#include "llum.h"
#include <vector>

using namespace std;


ConjuntLlums::ConjuntLlums() {

}

void ConjuntLlums::addLlum(Llum *llum) {
    llums.push_back(llum);
}

void ConjuntLlums::toGPU(QGLShaderProgram* program) {
    GLuint k_a = program->uniformLocation("ka_global");
    GLuint I_a = program->uniformLocation("Ia_global");
    glUniform3fv(k_a, 1, ka);
    glUniform3fv(I_a, 1, Ia);

    for (unsigned int i = 0; i < llums.size(); i++) {
        llums[i]->toGPU(program);
    }
}
