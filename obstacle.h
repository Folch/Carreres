#ifndef OBSTACLE_H
#define OBSTACLE_H

#include "objecte.h"

class Obstacle : public Objecte {
    public:
        Obstacle(double, double, double, double);
        Obstacle(vector<point4>, vector<Cara*>);
        virtual void make();

    private:
        void quad(int, int, int, int);
        static const int NumVerticesF = 6*6;
};

#endif // OBSTACLE_H
