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


/** Calculates the pull from the force given by the obstacle.
  * That is, a vector value that is perpendicular to the force from the obstacle
  * and pointing in the direction that is the shortest path around the obstacle
  * to the target.
  */
Force CA::getPull(Position& basePos, Position& target, Position& obstacle) {
    double passSide = getPassSide(basePos, target, obstacle);
    Force force = getForce(basePos.GetX(), basePos.GetY(), obstacle.GetX(), obstacle.GetY());
    Force perpForce;
    if (passSide < 0) {
        perpForce.X = force.Y;
        perpForce.Y = -force.X;
    } else {
        perpForce.X = -force.Y;
        perpForce.Y = force.X;
    }
    perpForce.len = sqrt(pow(perpForce.X, 2) + pow(perpForce.X, 2));
    Position forcePos(basePos.GetX() + perpForce.X, basePos.GetY() + perpForce.Y);
    Angle forceAngle = basePos.AngleOfLineToPos(forcePos);
    perpForce.rad = forceAngle.Get();
    perpForce.deg = forceAngle.Deg();
    return perpForce;
}
