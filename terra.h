#ifndef TERRA_H
#define TERRA_H

#include "objecte.h"

class Terra : public Objecte
{
public:
    Terra(float, float, float);
    Terra();

private:
    void init(float, float, float);
    void quad(int, int, int, int);
    static const int NumVerticesF = 6;
    static const int NumCares = 2;
};

#endif // TERRA_H
