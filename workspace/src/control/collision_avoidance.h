#ifndef COLLISION_AVOIDANCE_H
#define COLLISION_AVOIDANCE_H

#include <math.h>
#include <vector>
#include "position.h"

#define CUTOFF_RADIUS 0.5
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
    Force getTotalPull(Position basePos, Position ballPos, Position target, vector<Position>& team, vector<Position>& otherTeam, bool gravity);
    Force getBallPull(Position basePos, Position target, Position ballPos);
private:
    void normalize(Force& force);
    Force forceAtPoints(Position& position, vector<Position>& obstacles);
    Force getForce(double X, double Y, double obstacleX, double obstacleY);
    double getPassSide(Position& basePos, Position& target, Position& obstacle);
    void toPerp(Force& force, double passSide);
    Force getWallPull(Position& basePos, Position& target, double scale);
    int obstacleWeight;
    std::vector<Position> goalArea;
};

#endif // COLLISION_AVOIDANCE_H
