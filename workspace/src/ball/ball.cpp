#include "ball.h"

/**
 * @brief Default contructor for ball
 *
 * @param DBC reference to the real time database
 */
Ball::Ball(RTDBConn &DBC): RawBall(DBC), ballTimer(), refreshTimer()
{
    ballTimer.start();
    refreshTimer.start();
    for(int sample = 0; sample < NUM_SAMPLES; sample++) {
        posSamples.push_back(RawBall::GetPos());
        velocitySamples.push_back(RawBall::GetVelocity());
        phiSamples.push_back(RawBall::GetPhi());
    }
}

/**
 * @brief Updates the samples of the ball position, velocity and direction.
 * this function should be run every ~35ms
 *
 */
void Ball::updateSample() {
    if (ballTimer.getTime() > std::chrono::duration<double, std::milli>(35)) {
        posSamples.erase(posSamples.begin());
        posSamples.push_back(RawBall::GetPos());
        velocitySamples.erase(velocitySamples.begin());
        velocitySamples.push_back(RawBall::GetVelocity());
        phiSamples.erase(phiSamples.begin());
        phiSamples.push_back(RawBall::GetPhi());
        ballTimer.reset();
    }
}

/**
 * @brief Calculates the position of the ball based the samples
 *
 * @return Position a new estimated position
 */
Position Ball::GetPos() {
    vector<Position> newSamples = posSamples;
    double avgX = 0;
    double avgY = 0;
    //cout << "Positions: ";
    for (auto& pos : newSamples) {
        //cout << pos << ", ";
        avgX += pos.GetX();
        avgY += pos.GetY();
    }
    //cout << endl;
    avgX /= NUM_SAMPLES;
    avgY /= NUM_SAMPLES;
    Position avgPos(avgX, avgY);
    //cout << "Average pos: {" << avgX << ", " << avgY << "}" << endl;
    vector<double> distanceFromAvgPos;
    for (auto& pos : newSamples) {
        distanceFromAvgPos.push_back(pos.DistanceTo(avgPos));
    }
    for (int i = 0; i < 2; i++) {
        double maxRadius = 0;
        double index = 0;
        for (int j = 0; j < (int)distanceFromAvgPos.size(); j++) {
            if (maxRadius < distanceFromAvgPos[j]) {
                maxRadius = distanceFromAvgPos[j];
                index = j;
            }
        }
        distanceFromAvgPos.erase(distanceFromAvgPos.begin() + index);
        newSamples.erase(newSamples.begin() + index);
    }
    double newAvgX = 0, newAvgY = 0;
    for (auto& pos : newSamples) {
        newAvgX += pos.GetX();
        newAvgY += pos.GetY();
    }
    newAvgX /= newSamples.size();
    newAvgY /= newSamples.size();
    //cout << "New average pos: {" << newAvgX << ", " << newAvgY << "}" << endl;
    return Position(newAvgX, newAvgY);
    return Position(0,0);
}


/**
 * @brief Predicts where the ball is going to be in the y-axis based on a
 * given x coordinate.
 *
 * @param xLine The x coordinate where the y coordinate should be predicted on
 * @return Position The predicted position of the ball
 */
Position Ball::predictInY(double xLine) {
    return Position(xLine, tan(GetPhi() * M_PI / 180) * (xLine - GetPos().GetY()) + GetPos().GetY());
}

/**
 * @brief Calculates the velocity based on the samples
 *
 * @return double Returns a estimated velocity
 */
double Ball::GetVelocity() {
    vector<double> samples = velocitySamples;
    /*
    for (auto& vel : velocitySamples) {
        cout << vel << " # ";
    }
    cout << endl;
    */
    std::sort(samples.begin(), samples.end());
    samples.pop_back();
    samples.erase(samples.begin());
    double total = 0;
    for (auto& n : samples) {
        total += n;
    }
    return total / samples.size();
}

/**
 * @brief Calculates a angle phi of the ball based on the samples
 *
 * @return Angle Returns a estimates angle
 */
