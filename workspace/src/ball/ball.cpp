#include "ball.h"

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


Position Ball::predictInY(double xLine) {
    return Position(xLine, tan(GetPhi() * M_PI / 180) * (xLine - GetPos().GetY()) + GetPos().GetY());
}

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

bool Ball::inGoalArea(int side) {
    if (fabs(RawBall::GetPos().GetY()) < 0.290/* 0.350*/ &&
        fabs(RawBall::GetPos().GetX()) > 1.24/*1.15*/) {
        if (side == 0 || (side == 1 && RawBall::GetPos().GetX() > 0) || (side == -1 && RawBall::GetPos().GetX() < 0)) {
            return true;
        }
    }
    return false;
}

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

bool Ball::isStopped() {
    if (Ball::GetVelocity() < 0.00008) {
        return true;
    } else {
        return false;
    }
}

bool Ball::onSideOfField() {
    return fabs(GetPos().GetY()) > 0.4;
}

bool Ball::closeToTeamGoal(int side) {
    return ((side == 1 && GetPos().GetX() > 0.8) || (side == -1 && GetPos().GetX() < -0.8));
}

bool Ball::inEnemyGoalArea(int side) {
    return inGoalArea(-side);
}

bool Ball::inTeamGoalArea(int side) {
    return inGoalArea(side);
}

bool Ball::movingTowardsTeamGoal(int side) {
    return fabs(predictInY(1.4 * side).GetY()) < 0.3 && !isStopped() &&
           ((side == 1 && fabs(GetPhi().Deg()) < 90) || (side == -1 && fabs(GetPhi().Deg()) > 90));
}

bool Ball::onTeamSide(int side) {
    return (GetPos().GetX() < 0 && side == -1) || (GetPos().GetX() > 0 && side == 1);
}
