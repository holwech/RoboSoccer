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
};

class CA {
public:
    CA();
private:
    Force getForce(Position& point, Position& obstacle);
    void normalize(Force& force);
    void forceAtPoint(Position position, vector<Position> obstacles);
    int obstacleWeight;
};

#endif // COLLISION_AVOIDANCE_H
