#ifndef PLAYER_H
#define PLAYER_H

#include "raw_ball.h"
#include "position.h"
#include "robo/robo.h"
#include "atomic"
#include "config.cpp"


/** These functions should be called doing something else probably
 *  I was just lazy, so copy paste from goalkeeper
 **/
enum PState {
    IDLE,
    BEFORE_PASS,
    PASS,
    GOTO,
    BEFORE_KICK,
    KICK,
    BLOCK_BALL,
    DEFEND,
    KICK_OUT,
    TEST
};



/**
 *	Remember to have everything async. That means: no while-loops that block for a certain time, no usleep.
 * 	Do not use anything that blocks stalls the function.
 * 	While-loops can be replaced by if-statements, and usleep can be replaced by timers.
 */
class Player
{
public:
    Player(vector<Position>* positions, Position* ball, Channel* channel, Robo* robo);
    void run();
    PState getState();
    PState getPrevState();
    void setState(PState newState);
private:
    void readCommand();
    void goTo();
    Position defend();
    Position goalKick();
    Position goalKickToTeam();
    /** 0 is goalkeeper
     *  1 and 2 is team playes
     *  3-5 is other team
     */
    vector<Position>* positions;
    Position* ball;
    Channel* channel;
    Command command;
    Robo* robo;
    PState prevState;
    PState state;
};
#endif // PLAYER_H
