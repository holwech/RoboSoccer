#include "master/master.h"

/** Include all strategies here
 */


void Master::strategy_offensive(){
    double x = ball.GetPos().DistanceTo(Position(1.38,0));
    switch(s_case){
    case INIT:{
        // Our Team is behind
        cout << "Distance:" << x << endl;
        if( x <= 0.6 ){
            s_case = SHOOT;
        }
        else if( x > 0.6){
            s_case = POSITION;
        }

    }
    case SHOOT: {
        if (!player[0].isBusy() && !player[1].isBusy()) {
            send(Command(ACTION_KICK, Position(1.38, 0), 2.2), 1);
            s_case = INIT;
        }
        break;
    }
    case POSITION:{
        if (!player[0].isBusy() && !player[1].isBusy()){
            send(Command(ACTION_GOTO, Position(1.2, 0), 1.5), 1);
            s_case = INTERRUPT;
        }
        break;
    }
    case INTERRUPT: {
        if (!player[0].isBusy() && !player[1].isBusy()){
            send(Command(ACTION_PASS, Position(player[1].getX(),player[1].getY())), 0);
            s_case = INIT;
        }
        break;
    }
    default:
        cout << "No case for this case in strategy offensive" << endl;
        break;
    }
}



void Master::strategy_defensive(){
    switch(s_case) {
    case INIT:
        // Fix this so  it works for both sides
        if (ball.GetPos().GetX() > -0.2) {
            cout << "Strategy: Blocking ball" << endl;
            s_case = BLOCK;
        } else if (ball.GetPos().GetX() < -0.2) {
            cout << "Strategy: Shooting ball at goal" << endl;
            s_case = SHOOT_AT_GOAL;
        }
    case BLOCK: {
        bool nearPenaltyDone = tactic_nearpenaltyarea(-0.2);
        if (nearPenaltyDone) {
            cout << "Strategy: Wait" << endl;
            resetTVariables();
            s_case = WAIT;
        }
        break;
    }
    case SHOOT_AT_GOAL: {
        bool kickAtGoalDone = kickAtGoal();
        if (kickAtGoalDone) {
            cout << "Strategy: Wait" << endl;
            resetTVariables();
            s_case = WAIT;
        }
        break;
    }
    case WAIT:
        // If the ball is outside of the goal again, kick ball
        if (ball.GetPos().GetX() > -0.2 && ball.GetVelocity() < 0.01) {
            cout << "Strategy: Blocking ball" << endl;
            s_case = BLOCK;
        } else if (((ball.GetPos().GetX() > -0.6) ||
            (fabs(ball.GetPos().GetY()) > 0.2 && ball.GetPos().GetX() < -0.6)) &&
             ball.GetVelocity() < 0.01) {
            cout << "Strategy: Shooting ball at goal" << endl;
            s_case = SHOOT_AT_GOAL;
        }
        break;
    default:
        cout << "No case for this case in strategy defensive" << endl;
        break;
    }
}

void Master::strategy_demo(){

    if(referee.GetLeftSideGoals() > referee.GetRightSideGoals()){

    //do strategy_defensive();

        }


    //do strategy_offensive();




    /*
     * if ahead on score:
     * 		do strategy_defensive();
     * else:
     * 		do strategy_offensive();
     */
}
