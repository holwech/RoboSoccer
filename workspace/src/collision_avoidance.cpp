#include "collision_avoidance.h"

CA::CA() {
    obstacleWeight = 1;
}

Force CA::getForce(double X, double Y, double obstacleX, double obstacleY) {
    bool debug = false;
    double magnitude = obstacleWeight / (pow(obstacleX - X, 2) + pow(obstacleY - Y, 2));
    Force force = {obstacleX - X, obstacleY - Y};

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

    return force;
}

void CA::normalize(Force& force) {
    double length = sqrt(pow(force.X, 2) + pow(force.Y, 2));
    force.X = force.X / length;
    force.Y = force.Y / length;
}


Force CA::forceAtPoints(Position position, vector<Position> obstacles) {
    Force force = {0, 0};
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
