#ifndef RODA_H
#define RODA_H

#include "objecte.h"
#include "cara.h"

enum WheelType { ESQUERRA_POSTERIOR, DRETA_POSTERIOR, ESQUERRA_DAVANTERA, DRETA_DAVANTERA };

class Roda : public Objecte {
    public:

        Roda(vector<point4>, vector<Cara>);

        WheelType type;
};

#endif // RODA_H
