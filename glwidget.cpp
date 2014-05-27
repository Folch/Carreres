#include <QtGui>
#include <QtOpenGL>

#include <math.h>

#include "glwidget.h"
//#include <QGlobal.h>
#include <QTime>
#include "readobject.h"

GLWidget::GLWidget(QWidget *parent)
    : QGLWidget(QGLFormat(QGL::SampleBuffers), parent) {
    setFocusPolicy( Qt::StrongFocus );

    a = 50.0;
    h = 50.0;
    p = 50.0;

    qtGreen = QColor::fromCmykF(0.40, 0.0, 1.0, 0.0);
    qtPurple = QColor::fromCmykF(0.39, 0.39, 0.0, 0.0);
}


GLWidget::~GLWidget() {
    makeCurrent();
}

//  Metode per a carregar de fitxers el vertex i el fragment shader
void GLWidget::InitShader(const char* vShaderFile, const char* fShaderFile, QGLShaderProgram* program) {

    struct Shader {
        const char*  filename;
        GLenum       type;
        GLchar*      source;
    }  shaders[2] = {
    { vShaderFile, GL_VERTEX_SHADER, NULL },
    { fShaderFile, GL_FRAGMENT_SHADER, NULL }
};


    QGLShader *vshader = new QGLShader(QGLShader::Vertex, this);
    QGLShader *fshader = new QGLShader(QGLShader::Fragment, this);
    // Es llegeixen els dos shaders: el vertex i el fragment shader
    for ( int i = 0; i < 2; ++i ) {
        Shader& s = shaders[i];
        s.source = Common::readShaderSource( s.filename );
        if ( shaders[i].source == NULL ) {
            std::cerr << "Failed to read " << s.filename << std::endl;
            exit( EXIT_FAILURE );
        }
    }
    vshader->compileSourceCode(shaders[0].source);
    fshader->compileSourceCode(shaders[1].source);

    program->addShader(vshader);
    program->addShader(fshader);

    program->bindAttributeLocation("vPosition", PROGRAM_VERTEX_ATTRIBUTE);
    program->bindAttributeLocation("vNormal", PROGRAM_NORMAL_ATTRIBUTE);
    program->link();



    //program->bind();
}

// Metode per inicialitzar els shaders de l'aplicacio
void GLWidget::initShadersGPU() {

    //programs:
        // 0: programCotxe
        // 1: program

    //Carrega dels shaders i posa a punt per utilitzar els programes carregats a la GPU
    QGLShaderProgram* programCotxe = new QGLShaderProgram(this);
    InitShader( "../Carreres/vshadercotxe.glsl", "../Carreres/fshadercotxe.glsl", programCotxe);
    programs.push_back(programCotxe);


    QGLShaderProgram* program = new QGLShaderProgram(this);
    //InitShader( "../Carreres/vshadercotxe.glsl", "../Carreres/fshadercotxe.glsl", program);
    InitShader( "../Carreres/vshadergeneral.glsl", "../Carreres/fshadergeneral.glsl", program);
    programs.push_back(program);

    esc = new escena(new Camera("Paralela", this->size().width(), this->size().height()));

    esc->llums = new ConjuntLlums();
    esc->llums->Ia = vec3(0.2f);
    esc->llums->ka = vec3(0.2f);

    Llum *l = new Llum("llum_puntual");
    l->position = vec4(0,0,-50,0);
    l->diffuse = vec3(0.9f);
    l->ambient = vec3(0.0f);
    l->specular = vec3(0.9f);
    l->a = 0.0f;
    l->b = 0.0f;
    l->c = 1.0f;
    esc->llums->addLlum(l);

    Llum *l2 = new Llum("llum_direccional");
    l2->dir = vec4(0,0,50,0);
    l2->diffuse = vec3(0.9f);
    l2->ambient = vec3(0.0f);
    l2->specular = vec3(0.9f);
    l2->a = 0.0f;
    l2->b = 0.0f;
    l2->c = 1.0f;
    esc->llums->addLlum(l2);

    Llum *l3 = new Llum("llum_spot");
    l3->position = vec4(0,25,0,0);
    l3->dir = vec4(0,1,0,0);
    l3->angle = 0.57f;
    l3->diffuse = vec3(1.0f);
    l3->ambient = vec3(0.0f);
    l3->specular = vec3(1.0f);
    l3->a = 0.0f;
    l3->b = 0.0f;
    l3->c = 1.0f;
    esc->llums->addLlum(l3);


}


