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
    Force forceAtPoints(Position position, vector<Position> obstacles);
    Force getForce(double X, double Y, double obstacleX, double obstacleY);
private:
    void normalize(Force& force);
    int obstacleWeight;
};

#endif // COLLISION_AVOIDANCE_H
