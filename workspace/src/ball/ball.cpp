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

bool Ball::inGoalArea(int side) {
    cout << "PosX: " << GetPos().GetX() << " PosY: " << GetPos().GetY() << endl;
    if (side == -1 &&
        fabs(RawBall::GetPos().GetY()) < 0.350 &&
        fabs(RawBall::GetPos().GetX()) > 1.15) {
        return true;
    } else {
        return false;
    }
}