QSize GLWidget::minimumSizeHint() const {
    return QSize(50, 50);
}

QSize GLWidget::sizeHint() const {
    return QSize(400, -90400);
}

// Metodes per a poder rotar l'escena

static void qNormalizeAngle(int &angle) {
    while (angle < 0)
        angle += 360 * 16;
    while (angle > 360 * 16)
        angle -= 360 * 16;
}

void GLWidget::newObjecte(Objecte * obj) {

    if(dynamic_cast<Terra*>(obj)) {
        esc->addLand((Terra*)obj);
    }else{
        esc->addObjecte(obj);
    }
    obj->backupPoints();

    esc->resetCameraPanoramica();
    if(esc->camera->name == "Tercera")
        esc->actualitzaCameraThirdPerson();

    updateGL();
}

void GLWidget::newObstacle(int nombre) {
    // Metode que serveix per a donar d'alta un obstacle amb el punt original a xorig, yorig,zorig
    // d'una certa mida
    // ha d'estar a les ys del pla de la terra

    // Metode a implementar

    if(esc->terra == NULL || esc->myCar == NULL) {
        //mostrar un dialog d'error
       //QMessageBox::information(0, "Avis", "Has de crear abans el terra i el cotxe");
       //return;
    }

    Capsa3D capsa = esc->CapsaMinCont3DEscena();
    GLfloat w = capsa.a;
    GLfloat p = capsa.p;
    GLfloat t;
    if(w<p) t = w;
    else t = p;
    Obstacle *o;
    srand(time(NULL));
    int i = 0;
    while(i<nombre) {
        double x = rand()/(float)RAND_MAX * w - w/2;
        double z = rand()/(float)RAND_MAX * p - w/2;
        o = new Obstacle(x,0,z,t*0.1f);
        o->make();
        if(esc->isCollision(o)) {
            i--;
            delete o;
        }else {
            delete o;
            o = new Obstacle(x,0,z,t*0.1f);
            o->make();
            o->toGPU(programs[1]);
            newObjecte(o);
        }
        i++;
    }


}

void GLWidget::newTerra(float amplaria, float profunditat, float y) {
    // Metode que crea un objecte terra poligon amb el punt original a xorig, yorig, zorig
    // (quadrat d'una certa mida amb origen a xorig, yorig, zorig

    // Metode a implementar
    Terra *t;


    esc->llums->llums[0]->position = vec4(0,0,-max(amplaria, profunditat),1);
    esc->llums->llums[2]->position = vec4(0,max(amplaria/2.0f, profunditat/2.0f), 0,1);
    t = new Terra(amplaria, profunditat, y);
    t->make();
    t->toGPU(programs[1]);
    newObjecte(t);



 }

void GLWidget::newCotxe(QString fichero, float xorig, float zorig, float mida, float xdirec, float ydirec, float zdirec) {
    Cotxe *obj;
    // parametres que posen l'objecte cotxe al punt original xorig, yorig, zorig i d'una certa mida
    // Cal modificar-lo per a que es posicioni a la Y correcte
    float yorig = 0;

    if(mida<20) mida = 20;

    ReadObject *r = new ReadObject();
    //r.readObj(obj, &cares, &vertexs);
    obj = r->readCar(fichero, mida, xorig, yorig, zorig, 0.,0.,0., xdirec, ydirec, zdirec );


    //obj = new Cotxe(vertexs, cares,mida, xorig, yorig, zorig, 0., 0., 0.,xdirec, ydirec, zdirec);
    obj->make();
    obj->toGPU(programs[0]);
    newObjecte(obj);
    //esc->actualitzaCameraThirdPerson();
    updateGL();

    delete r;
}

void GLWidget::initializeGL() {
    glEnable(GL_DEPTH_TEST);
    //glEnable(GL_CULL_FACE);

    std::cout<<"Estic inicialitzant el shaders"<<std::ends;
    initShadersGPU();

    // A canviar per posar una imatge de fons: Estrelles...
    qglClearColor(qtPurple.dark());
}

void GLWidget::resetView() {
    esc->applyCamera("Paralela");

    esc->reset();

    // Metode a modificar per a adaptar tots els objectes de l'escena.
    for (int i = 0; i < esc->vObjectes.size(); ++i) {
        esc->vObjectes[i]->backupPoints();
    }
    if(esc->terra != NULL) {
        esc->terra->backupPoints();
    }


    updateGL();
}

void GLWidget::paintGL() {
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    esc->draw();
}

