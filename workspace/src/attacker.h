#ifndef ATTACKER_H
#define ATTACKER_H

#include "raw_ball.h"
#include "position.h"
#include "robo.h"


/** These functions should be called doing something else probably
 *  I was just lazy, so copy paste from goalkeeper
 **/
enum AState {
    DEFEND,
    GOAL_KICK,
    GOAL_KICK_TO_TEAM,
    IDLE,
    MOVING
};


/**
 *	Remember to have everything async. That means: no while-loops that block for a certain time, no usleep.
 * 	Do not use anything that blocks stalls the function.
 * 	While-loops can be replaced by if-statements, and usleep can be replaced by timers.
 */
class Attacker
{
public:
    Attacker(vector<Robo*>& robot, RawBall& ball) : robot(robot), ball(ball) {};
    Position nextMove();
    AState getState();
    AState getPrevState();
    void setState(AState newState);
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
    AState prevState;
    AState state;
};
#endif // ATTACKER_H
