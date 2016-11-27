#include "collision_avoidance.h"

CA::CA() {
    obstacleWeight = 1;
}

/** Return the force from an obstacle */
Force CA::getForce(double X, double Y, double obstacleX, double obstacleY) {
    bool debug = false;
    double magnitude = obstacleWeight / (pow(obstacleX - X, 2) + pow(obstacleY - Y, 2));
    Force force = {obstacleX - X, obstacleY - Y, 0, 0, 0};

    if (debug) {
      cout << "---------- CA.getForce ----------" << endl;
      cout << "Input X: " << X << " Y: " << Y << " obstacleX: " << obstacleX << " obstacleY: " << obstacleY<< endl;
      cout << "Magnitude: " << magnitude << endl;
      cout << "ForceX: " << force.X << " ForceY: " << force.Y << endl;
    }

    normalize(force);
    force.X = force.X * magnitude;
    force.Y = force.Y * magnitude;

    if (debug) {
      cout << "Normalized ForceX: " << force.X << " ForceY: " << force.Y << endl;
      cout << "----- ----- ------" << endl;
    }

    /** Change the sign since the values are opposite of what they
      * should be for some reason
      */
    force.X = -force.X;
    force.Y = -force.Y;
    return force;
}

/** Normalizes a vector */
void CA::normalize(Force& force) {
    double length = sqrt(pow(force.X, 2) + pow(force.Y, 2));
    force.X = force.X / length;
    force.Y = force.Y / length;
}

/** Calculates the total force on a object, based on multiple obstacles */
Force CA::forceAtPoints(Position& position, vector<Position>& obstacles) {
    Force force = {0, 0, 0, 0, 0};
    for (unsigned int obstacle = 0; obstacle < obstacles.size(); obstacle++) {
        Force obstacleForce = getForce(
                        position.GetX(),
                        position.GetY(),
                        obstacles[obstacle].GetX(),
                        obstacles[obstacle].GetY()
                    );
        force.X += obstacleForce.X;
        force.Y += obstacleForce.Y;
    }
    return force;
}

/** Checks to find shortest path to pass to reach its target.
  * Negative value indicates that target is on the right side,
  * positive indicates that target is on the left side of the obstacle.
  */
double CA::getPassSide(Position& basePos, Position& target, Position& obstacle) {
    bool debug = false;
    Angle baseToTarget = basePos.AngleOfLineToPos(target);
    Angle baseToObstacle = basePos.AngleOfLineToPos(obstacle);

    if (debug) {
        cout << "Base to target: " << baseToTarget.Get() << endl;
        cout << "Base to obstacle: " << baseToObstacle.Get() << endl;
        cout << "Diff (rad): " << (baseToObstacle - baseToTarget).Get() << endl;
        cout << "Diff (deg): " << (baseToObstacle - baseToTarget).Deg() << endl;
    }

    /** If the ball is exactly behinde the obstacle, go to the right */
    double passSide = (baseToObstacle - baseToTarget).Get();
    if (passSide == 0) {
        passSide = -1;
    }
    return passSide;
}

void CA::toPerp(Force& force, double passSide) {
    if (passSide < 0) {
        force.X = force.Y;
        force.Y = -force.X;
    } else {
        force.X = -force.Y;
        force.Y = force.X;
    }
}


/** Calculates the pull from the force given by the obstacle.
  * That is, a vector value that is perpendicular to the force from the obstacle
  * and pointing in the direction that is the shortest path around the obstacle
  * to the target.
  */
Force CA::getPull(Position& basePos, Position& target, Position& obstacle) {
    double passSide = getPassSide(basePos, target, obstacle);

    /** If the angle between the base, obstacle and target is larger than
      * 90 degrees, the force is set to 0. This is because the obstacle is passed
      * and does not need to be avoided anymore
      */
    if (fabs(passSide) >= 1.5708 ||
        (basePos.DistanceTo(obstacle) > basePos.DistanceTo(target) &&
         fabs(passSide) <= 1.5708)) {
        Force temp = {0.0, 0.0, 0.0, 0.0, 0.0};
        return temp;
    }
    Force force = getForce(basePos.GetX(), basePos.GetY(), obstacle.GetX(), obstacle.GetY());
    toPerp(force, passSide);

    force.len = sqrt(pow(force.X, 2) + pow(force.X, 2));
    Position forcePos(basePos.GetX() + force.X, basePos.GetY() + force.Y);
    Angle forceAngle = basePos.AngleOfLineToPos(forcePos);
    force.rad = forceAngle.Get();
    force.deg = forceAngle.Deg();
    return force;
}

Force CA::operator+=(const Force& force) {
   Force newForce;
   newForce
   return box;
}

Fraction& operator += (const Fraction &obj)
{
.  num = (num * obj.den) + (obj.num * den) ;
.  den = obj.den * den ;
.  return (*this) ;
}

Force CA::getTotalPull(Position basePos, Position target, vector<Position>& team, vector<Position>& otherTeam, bool gravity = false) {
    Force totalForce = {0.0, 0.0, 0.0, 0.0, 0.0};
    for (Position &obstacle : team) {
        Force temp = getPull(basePos, target, obstacle);
    }
}
