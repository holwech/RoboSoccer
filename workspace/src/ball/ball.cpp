#include "ball.h"

Ball::Ball(RTDBConn &DBC): RawBall(DBC)
{
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

