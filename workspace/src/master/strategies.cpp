#include "master/master.h"

/** Include all strategies here
 */
#define KICK_SPEED 3.0
#define APPROACH_SPEED 2.0

void Master::debugContinue() {
    if (0) {
        cout << ">> STRATEGY STATE: " << strategyStateNames[s_case] << endl;
        cout << ">> Continue (y)? " << endl;
        string ans;
        cin >> ans;
    }
}

void Master::statePrint(S_Case current_case) {
    if (current_case != s_case_prev) {
        cout << ">> STRATEGY STATE: " << strategyStateNames[current_case] << endl;
    }
    s_case_prev = current_case;
}

void Master::strategy_offensive3(){
    switch(s_case){
    case INIT:
        send(Command(ACTION_DEFEND), 0);
        send(Command(ACTION_GOTO, Position(0.5 * side, 0.4), 1.5), 1);
        send(Command(ACTION_GOTO, Position(0.1 * side, -0.4), 1.5), 2);
        cout << "Strategy defensive: WAIT" << endl;
        break;
    case WAIT:
        break;
    case BLOCK:
        send(Command(ACTION_BLOCK_BALL,Position(0,0)) , 1);
        send(Command(ACTION_GOTO, Position(0.5*-side, 0.0)), 1);
        break;
    case SHOOT_AT_GOAL:
        if(player[1].getPos().DistanceTo(ball.GetPos()) < player[2].getPos().DistanceTo(ball.GetPos()) + 0.3)
        {
            send(Command(ACTION_GOTO, Position(0.75 * -side, -0.4), 2.0), 2);
            send(Command(ACTION_KICK, Position(1.38*-side, 0), KICK_SPEED, APPROACH_SPEED), 1);
        }else{
            send(Command(ACTION_GOTO, Position(0.9 * -side, 0.4), 2.0), 1);
            send(Command(ACTION_KICK, Position(1.38*-side, 0), KICK_SPEED, APPROACH_SPEED), 2);
        }
        break;
    case NEXT:
        break;
    default:
        cout << "no case for this in offensive strategy" << endl;
        break;
    }
    offensiveNextMove();
}

void Master::offensiveNextMove(){
    switch(s_case){
    case INIT:
        s_case = WAIT;
        break;
    case WAIT:
        if (!player[1].isBusy() && !player[2].isBusy()) {
            cout << "Strategy offensive: NEXT" << endl;
            s_case = SHOOT_AT_GOAL;
        }
        break;
    case NEXT:
        /*if ((side == LEFT && ball.GetPos().GetX() > 0.2) ||
            (side == RIGHT && ball.GetPos().GetX() < -0.2)) {
            cout << "Strategy defensive: SHOOT_AT_GOAL" << endl;
            s_case = SHOOT_AT_GOAL;
        }*/
        break;
    case SHOOT_AT_GOAL:
            //shoot even if far away form opponent goal
        if (!((side == LEFT && ball.GetPos().GetX() > 0.2) ||
            (side == RIGHT && ball.GetPos().GetX() < -0.2)) ) {
            cout << "Strategy defensive: SHOOT_AT_GOAL" << endl;
            s_case = SHOOT_AT_GOAL;
        }
        break;
    default:
        cout << "No case for this in offensivenextmove function" << endl;
        break;
    }
}