void GLWidget::resizeGL(int width, int height) {
    int side = qMin(width, height);
    glViewport((width - side) / 2, (height - side) / 2, side, side);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
#ifdef QT_OPENGL_ES_1
    glOrthof(-150, +150, -150, +150, 0.0, 150.0);
#else
    glOrtho(-150, +150, -150, +150, 0.0, 150.0);
#endif
    glMatrixMode(GL_MODELVIEW);
}


void GLWidget::Zoom(int zoom) {
    /*Si la cámara es la de tercera no permet fer zoom*/
    if(esc->camera->name == "Tercera") return;
    if(zoom == 1)
        esc->camera->AmpliaWindow(0.05);
    else
        esc->camera->AmpliaWindow(-0.05);

    esc->actualitzaCameraPanoramica(false);

    updateGL();
}

void GLWidget::Pan(int dx, int dy) {
    /*Si la cámara es la de tercera no permet fer panning*/
    if(esc->camera->name == "Tercera") return;
    Capsa3D c = esc->CapsaMinCont3DEscena();
    GLfloat factor = c.max_size*0.005f;
    esc->camera->wd.pmin[0] -= factor*dx;
    esc->camera->wd.pmin[1] += factor*dy;
    esc->actualitzaCameraPanoramica(false);

    updateGL();
}


void GLWidget::mousePressEvent(QMouseEvent *event) {
    lastPos = event->pos();
}

void GLWidget::mouseMoveEvent(QMouseEvent *event) {
    int dx = event->x() - lastPos.x();
    int dy = event->y() - lastPos.y();

    if (event->buttons() & Qt::LeftButton) {
        /*Si la cámara es la de tercera no permet rotarla*/
        if(esc->camera->name == "Tercera") return;
        if(lastPos.y() != event->y() && lastPos.x() != event->x()) {
            esc->camera->setRotation(dy, dx);
        }else if(lastPos.y() != event->y()) {
            esc->camera->setRotation(dy, 0);
        } else if(lastPos.x() != event->x()) {
           esc->camera->setRotation(0, dx);
        }
        updateGL();
    } else if (event->buttons() & Qt::RightButton) {
        Pan(dx,dy);
    } else if(event->buttons() & Qt::MidButton) {
        if(lastPos.y() > event->y())
            Zoom(-1);
        else
            Zoom(1);
    }
    lastPos = event->pos();
}

void GLWidget::keyPressEvent(QKeyEvent *event) {
    // Metode a implementar

    switch ( event->key() ){
        case Qt::Key_Up:
            if(!esc->isCollision(esc->myCar)){
                esc->myCar->backupPoints();
                esc->myCar->forward();
            }
            else
                esc->myCar->restorePoints();
            //esc->camera->setVRP(esc->myCar->calculCapsa3D());
            //esc->camera->update();
            esc->actualitzaCameraThirdPerson();
            break;
        case Qt::Key_Down:
            if(!esc->isCollision(esc->myCar)){
                esc->myCar->backupPoints();
                esc->myCar->backward();
            }else
                esc->myCar->restorePoints();
            //esc->camera->setVRP(esc->myCar->calculCapsa3D());
            //esc->camera->update();
            esc->actualitzaCameraThirdPerson();
            break;
        case Qt::Key_Left:
            esc->myCar->turnleft();
            break;
        case Qt::Key_Right:
            esc->myCar->turnright();
            break;
        case Qt::Key_Escape:
            if(esc->myCar == NULL) return;
            if (!esc->cameras.contains("Tercera")) {
                Camera *c = new Camera("Tercera", this->size().width(), this->height());
                esc->addCamera(c);
                esc->iniLookAtCotxe();
            }

            QString name;
            if(esc->camera->name == "Tercera")
                name = "Paralela";
            else
                name = "Tercera";
            esc->applyCamera(name);
            if(name == "Tercera")
                esc->actualitzaCameraThirdPerson();
            break;
    }

    if (event->key()==Qt::Key_1){
        esc->llums->llums[0]->on = esc->llums->llums[0]->on == 0 ? 1:0;
    } else if(event->key()==Qt::Key_2) {
        esc->llums->llums[1]->on = esc->llums->llums[1]->on == 0 ? 1:0;
    } else if(event->key()==Qt::Key_3) {
        esc->llums->llums[2]->on = esc->llums->llums[2]->on == 0 ? 1:0;
    }
    updateGL();
}
void GLWidget::keyReleaseEvent(QKeyEvent *event)
{
    // Metode a implementar en el cas que es doni velocitat al cotxe

}
