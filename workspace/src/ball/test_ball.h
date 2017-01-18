#ifndef TEST_BALL_H
#define TEST_BALL_H
#include "ball.h"
#include "master/master.h"

class test_ball {
public:
    test_ball(Ball& ball) : ball(ball) { }
    void test(int side);
private:
    Ball ball;
};



#endif // TEST_BALL_H
