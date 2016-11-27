#ifndef COLLISION_AVOIDANCE_H
#define COLLISION_AVOIDANCE_H

#include <math.h>
#include <vector>
#include "position.h"

struct Point {
    double X;
    double Y;
};

struct Force {
    double X;
    double Y;
    double len;
    double rad;
    double deg;
};


class CA {
    friend class Test;
public:
    CA();
    Force getPull(Position& basePos, Position& target, Position& obstacle);
private:
    void normalize(Force& force);
    Force forceAtPoints(Position& position, vector<Position>& obstacles);
    Force getForce(double X, double Y, double obstacleX, double obstacleY);
    double getPassSide(Position& basePos, Position& target, Position& obstacle);
    int obstacleWeight;
};

#endif // COLLISION_AVOIDANCE_H
