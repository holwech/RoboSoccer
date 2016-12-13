#ifndef PLAYER_H
#define PLAYER_H

#include "raw_ball.h"
#include "position.h"
#include "robo/robo.h"
#include "atomic"
#include "config.cpp"
#include "timer.h"


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
    friend class Test_player;
public:
    Player(vector<Position>* positions, RawBall* ball, Channel* channel, Robo* robo);
    void run();
    PState getState();
    PState getPrevState();
    Player(Player&& other);
    Player(const Player& other);
    Player& operator = (Player&& other);
    Player& operator = (const Player& other);
private:
    void readCommand();
    void setState(PState newState);
    /** 0 is goalkeeper
     *  1 and 2 is team playes
     *  3-5 is other team
     */
    vector<Position>* positions;
    RawBall* ball;
    Channel* channel;
    Command command;
    Robo* robo;
    atomic<PState> prevState;
    atomic<PState> state;
    mutable std::mutex mutex;

    /** General Variables and Functions*/
    void idle();
    void goTo(Position target);
    void before_kick(Position kick_position, Position target_of_kick); //Get to position before kick -> can be used for attacker's kick and pass
    Position pos_before_kick;
    Position aux_pos_before_kick;
    int control;
    Angle ang;
    double delta;

    /** Goalkeeper variables and Functions */
    void defend();
    void goalkeeperkick();
    double gk_delta;
    timer timergk;

    /** Attacker variables and functions */

};
#endif // PLAYER_H