void Master::strategy_offensive2()
{
    //////////////// 2 robots always push the ball towards the enemy goal, last one in goal

    // If robots behind ball, attack
    if(ball.GetX() > -1.3 && ball.GetX() < 0.9 &&  ball.GetY() > 0 &&  ball.GetX()> player[1].getX())
    {
        send(Command(ACTION_KICK, Position(1.3, 0.0), 2.6, 0.8), 1);
        cout << "state1" << endl;
    }else if(ball.GetX() > -1.3 && ball.GetX() < 0.9 && ball.GetY() < 0 && ball.GetX()> player[0].getX())
    {
        send(Command(ACTION_KICK, Position(1.3, 0.0), 2.6, 0.8), 0);
    }

   // If robots not behind ball, get behind ball
   if(ball.GetX() > -1.3 && ball.GetX() < 0.9  &&  ball.GetX()<= player[1].getX() && ball.GetY() > 0)
   {
       send(Command(ACTION_BEFORE_KICK, ball.GetPos(), Position(1.38,0.0), 2.0), 1);
       cout << "state2" << endl;
    } else if(ball.GetX() > -1.3 && ball.GetX() < 0.9  &&  ball.GetX() <= player[0].getX() && ball.GetY() < 0)
   {
      send(Command(ACTION_BEFORE_KICK, ball.GetPos(), Position(1.38,0.0), 2.0), 0);
   }

   // If ball is next to edge just drive robots towards it
  if(ball.GetX() < 1.5 && ball.GetX() > 1.3 && ball.GetY() > 0.4)
  {
      send(Command(ACTION_GOTO, ball.GetPos(), 1.4), 1);
      cout << "state3" << endl;
  }


  if(ball.GetX() < 1.5 && ball.GetX() > 1.3 && ball.GetY() < -0.4)
  {
      send(Command(ACTION_GOTO, ball.GetPos(), 1.4), 0);

  }

  // Part of field in front of penalty area: kick towards goal
  if(ball.GetX() < 1.2  && ball.GetX() >= 0.9 && fabs(ball.GetY()) < 0.4 && player[1].getPos().DistanceTo(ball.GetPos())<player[0].getPos().DistanceTo(ball.GetPos()))
  {
    send(Command(ACTION_KICK, Position(1.38, 0.0), 2.6, 0.6), 1);
    cout << "state4" << endl;
  } else if(ball.GetX() < 1.2  && ball.GetX() >= 0.9 && fabs(ball.GetY()) < 0.4)
  {
    send(Command(ACTION_KICK, Position(1.38, 0.0), 2.6, 0.6), 0);
  }

  // Tell goal keeper to defend

  send(Command(ACTION_DEFEND), 2);
}

void Master::strategy_offensive()
{
    double x = ball.GetPos().DistanceTo(Position(1.38,0));
    switch(s_case){
        case INIT:
            // Our Team is behind
            if( x <= 0.7 && x >= 0.1 ){
                s_case = SHOOT;
            }
            if( x > 0.7 ){
                s_case = POSITION;
            }
            else {
                s_case = INIT;
            }

        break;

        case SHOOT:
        if (!player[0].isBusy() && !player[1].isBusy() && ball.GetVelocity() < 0.01 ) {
            send(Command(ACTION_KICK, Position(1.38, 0), 2.2), 1);
            cout << "Kick" << endl;
        }
        s_case = INIT;
        break;

        case POSITION:
        if (!player[0].isBusy() && !player[1].isBusy() && ball.GetVelocity() < 0.01 ){
            send(Command(ACTION_GOTO, Position(0.9, 0), 1.0), 1);
            cout << "Position" << endl;
            }
        s_case = INTERRUPT;
        break;

        case INTERRUPT:
        if (!player[0].isBusy() && !player[1].isBusy() && ball.GetVelocity() < 0.01 ){
            send(Command(ACTION_PASS, Position(player[1].getX(),player[1].getY())), 0);
            cout << "PASS" << endl;
            }
        s_case = INIT;
        break;

        default:
            cout << "No case for this case in strategy offensive" << endl;
            break;
    }
}

Position Master::trackBall() {
    Position ballPos = ball.GetPos();
    double xLine = ballPos.GetX() + 0.5 * side;
    if (fabs(xLine) > 1.2) {
        xLine = 1.2 * side;
    }
    return ball.predictInY(xLine);
}

void Master::strategy_best() {
    switch(s_case) {
    case INIT:
        nextMove(true);
        break;
    case WAIT: {
        bool waitDone = false;
       /* if (!player[1].isBusy() && !player[2].isBusy()) {
            waitDone = true;
        }*/
        nextMove(waitDone);
        break;
    }
    case SHOOT_AT_GOAL:
      {
        bool kickAtGoalDone = kickAtGoal(1);
        send(Command(ACTION_GOTO, Position(trackBall()), 1.5, true), 2);
        nextMove(kickAtGoalDone);
        break;
      }
    case COUNTER: {
        bool throughDone = throughPass();
        nextMove(throughDone);
        break;
    }
    case BLOCK:
      {
        bool nearPenaltyDone = tactic_nearpenaltyarea(0.2 * -side);
        nextMove(nearPenaltyDone);
        break;
      }
    case POSITION_ENEMY:{
        send(Command(ACTION_GOTO, Position(0.8 * -side, 0.6), 2.5, true), 1);
        send(Command(ACTION_GOTO, Position(0.8 * -side, -0.6), 2.5, true), 2);
        nextMove(true);
        break;
    }
    case POSITION_TEAM:{
        send(Command(ACTION_GOTO, Position(0.8 * side, 0.6), 2.5, true), 1);
        send(Command(ACTION_GOTO, Position(0.8 * side, -0.6), 2.5, true), 2);
        nextMove(true);
        break;
    }
    default:
        cout << "No case for this in strategy defensive" << endl;
        break;
    }
}