Angle Ball::GetPhi() {
    // If the ball is not moving, keep the trajectory before it stopped
    if (isStopped()) {
        return prevAngle;
    }
    vector<Angle> samples = phiSamples;
    /*
    for (auto& vel : phiSamples) {
        cout << vel << " * ";
    }
    cout << endl;
    */
    std::sort(samples.begin(), samples.end());
    samples.pop_back();
    samples.erase(samples.begin());
    /*
    for (auto& vel : samples) {
        cout << vel << " * ";
    }
    cout << endl;
    */
    double total = 0;
    for (auto& n : samples) {
        total += n.Get();
    }
    total /= samples.size();
    Angle newAngle(total);
    prevAngle = newAngle;
    return newAngle;
}

/**
 * @brief Checks whether the ball is in the goal area
 *
 * @param side Selects which goal area should be checked.
 * @return bool Return true if ball is in goal area
 */
bool Ball::inGoalArea(int side) {
    if (fabs(RawBall::GetPos().GetY()) < 0.32/* 0.350*/ &&
        fabs(RawBall::GetPos().GetX()) > 1.20/*1.15*/) {
        if (side == 0 || (side == 1 && RawBall::GetPos().GetX() > 0) || (side == -1 && RawBall::GetPos().GetX() < 0)) {
            return true;
        }
    }
    return false;
}

/**
 * @brief Check whether the ball is near the edges
 *
 * @return edges Returns the gives edge that the ball is close to
 */
edges Ball::nearEdge() {
    double X = RawBall::GetPos().GetX();
    double Y = RawBall::GetPos().GetY();
    if (X > BOUND_X) {
       if (Y < -BOUND_Y) {
           return E_BOTTOM_RIGHT;
       } else if (Y > BOUND_Y) {
           return E_TOP_RIGHT;
       } else {
           return E_RIGHT;
       }
    } else if (X < -BOUND_X) {
       if (Y < -BOUND_Y) {
           return E_BOTTOM_LEFT;
       } else if (Y > BOUND_Y) {
           return E_TOP_LEFT;
       } else {
           return E_LEFT;
       }
    } else if (Y < -BOUND_Y) {
        return E_BOTTOM;
    } else if (Y > BOUND_Y) {
        return E_TOP;
    } else {
        return E_NONE;
    }
}

/**
 * @brief Checks whether if the ball is moving
 *
 * @return bool Returns true if the ball is stopped
 */
bool Ball::isStopped() {
    if (Ball::GetVelocity() < 0.00008) {
        return true;
    } else {
        return false;
    }
}

/**
 * @brief Checks whether the ball is on the side of the field
 *
 * @return bool Returns true if ball is on side of field
 */
bool Ball::onSideOfField() {
    return fabs(GetPos().GetY()) > 0.4;
}

/**
 * @brief Checks whether the ball is close to team goal
 *
 * @param side Selects which side the team is playing on
 * @return bool Returns true if ball is close to team goal
 */
bool Ball::closeToTeamGoal(int side) {
    return ((side == 1 && GetPos().GetX() > 0.6) || (side == -1 && GetPos().GetX() < -0.6));
}

/**
 * @brief Checks whether the ball is in enemy goal area
 *
 * @param side Sets which side the friendly team is playing on
 * @return bool Returns true if ball is in enemy goal area
 */
bool Ball::inEnemyGoalArea(int side) {
    return inGoalArea(-side);
}

/**
 * @brief Checks whether the ball is in team goal area
 *
 * @param side Sets which side the friendly team is playing on
 * @return bool Returns true if ball is in team goal area
 */
bool Ball::inTeamGoalArea(int side) {
    return inGoalArea(side);
}

/**
 * @brief Checks whether the ball is moving towards team goal
 *
 * @param side Sets which side the friendly team is playing on
 * @return bool Returns true if the ball is moving towards the team goal
 */
bool Ball::movingTowardsTeamGoal(int side) {
    return fabs(predictInY(1.4 * side).GetY()) < 0.4 && !isStopped() &&
           ((side == 1 && fabs(GetPhi().Deg()) < 90) || (side == -1 && fabs(GetPhi().Deg()) > 90));
}

/**
 * @brief Checks whether the ball is on the team side of the field
 *
 * @param side Sets which side the friendly team is playing on
 * @return bool Returns true if the ball is on the team side
 */
bool Ball::onTeamSide(int side) {
    return (GetPos().GetX() < 0 && side == -1) || (GetPos().GetX() > 0 && side == 1);
}
