#include "objecte.h"

/*
 * Mètode que es crida per crear un objecte amb les cares ja montades.
 *
 *
 */

Objecte::Objecte(vector<point4> vertexs, vector<Cara*> cares, GLfloat mida, GLfloat xorig,GLfloat yorig,GLfloat zorig,double xrot, double yrot, double zrot) : numPoints(3*cares.size()){
    this->cares = cares;
    this->vertexs = vertexs;
    init(mida, xorig, yorig, zorig, xrot, yrot, zrot);

}

Objecte::Objecte(vector<point4> vertexs, vector<Cara*> cares) : numPoints(3*cares.size()){
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

Objecte::Objecte(int npoints, GLfloat mida, GLfloat xorig,GLfloat yorig,GLfloat zorig,double xrot, double yrot, double zrot) : numPoints(npoints) {
    init(mida, xorig, yorig, zorig, xrot, yrot, zrot);

}

void Objecte::init(GLfloat mida, GLfloat xorig,GLfloat yorig,GLfloat zorig,double xrot, double yrot, double zrot) {

    points = new point4[numPoints];
    pointsTmp = new point4[numPoints];
    normals = new point4[numPoints];
    Index = 0;
    this->tam = mida;
    this->xorig = xorig;
    this->yorig = yorig;
    this->zorig = zorig;
    this->xRot = xrot;
    this->yRot = yrot;
    this->zRot = zrot;

    this->mode = GL_LINES;

    this->capsa.a = 0;
    this->capsa.h = 0;
    this->capsa.p = 0;

    tipusNormal = GOURAUD;
}


Objecte::~Objecte()
{
    if(points!=NULL)
        delete points;
    if(normals!=NULL)
        delete normals;
}


Capsa3D Objecte::calculCapsa3D() {
    // Metode a implementar: calcula la capsa mínima contenidora d'un objecte

    GLfloat a_min = std::numeric_limits<int>::max(),
            a_max = std::numeric_limits<int>::min(),
            h_min = std::numeric_limits<int>::max(),
            h_max = std::numeric_limits<int>::min(),
            p_min = std::numeric_limits<int>::max(),
            p_max = std::numeric_limits<int>::min();

    //vec3(a,h,p)
    for (int i = 0; i < Index; i++) {
        if(points[i].x>a_max)
            a_max = points[i].x;
        if(points[i].x<a_min)
            a_min = points[i].x;
        if(points[i].y>h_max)
            h_max = points[i].y;
        if(points[i].y<h_min)
            h_min = points[i].y;
        if(points[i].z>p_max)
            p_max = points[i].z;
        if(points[i].z<p_min)
            p_min = points[i].z;
    }
    Capsa3D capsa;
    capsa.pmin = vec3(a_min, h_min, p_min);
    capsa.pmax = vec3(a_max,h_max,p_max);
    capsa.a = a_max - a_min;
    capsa.h = h_max - h_min;
    capsa.p = p_max - p_min;
    capsa.pmig = vec3(a_min+capsa.a/2.0f,h_min+capsa.h/2.0f,p_min+capsa.p/2.0f);
    capsa.max_size = max(capsa.a, max(capsa.h, capsa.p));


    this->capsa = capsa;
    return capsa;

}




void Objecte::aplicaTG(mat4 m)
{
    aplicaTGPoints(m);

    // Actualitzacio del vertex array per a preparar per pintar
    glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(point4) * Index,
                     &points[0] );

}


void Objecte::aplicaTGPoints(mat4 m) {

    aplicaTGNormals(m);

    point4 *transformed_points = new point4[Index];

    for ( int i = 0; i < Index; i++) {
        transformed_points[i] = m * points[i];
    }

    transformed_points = &transformed_points[0];
    points = &points[0];

    for ( int i = 0; i < Index; i++ )
    {
        points[i] = transformed_points[i];
    }

    delete transformed_points;
}


void Objecte::aplicaTGNormals(mat4 m) {
    for (int i = 0; i < numPoints; ++i) {
        normals[i] = normalize(m * normals[i]);
    }
}

void Objecte::backupPoints() {
    for ( int i = 0; i < Index; i++ )
        pointsTmp[i] = points[i];
}

void Objecte::restorePoints() {
    for ( int i = 0; i < Index; i++ )
        points[i] = pointsTmp[i];
}

