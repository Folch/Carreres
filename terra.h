#ifndef TERRA_H
#define TERRA_H

#include "objecte.h"

class Terra : public Objecte
{
public:
    Terra(float, float, float);
    Terra();
    virtual void make();

private:
    void init(float, float, float);
    static const int NumVerticesF = 6;
};

#endif // TERRA_H
