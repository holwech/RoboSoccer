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
    Player(Channel* channel, RTDBConn& DBC, const int deviceNr);
    void run();
    void update(vector<Position> pos);
    PState getState();
    PState getPrevState();
    Position getPos();
    bool isBusy();
    void setBusy(bool flag);
    Player(Player&& other);
    Player(const Player& other);
    Player& operator = (Player&& other);
    Player& operator = (const Player& other);
private:
    Position position(int robot);
    void readCommand();
    void setState(PState newState);
    void updateRobo();
    void done();
    /** 0 is goalkeeper
     *  1 and 2 is team playes
     *  3-5 is other team
     */
    RTDBConn& DBC;
    int deviceNr;
    vector<Position> positions;
    RawBall ball;
    Channel* channel;
    Command command;
    Robo robo;
    atomic<PState> prevState;
    atomic<PState> state;
    atomic<bool> busy;
    int side;
    mutable std::mutex mutex;

    /** General Variables and Functions*/
    void idle();
    void goTo(Position target);
    bool before_kick(Position kick_position, Position target_of_kick); //Get to position before kick -> can be used for attacker's kick and pass
    void kick(Position target);
    void pass(Position target);
    void drivingKick(Position target);
    int phase;
    Position pos_before_kick;
    Position aux_pos_before_kick;
    double counter;
    int control;
    Angle ang;
    double delta;

    /** Goalkeeper variables and Functions */
    void defend();
    void goalkeeperkick();
    double ballangle;
    double ballx;
    double bally;
    double gk_delta;
    timer timergk;

    /** Attacker variables and functions */
    void blockBall(int xcord);

};
#endif // PLAYER_H
