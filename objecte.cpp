#include "objecte.h"
#include "readfile.h"

Objecte::Objecte(int npoints, QObject *parent) : numPoints(npoints), QObject(parent) {
    points = new point4[npoints];
    colors = new color4[npoints];
}

Objecte::Objecte(int npoints, QString n, GLdouble tamanio, GLdouble x0, GLdouble y0, GLdouble z0, double girx, double giry, double girz) : numPoints(npoints)
{
    points = new point4[npoints];
    colors = new color4[npoints];
    tam = tamanio;
    std::cout<<"Estic en el constructor parametritzat del cotxe\n";
    xorig = x0;
    yorig = y0;
    zorig = z0;

    xRot = girx;
    yRot = giry;
    zRot = girz;


    nom = n;
    Index = 0;

    readObj(n);

    make();

}


Objecte::~Objecte() {
    delete points;
    delete colors;
}


void Objecte::aplicaTG(mat4 m) {
    aplicaTGPoints(m);

    // Actualitzacio del vertex array per a preparar per pintar
    glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(point4) * Index,
                     &points[0] );

}


void Objecte::aplicaTGPoints(mat4 m) {
    point4  *transformed_points = new point4[Index];

    for ( int i = 0; i < Index; ++i ) {
        transformed_points[i] = m * points[i];
    }

    transformed_points = &transformed_points[0];
    points = &points[0];

    for ( int i = 0; i < Index; ++i )
    {
        points[i] = transformed_points[i];
    }

    delete transformed_points;
}

void Objecte::aplicaTGCentrat(mat4 m) {

    // Metode a modificar
    aplicaTGPoints(m); //WTF??
    aplicaTG(m);
}

void Objecte::toGPU(QGLShaderProgram *pr){

    program = pr;

    std::cout<<"Passo les dades de l'objecte a la GPU\n";

    glGenBuffers( 1, &buffer );
    glBindBuffer( GL_ARRAY_BUFFER, buffer );
    glBufferData( GL_ARRAY_BUFFER, sizeof(point4) * Index + sizeof(color4) * Index,
                  NULL, GL_STATIC_DRAW );
    program->link();

    program->bind();
    glEnable( GL_DEPTH_TEST );
}





float Objecte::getYOrig() {
    return this->yorig;
}


void Objecte::construeix_cara ( char **words, int nwords) {
    Cara face;
    for (int i = 0; i < nwords; i++) {
        int vindex;
        int nindex;
        int tindex;

        if ((words[i][0]>='0')&&(words[i][0]<='9')) {
            get_indices (words[i], &vindex, &tindex, &nindex);

#if 0
            printf ("vtn: %d %d %d\n", vindex, tindex, nindex);
#endif

            /* store the vertex index */

            if (vindex > 0)       /* indices are from one, not zero */
                face.idxVertices.push_back(vindex - 1);
            else if (vindex < 0)  /* negative indices mean count backwards */
                face.idxVertices.push_back(vertexs.size() + vindex);
            else {
                fprintf (stderr, "Zero indices not allowed: '%s'\n", str_orig);
                exit (-1);
            }
        }
    }
    face.color = vec4(1.0, 0.0, 0.0, 1.0);
    cares.push_back(face);
}


