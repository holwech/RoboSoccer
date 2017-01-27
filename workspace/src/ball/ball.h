#ifndef BALL_H
#define BALL_H
#include "raw_ball.h"
#include "timer.h"
#define BOUND_X 1.3
#define BOUND_Y 0.780
#define NUM_SAMPLES 10

/**
 * LEFT = -x, RIGHT = x, TOP = y, BOTTOM = -y
 */
enum edges{
    E_LEFT,
    E_RIGHT,
    E_TOP,
    E_BOTTOM,
    E_TOP_LEFT,
    E_TOP_RIGHT,
    E_BOTTOM_LEFT,
    E_BOTTOM_RIGHT,
    E_NONE
};

class Ball : public RawBall
{
    friend class test_ball;
public:
    Ball(RTDBConn &DBC);
    double GetVelocity();
    //Position GetPos();
    bool inGoalArea();
    edges nearEdge();
    bool isStopped();
    void updateSample();
    Position predictInY(double xLine);
private:
    timer ballTimer;
    vector<Position> samples;
    int sampleCount;
};

#endif // BALL_H
