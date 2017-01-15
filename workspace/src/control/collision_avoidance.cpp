#include "collision_avoidance.h"
#include "ball/ball.h"

CA::CA() {
    //Adding goalArea obstacle-Positions
    //GoalArea towards pc-s
    //right
    goalArea.push_back(Position(1.35, -0.365));
    goalArea.push_back(Position(1.42, -0.365));
    //left
    goalArea.push_back(Position(1.41, 0.349));
    goalArea.push_back(Position(1.34, 0.349));
    //mid
    goalArea.push_back(Position(1.29, 0.0));
    //GoalArea towards door
    //todo
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
    double tempX = force.X;
    if (passSide < 0) {
        force.X = force.Y;
        force.Y = -tempX;
    } else {
        force.X = -force.Y;
        force.Y = tempX;
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

/** Field boundaries: {-1.383, 0.89}, {1.422, 0.876}, {1.465, -0.924}, {-1.466, -0.884}
  * Goal boundaries
  * Goal (not by the door): {1.414, 0.338}, {1.191, 0.341}, {1.205, -0.354}, {1.430, -0.362}
  * Goal (by the door): {-1.389, 0.369}, {-1.174, 0.360}, {-1.194, -0.332}, {-1.415, -0.325}
  */

/** This program is just based on approx. boundaries, and probably needs some tuning*/
Force CA::getWallPull(Position& basePos, Position& target, double scale = 0.1) {
    bool debug = false;
    Position leftPos(-1.383, basePos.GetY());
    Position rightPos(1.422, basePos.GetY());
    if (basePos.GetY() <= 0.35 && basePos.GetY() >= -0.35 && fabs(basePos.GetX()) < 1.2) {
        leftPos.SetX(-1.2);
        rightPos.SetX(1.2);
    }
    Position topPos(basePos.GetX(), 0.876);
    if (topPos.GetX() >= 1.2 && (basePos.GetY() < -0.360)) {
        if(debug){
            cout << "IN TOP POS RIGHT" << endl;
        }
        topPos.SetY(-0.360);
    } else if (topPos.GetX() <= -1.2 && basePos.GetY() < -0.360) {
        if(debug){
            cout << "IN TOP POS LEFT" << endl;
        }
        topPos.SetY(-0.360);
    }
    Position botPos(basePos.GetX(), -0.884);
    if (botPos.GetX() >= 1.2 && basePos.GetY() > 0.360) {
        if(debug){
            cout << "IN BOT POS RIGHT" << endl;
        }
        botPos.SetY(0.360);
    } else if (botPos.GetX() <= -1.2 && basePos.GetY() > 0.360) {
        if(debug){
            cout << "IN BOT POS LEFT" << endl;
        }
        botPos.SetY(0.360);
    }
    Force left = getForce(basePos.GetX(), basePos.GetY(), leftPos.GetX(), leftPos.GetY());
    Force right = getForce(basePos.GetX(), basePos.GetY(), rightPos.GetX(), rightPos.GetY());
    Force top = getForce(basePos.GetX(), basePos.GetY(), topPos.GetX(), topPos.GetY());
    Force bot = getForce(basePos.GetX(), basePos.GetY(), botPos.GetX(), botPos.GetY());
    if (debug) {
        cout << "----- ----- CA::getWallPull ----- -----" << endl;
        cout << "leftPos: " << leftPos.GetX() << ", " << leftPos.GetY() << endl;
        cout << "left: " << left.X << ", " << left.Y << endl;
        cout << "rightPos: " << rightPos.GetX() << ", " << rightPos.GetY() << endl;
        cout << "right: " << right.X << ", " << right.Y << endl;
        cout << "topPos: " << topPos.GetX() << ", " << topPos.GetY() << endl;
        cout << "top: " << top.X << ", " << top.Y << endl;
        cout << "botPos: " << botPos.GetX() << ", " << botPos.GetY() << endl;
        cout << "bot: " << bot.X << ", " << bot.Y << endl;
    }
    Force boundaryForce = {(left.X + right.X + top.X + bot.X) * scale, (left.Y + right.Y + top.Y + bot.Y) * scale, 0.0, 0.0, 0.0};
    return boundaryForce;
}

Force CA::getTotalPull(Position basePos, Position target, vector<Position>& team, vector<Position>& otherTeam, bool gravity = false) {
    Force totalForce = {0.0, 0.0, 0.0, 0.0, 0.0};
    Force temp;
    for (Position &obstacle : team) {
        temp = getPull(basePos, target, obstacle);
        totalForce.X += temp.X;
        totalForce.Y += temp.Y;
    }
    for (Position &obstacle : otherTeam) {
        temp = getPull(basePos, target, obstacle);
        totalForce.X += temp.X;
        totalForce.Y += temp.Y;
    }
  //  Force wallPull = getWallPull(basePos, target);
    Force wallPull = {0,0,0,0,0};
    totalForce.X += wallPull.X;
    totalForce.Y += wallPull.Y;

    totalForce.len = sqrt(pow(totalForce.X, 2) + pow(totalForce.Y, 2));
    Angle angle = basePos.AngleOfLineToPos(Position(basePos.GetX() + totalForce.X, basePos.GetY() + totalForce.Y));
    totalForce.deg = angle.Deg();
    totalForce.rad = angle.Get();
    cout << "THIS SHOULD NOT PRINT, IF IT DOES COLLISOIN AVOIDANCE IS ON" << endl;
    return totalForce;
}

Force CA::getBallPull(Position basePos, Position target, Position ballPos){
    Force totalForce = getPull(basePos, target, );
    totalForce.len = sqrt(pow(totalForce.X, 2) + pow(totalForce.Y, 2));
    Angle angle = basePos.AngleOfLineToPos(Position(basePos.GetX() + totalForce.X, basePos.GetY() + totalForce.Y));
    totalForce.deg = angle.Deg();
    totalForce.rad = angle.Get();
    return totalForce;
}
