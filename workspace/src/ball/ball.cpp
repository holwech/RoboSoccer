#include "ball.h"

Ball::Ball(RTDBConn &DBC): RawBall(DBC), ballTimer(), sampleCount(0)
{
    ballTimer.start();
    for(int sample = 0; sample < NUM_SAMPLES; sample++) {
        samples.push_back(RawBall::GetPos());
    }
}

void Ball::updateSample() {
    if (ballTimer.getTime() > std::chrono::duration<double, std::milli>(35)) {
        cout << "Before: " << samples[sampleCount] << endl;
        samples[sampleCount] = RawBall::GetPos();
        cout << "After: " << samples[sampleCount] << endl;
        ballTimer.reset();
        sampleCount++;
        // Reset to first samples in vector
        if (sampleCount == NUM_SAMPLES) {
            sampleCount = 0;
        }
    }
}
/*
Position Ball::GetPos() {
    vector<Position> newSamples = samples;
    double avgX = 0;
    double avgY = 0;
    cout << "Positions: ";
    for (auto& pos : newSamples) {
        cout << pos << ", ";
        avgX += pos.GetX();
        avgY += pos.GetY();
    }
    cout << endl;
    avgX /= NUM_SAMPLES;
    avgY /= NUM_SAMPLES;
    Position avgPos(avgX, avgY);
    cout << "Average pos: {" << avgX << ", " << avgY << "}" << endl;
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
    cout << "New average pos: {" << newAvgX << ", " << newAvgY << "}" << endl;
    return Position(newAvgX, newAvgY);
    return Position(0,0);
}

*/

Position Ball::predictInY(double xLine) {
    return Position(xLine, tan(GetPhi() * M_PI / 180) * (xLine - GetPos().GetY()) + GetPos().GetY());
}

double Ball::GetVelocity() {
    vector<double> samples;
    for (int sample = 0; sample < 10; sample++) {
        samples.push_back(RawBall::GetVelocity());
    }
    std::sort(samples.begin(), samples.end());
    samples.pop_back();
    samples.erase(samples.begin());
    double total = 0;
    for (auto& n : samples) {
        total += n;
    }
    return total / 8;
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
