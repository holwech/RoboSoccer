#include "master/master.h"

/** This is where all tactics should be included
 * 	All tactics should return true when they're DONE
 */

void Master::exampleTactic() {
    Position target = Position(1.0, 0.0);
    if (player[0].getState() == IDLE && !player[0].isBusy()) {
        send(Command(ACTION_BEFORE_KICK, ball.GetPos(), target), 0);
    } else if (player[0].getPrevState() == BEFORE_KICK && !player[0].isBusy()){
        send(Command(ACTION_KICK, target), 0);
    } else if (player[0].getPrevState() == KICK && !player[0].isBusy()) {
        send(Command(ACTION_GOTO, target), 0);
    }
}

/** This function will place one robot at one corner of the field
 *  and then the other robot will pass the ball to this player.
 *  The player will then try to kick at the goal.
 */
bool Master::crossPassAndShoot() {
    switch(t_state) {
    // Position robots accordingly
    case STEP1:
        send(Command(ACTION_GOTO, Position(0.5, 0.5)), 1);
//        send(Command(ACTION_BEFORE_KICK, ball.GetPos(), player[1].getPos()), 2);
        t_state = STEP2;
        break;
    // Pass the ball to the other robot
    case STEP2:
        if (!player[1].isBusy()) {
           send(Command(ACTION_BEFORE_KICK, ball.GetPos(), player[1].getPos()), 2);
           t_state = STEP3;
        }
        break;
    case STEP3:
        if (!player[2].isBusy()) {
            send(Command(ACTION_PASS, player[1].getPos()),2);
           t_state = STEP4;
        }
        break;
    // Position the receiving robot according to the ball
    case STEP4:
 //       if (!player[2].isBusy()) {
         if (ball.GetVelocity()<0.0001&&!player[2].isBusy()) {
           send(Command(ACTION_BEFORE_KICK, ball.GetPos(), Position(1.0, 0.0)), 1);
           t_state = STEP5;
        }
        break;
    // Kick the ball towards the goal
    case STEP5:
        if (!player[1].isBusy()) {
//         send(Command(ACTION_KICK, Position(1.0, 0.0)), 1);
//         send(Command(ACTION_PASS, Position(1.0, 0.0)), 1);

            t_state = STEP6;
        }
        break;
    // When done kicking, terminate tactic
    case STEP6:
        if (!player[1].isBusy()) {
            return true;
        }
        break;
    }
    return false;
}
