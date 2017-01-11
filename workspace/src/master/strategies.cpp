#include "master/master.h"

/** Include all strategies here
 */


void Master::strategy_offensive(){
    switch(s_case){
    case INIT:
        // Our Team is behind
        if (ball.GetPos().DistanceTo(Position(1.38, 0))<0.8){
            s_case = SHOOT;
        }
        else {
            s_case = INTERRUPT;
        }

    case SHOOT:
        if (!player[1].isBusy()) {
            send(Command(ACTION_KICK, Position(1.38, 0), 2.2), 1);
            s_case = INIT;
        }
        break;

    case INTERRUPT:
        if (!player[0].isBusy() && !player[1].isBusy()){
            send(Command(ACTION_GOTO, Position(1.2, 0), 1.5), 1);
            send(Command(ACTION_PASS, Position(player[1].getX(),player[1].getY())), 0);
            s_case = INIT;
        }
        break;


    default:
        cout << "No case for this case in strategy offensive" << endl;
        break;
    }
}



void Master::strategy_defensive(){
    switch(s_case) {
    case INIT:
        // Fix this so it works for both sides
        if (ball.GetPos().GetX() > -0.2) {
            s_case = BLOCK;
        } else if (ball.GetPos().GetX() < -0.2) {
            s_case = SHOOT_AT_GOAL;
        }
    case BLOCK:
        tactic_nearpenaltyarea(-0.2);
        break;
    case SHOOT_AT_GOAL:
        kickAtGoal();
        break;
    case WAIT:
        // If the ball is outside of the goal again, kick ball
        if (ball.GetPos().GetX() > -0.2) {
            s_case = BLOCK;
        } else if (ball.GetPos().GetX() > -0.6 ||
            (fabs(ball.GetPos().GetY()) && ball.GetPos().GetX() < -0.6)) {
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
