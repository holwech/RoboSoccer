#ifndef ATTACKER_H
#define ATTACKER_H

#include "raw_ball.h"
#include "position.h"
#include "robo/robo.h"
#include "atomic"


/** These functions should be called doing something else probably
 *  I was just lazy, so copy paste from goalkeeper
 **/
enum AState {
    A_DEFEND,
    A_GOAL_KICK,
    A_GOAL_KICK_TO_TEAM,
    A_IDLE,
    A_MOVING
};


typedef enum {
    PREPARE_PASS,
    PASS,
    KICK,
    DEFEND
} Action;

struct Command {
    Action action;
};

/**
 *	Remember to have everything async. That means: no while-loops that block for a certain time, no usleep.
 * 	Do not use anything that blocks stalls the function.
 * 	While-loops can be replaced by if-statements, and usleep can be replaced by timers.
 */
class Attacker
{
public:
    Attacker(vector<Position>& positions, Position& ball, Command& command) : positions(positions), ball(ball), command(command) {};
    void run();
    AState getState();
    AState getPrevState();
    void setState(AState newState);
private:
    Position defend();
    Position goalKick();
    Position goalKickToTeam();
    /** 0 is goalkeeper
     *  1 and 2 is team playes
     *  3-5 is other team
     */
    vector<Position> positions;
    Position ball;
    Command command;

    AState prevState;
    AState state;
};
#endif // ATTACKER_H
