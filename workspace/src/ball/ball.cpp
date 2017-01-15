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
    samples.erase(samples.begin());
    samples.erase(samples.end());
    double total = 0;
    for (auto& n : samples) {
        total += n;
    }
    return total / 8;
}
