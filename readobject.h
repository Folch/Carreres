#ifndef READOBJECT_H
#define READOBJECT_H

#include "cotxe.h"
#include "objecte.h"
#include "obstacle.h"
#include "roda.h"
#include "cara.h"

class ReadObject {
    public:
        ReadObject();
        Cotxe* readCar(QString, GLfloat, GLfloat,GLfloat,GLfloat,double, double, double, float,float,float);


    private:
        QHash<QString,QString> readmtl(QString filename);
        void readObj(QString, Objecte*);
        void buildFace(char**, int, vector<Cara*>*, vector<point4>*, int);
};

#endif // READOBJECT_H
