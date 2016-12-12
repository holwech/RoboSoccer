#ifndef GOALKEEPER_H
#define GOALKEEPER_H

#include "raw_ball.h"
#include "position.h"
#include "robo/robo.h"
#include "test_goalkeeper.h"

enum GState {
    DEFEND,
    GOAL_KICK,
    GOAL_KICK_TO_TEAM,
    IDLE,
    MOVING
};


/**
 *	Remember to have everything async. That means: no while-loops that block for a certain time, no usleep.
 * 	Do not use anything that blocks/stalls the function.
 * 	While-loops can be replaced by if-statements, and usleep can be replaced by timers.
 */
class Goalkeeper
{
    friend class TestGoalkeeper;
public:
    Goalkeeper(vector<Robo*>& robot, RawBall& ball) : robot(robot), ball(ball) {}
    Position nextMove();
    GState getState();
    GState getPrevState();
    void setState(GState newState);
private:
    void update();
    Position defend();
    Position goalKick();
    Position goalKickToTeam();
    vector<Robo*> robot;
    RawBall ball;
    /** 0 is goalkeeper
     *  1 and 2 is team playes
     *  3-5 is other team
     */
    vector<Position> positions;
    Position ballPos;
    GState prevState;
    GState state;
};

#endif // GOALKEEPER_H