void Objecte::aplicaTGCentrat(mat4 m) {
    Capsa3D c = calculCapsa3D();
    aplicaTG(Translate(c.pmig.x, c.pmig.y, c.pmig.z)*m*Translate(-c.pmig.x, -c.pmig.y, -c.pmig.z));
}

void Objecte::toGPU(QGLShaderProgram *pr) {

    program = pr;

    std::cout<<"Passo les dades de l'objecte a la GPU\n";

    glGenBuffers( 1, &buffer );
    glBindBuffer( GL_ARRAY_BUFFER, buffer );
    glBufferData( GL_ARRAY_BUFFER, sizeof(point4) * Index + sizeof(point4) * Index,
                  NULL, GL_STATIC_DRAW );
    program->link();

    //program->bind();
    glEnable( GL_DEPTH_TEST );
}

// Pintat en la GPU.
void Objecte::draw() {

    m->toGPU(program);

    // cal activar el buffer de l'objecte. Potser que ja n'hi hagi un altre actiu
    glBindBuffer( GL_ARRAY_BUFFER, buffer );

    // per si han canviat les coordenades dels punts
    glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(point4) * Index, &points[0] );
    glBufferSubData( GL_ARRAY_BUFFER, sizeof(point4) * Index, sizeof(point4) * Index, &normals[0] );



    // Per a conservar el buffer
    int vertexLocation = program->attributeLocation("vPosition");
    int normalLocation = program->attributeLocation("vNormal");

    program->enableAttributeArray(vertexLocation);
    program->setAttributeBuffer("vPosition", GL_FLOAT, 0, 4);

    program->enableAttributeArray(normalLocation);
    program->setAttributeBuffer("vNormal", GL_FLOAT, sizeof(point4) * Index, 4);

    //glPolygonMode(GL_FRONT_AND_BACK, GL_FA);
    glDrawArrays(mode, 0, Index );

    // Abans nomes es feia: glDrawArrays( GL_TRIANGLES, 0, NumVerticesP );
}

void Objecte::make() {
    bindCares();
    calculaNormals();
    Index = 0;
    vec4 vector;
    int idx;
    for(unsigned int i=0; i<cares.size(); i++) {
        for(unsigned int j=0; j<cares[i]->idxVertices.size(); j++) {
            idx = cares[i]->idxVertices[j];
            points[Index] = vertexs[idx];
            pointsTmp[Index] = vertexs[idx];
            if(tipusNormal == GOURAUD)
                vector = getNormalGourond(caresHash[idx]);
            else if(tipusNormal == FLAT)
                vector = getNormalFlatShading(caresHash[idx]);
            normals[Index].x = vector.x;
            normals[Index].y = vector.y;
            normals[Index].z = vector.z;
            Index++;
        }
    }
    //normalsFlatShading();
}

void Objecte::calculaNormals() {
    for (int i = 0; i < cares.size(); i++) {
        cares[i]->calculaNormal(vertexs);
    }
}

void Objecte::bindCares() {
    for (int i = 0; i < vertexs.size(); i++) {
        for (int j = 0; j < cares.size(); j++) {
            if(cares[j]->vertex(i))
                caresHash[i].push_back(cares[j]);
        }
    }
}

/*Aquí tinc una llista de cares, i he de retornar la normal d'una d'elles*/
vec4 Objecte::getNormalFlatShading(vector<Cara*> cares) {
        return cares[0]->normal;
}

/*Aquí tinc una llista de cares, i he de retornar la normal d'una d'elles*/
vec4 Objecte::getNormalGourond(vector<Cara*> cares) {
    vec4 vector(0,0,0);
    vec4 tmp;
    for (int i = 0; i < cares.size(); ++i) {
        //vector += cares[i].calculaNormal(v);
        tmp = cares[i]->normal;
        vector.x += tmp.x;
        vector.y += tmp.y;
        vector.z += tmp.z;
    }
    vector /= cares.size();
    return vector;
}

float Objecte::getYOrig() {
    return this->yorig;
}

bool Objecte::isCollision(Objecte * obj){

    Capsa3D c1 = obj->calculCapsa3D();
    Capsa3D c2 = this->calculCapsa3D();
    return (c1.pmax.x > c2.pmin.x &&
            c1.pmin.x < c2.pmax.x &&
            c1.pmax.y > c2.pmin.y &&
            c1.pmin.y < c2.pmax.y &&
            c1.pmax.z > c2.pmin.z &&
            c1.pmin.z < c2.pmax.z );
}



