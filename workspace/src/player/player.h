#ifndef PLAYER_H
#define PLAYER_H

#include "ball/ball.h"
#include "position.h"
#include "robo/robo.h"
#include "atomic"
#include "config.cpp"
#include "timer.h"

#define PLAYER_DEBUG 0


/** These functions should be called doing something else probably
 *  I was just lazy, so copy paste from goalkeeper
 **/
enum PState {
    IDLE,
    STOP,
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

enum AState {
    A_STEP1,
    A_STEP2,
    A_STEP3,
    A_STEP4,
    A_STEP5,
    A_STEP6
};

enum fieldSide{
    LEFT = -1,
    RIGHT = 1
};

enum GoalkeeperState{
    BLOCK_WINDOW = 0,
    BLOCK_NOT_WINDOW = 1,
    GOALKEEPER_STOP,
    DYNAMIC_DEFEND,
    GOALKEEPER_KICK
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
    fieldSide side;
    PState getState();
    PState getPrevState();
    Position getPos();
    double getX();
    double getY();
    bool isBusy();
    void setBusy(bool flag);
    Player(Player&& other);
    Player(const Player& other);
    Player& operator = (Player&& other);
    Player& operator = (const Player& other);
private:
    Position position(int robot);
    enum{ STEP1 = 1, STEP2, STEP3} state_before_kick;
    void debugContinue();
    void playerPrint(string message);
    void playerPrintState(string message);
    void readCommand();
    void setState(PState newState);
    void updateRobo(bool isGoalkeeper);
    void done();
    bool angeled_behind_ball(Position targetPos, double speed = 1.5);
    PState getPrevPrevState();
    /** 0 is goalkeeper
     *  1 and 2 is team playes
     *  3-5 is other team
     */
    RTDBConn& DBC;
    int deviceNr;
    vector<Position> positions;
    Ball ball;
    Channel* channel;
    Command command;
    Robo robo;
    PState prevPrevState;
    atomic<PState> prevState;
    atomic<PState> state;
    atomic<bool> busy;
    mutable std::mutex mutex;

    /** General Variables and Functions*/
    void idle();
    bool stop();
    bool goTo(Position target, double speed = 1, bool ca = false);
    bool before_kick(Position kick_position, Position target_of_kick, double before_kick_speed); //Get to position before kick -> can be used for attacker's kick and pass
    bool before_kick_improved(Position kick_position, Position target_of_kick, double before_kick_speed);
    double lengthToBall;
    bool kick(Position target, double speed = 2.8, double approach_speed = 0.4);
    bool pass(Position target);
    void drivingKick(Position target);
    bool old_pass(Position target);
    bool old_kick(Position target);
    int phase;
    //Position pos_before_kick_far;
    //Position pos_before_kick_near;
    //Position aux_pos_before_kick;
    double counter;
    int control;
    AState pass_state;
    AState kick_state;
    Position endKickPos;
    double passSpeed;
    timer stuck_timer;

    Angle ang;
    double delta;

    /** Goalkeeper variables and Functions */
    void defend_tom();
    void defend();
    void goalkeeperkick();
    double ballangle;
    double ballx;
    double bally;
    double gk_delta;
    timer timergk;

    /** Attacker variables and functions */
    bool blockBall(int xcord);

    GoalkeeperState Gstate;
    void getNextGoalkeeperState();
};
#endif // PLAYER_H