void Master::nextMove(bool moveDone) {
    S_Case nextState = s_case;
    bool ballOnSideOfField = fabs(ball.GetPos().GetY()) > 0.4;
    bool ballCloseToTeamGoal = ((side == 1 && ball.GetPos().GetX() > 0.8) || (side == -1 && ball.GetPos().GetX() < -0.8));
    bool ballInEnemyGoalArea = ball.inGoalArea(-side);
    bool ballInTeamGoalArea = ball.inGoalArea(side);
    bool ballTowardsTeamGoal = fabs(ball.predictInY(1.4 * side).GetY()) < 0.3 &&
                                ((side == RIGHT && fabs(ball.GetPhi().Deg()) < 90) || (side == LEFT && fabs(ball.GetPhi().Deg()) > 90));
    bool ballOnTeamSide = (ball.GetPos().GetX() < 0 && side == LEFT) || (ball.GetPos().GetX() > 0 && side == RIGHT);
    if (ballInEnemyGoalArea && s_case == WAIT) {
        // wait
    } else if (ballInEnemyGoalArea) {
        nextState = POSITION_ENEMY;
        if (moveDone && s_case == POSITION_ENEMY) {
            nextState = WAIT;
        }
    } else if (ballInTeamGoalArea) {
        nextState = POSITION_TEAM;
        if (moveDone && s_case == POSITION_TEAM) {
            nextState = WAIT;
        }
    } else if (ballTowardsTeamGoal && ballOnTeamSide) {
        nextState = BLOCK;
        if (moveDone && s_case == BLOCK) {
            nextState = SHOOT_AT_GOAL;
        }
    } else if (ballOnSideOfField && ballCloseToTeamGoal) {
        nextState = COUNTER;
        if (moveDone) {
            nextState = SHOOT_AT_GOAL;
        }
    } else {
        nextState = SHOOT_AT_GOAL;
        if (moveDone) {
            resetTVariables();
        }
    }
    if (nextState != s_case) {
        resetTVariables();
        s_case = nextState;
        statePrint(s_case);
        debugContinue();
    }
}

// Player 1 is defender and player 2 is attacker
void Master::strategy_defensive() {
    switch(s_case) {
    case INIT:
        send(Command(ACTION_DEFEND), 0);
        send(Command(ACTION_GOTO, Position(0.5 * side, 0.0), 1.5), 1);
        send(Command(ACTION_GOTO, Position(0.1 * side, 0.0), 1.5), 2);
        cout << "Strategy defensive: WAIT" << endl;
        s_case = WAIT;
        debugContinue();
        break;
    case WAIT:
        if (!player[1].isBusy() && !player[2].isBusy()) {
            cout << "Strategy defensive: NEXT W" << endl;
            nextDefensiveMove();
        }
        break;
    case SHOOT_AT_GOAL:
      {
        bool kickAtGoalDone = kickAtGoal();
        if (kickAtGoalDone)
        {
          resetTVariables();
        }
        nextDefensiveMove();
        break;
      }
    case BLOCK:
      {
        bool nearPenaltyDone = tactic_nearpenaltyarea(0.2 * -side);
        if (nearPenaltyDone) {
            resetTVariables();
        }
        nextDefensiveMove();
        break;
      }
    default:
        cout << "No case for this in strategy defensive" << endl;
        break;
    }
}

void Master::nextDefensiveMove() {
    S_Case nextState = s_case;
    if (ball.inGoalArea()) {
        nextState = INIT;
    // If ball is on the other teams field, shoot at goal
    } else if ((side == LEFT && ball.GetPos().GetX() > 0.2) ||
        (side == RIGHT && ball.GetPos().GetX() < -0.2)) {
        nextState = SHOOT_AT_GOAL;
    } else if ((side == LEFT && ball.GetPos().GetX() <= 0.2) ||
                   (side == RIGHT && ball.GetPos().GetX() >= -0.2)) {
        nextState = BLOCK;
    }
    if (nextState != s_case) {
        resetTVariables();
        s_case = nextState;
        debugContinue();
    }
}

