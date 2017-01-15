#ifndef BALL_H
#define BALL_H
#include "raw_ball.h"
class Ball : public RawBall
{
public:
    Ball(RTDBConn &DBC);
    double GetVelocity();

};

#endif // BALL_H
