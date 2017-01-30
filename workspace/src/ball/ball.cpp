#include "ball.h"

Ball::Ball(RTDBConn &DBC): RawBall(DBC), ballTimer(), refreshTimer()
{
    ballTimer.start();
    refreshTimer.start();
    for(int sample = 0; sample < NUM_SAMPLES; sample++) {
        posSamples.push_back(RawBall::GetPos());
        velocitySamples.push_back(RawBall::GetVelocity());
        if (sample < 5) {
            phiSamples.push_back(RawBall::GetPhi());
        }
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
    for (int i = 0; i < 4; i++) {
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
    samples.pop_back();
    samples.erase(samples.begin());
    samples.erase(samples.begin());
    double total = 0;
    for (auto& n : samples) {
        total += n;
    }
    return total / 6;
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
    total /= 3;
    Angle newAngle(total);
    prevAngle = newAngle;
    return newAngle;
}

bool Ball::inGoalArea() {
    if (fabs(RawBall::GetPos().GetY()) < 0.350 &&
        fabs(RawBall::GetPos().GetX()) > 1.15) {
        return true;
    } else {
        return false;
    }
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
