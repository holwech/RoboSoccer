#include "collision_avoidance.h"

CA::CA() {
}

Force CA::getForce(Point& point, Point& obstacle) {
  double magnitude = obstacleWeight / sqrt(
                       (pow(obstacle.X, 2.0) - pow(point.X, 2.0)) +
                       (pow(obstacle.Y, 2.0) - pow(point.Y, 2.0))
                     );
  Force force = {obstacle.X - point.X, obstacle.Y - point.Y};
  normalize(force);
  force.X = force.X * magnitude;
  force.Y = force.Y * magnitude;
  return force;
}

void CA::normalize(Force& force) {
  double length = sqrt(pow(force.X, 2.0) + pow(force.Y, 2.0));
  force.X = force.X / length;
  force.Y = force.Y / length;
}

void CA::forceAtPoint(Position position, vector<Position> obstacles) {
  Force force = {0, 0};
  for (int obstacle = 0; obstacle < obstacles.size(); obstacle++) {
    Force obstacleForce = getForce(position, obstacles[obstacle]);
    force.X += obstacleForce.X;
    force.Y += obstacleForce.Y;
  }
  return force;
}
