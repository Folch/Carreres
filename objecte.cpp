#include "objecte.h"

/*
 * Mètode que es crida per crear un objecte amb les cares ja montades.
 *
 *
 */

Objecte::Objecte(vector<point4> vertexs, vector<Cara> cares, GLfloat mida, GLfloat xorig,GLfloat yorig,GLfloat zorig,double xrot, double yrot, double zrot) : numPoints(3*cares.size()){
    this->cares = cares;
    this->vertexs = vertexs;
    init(mida, xorig, yorig, zorig, xrot, yrot, zrot);

}

Objecte::Objecte(vector<point4> vertexs, vector<Cara> cares) : numPoints(3*cares.size()){
    this->cares = cares;
    this->vertexs = vertexs;
    init(1,0,0,0,0,0,0);

}

/*
 * Mètode que es crida, per crear objectes primaris com triangles o quadrats.
 *
 *
 */

Objecte::Objecte(int npoints) : numPoints(npoints) {
    init(1,0,0,0,0,0,0);

}

void Objecte::init(GLfloat mida, GLfloat xorig,GLfloat yorig,GLfloat zorig,double xrot, double yrot, double zrot) {

    points = new point4[numPoints];
    colors = new color4[numPoints];
    Index = 0;
    this->tam = mida;
    this->xorig = xorig;
    this->yorig = yorig;
    this->zorig = zorig;
    this->xRot = xrot;
    this->yRot = yrot;
    this->zRot = zrot;
}


Objecte::~Objecte()
{
    if(points!=NULL)
        delete points;
    if(colors!=NULL)
        delete colors;
}


Capsa3D Objecte::calculCapsa3D()
{

    // Metode a implementar: calcula la capsa mínima contenidora d'un objecte

    vec3    pmin, pmax;

    return capsa;
}




void Objecte::aplicaTG(mat4 m)
{
    aplicaTGPoints(m);

    // Actualitzacio del vertex array per a preparar per pintar
    glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(point4) * Index,
                     &points[0] );

}


void Objecte::aplicaTGPoints(mat4 m)
{
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

void Objecte::aplicaTGCentrat(mat4 m)
{

    // Metode a modificar
    aplicaTGPoints(m);
    aplicaTG(m);
}

void Objecte::toGPU(QGLShaderProgram *pr){

    program = pr;

    std::cout<<"Passo les dades de l'objecte a la GPU\n";

    glGenBuffers( 1, &buffer );
    glBindBuffer( GL_ARRAY_BUFFER, buffer );
    glBufferData( GL_ARRAY_BUFFER, sizeof(point4) * numPoints + sizeof(color4) * numPoints,
                  NULL, GL_STATIC_DRAW );
    program->link();

    program->bind();
    glEnable( GL_DEPTH_TEST );
}

// Pintat en la GPU.
void Objecte::draw()
{

    // cal activar el buffer de l'objecte. Potser que ja n'hi hagi un altre actiu
    glBindBuffer( GL_ARRAY_BUFFER, buffer );

    // per si han canviat les coordenades dels punts
    glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(point4) * Index, &points[0] );
    glBufferSubData( GL_ARRAY_BUFFER, sizeof(point4) * Index, sizeof(color4) * Index, &colors[0] );

    // Per a conservar el buffer
    int vertexLocation = program->attributeLocation("vPosition");
    int colorLocation = program->attributeLocation("vColor");

    program->enableAttributeArray(vertexLocation);
    program->setAttributeBuffer("vPosition", GL_FLOAT, 0, 4);

    program->enableAttributeArray(colorLocation);
    program->setAttributeBuffer("vColor", GL_FLOAT, sizeof(point4) * Index, 4);


    //glPolygonMode(GL_FRONT_AND_BACK, GL_FA);
    glDrawArrays( GL_TRIANGLES, 0, Index );

    // Abans nomes es feia: glDrawArrays( GL_TRIANGLES, 0, NumVerticesP );
}

void Objecte::make()
{

    static vec3  base_colors[] = {
        vec3( 1.0, 0.0, 0.0 ),
        vec3( 0.0, 1.0, 0.0 ),
        vec3( 0.0, 0.0, 1.0 ),
        vec3( 1.0, 1.0, 0.0 )
    };
    // Recorregut de totes les cares per a posar-les en les estructures de la GPU
    // Cal recorrer l'estructura de l'objecte per a pintar les seves cares

    Index = 0;

    for(unsigned int i=0; i<cares.size(); i++)
    {
        for(unsigned int j=0; j<cares[i].idxVertices.size(); j++)
        {
            points[Index] = vertexs[cares[i].idxVertices[j]];
            colors[Index] = base_colors[i%4];
            Index++;
        }
    }

    // S'ha de dimensionar uniformement l'objecte a la capsa de l'escena i s'ha posicionar en el lloc corresponent
}

float Objecte::getYOrig() {
    return this->yorig;
}



