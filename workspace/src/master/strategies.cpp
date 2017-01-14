#include "master/master.h"

/** Include all strategies here
 */


void Master::strategy_offensive2()
{

    //////////////// 2 robots always push the ball towards the enemy goal

    // If robots behind ball attack,
    if(ball.GetX() > -1.25 && ball.GetX() < 0.9 &&  ball.GetY() > 0 &&  ball.GetX()> player[1].getX())
    {
        send(Command(ACTION_KICK, Position(1.3, 0.0), 2.6, 1), 1);

    }else if(ball.GetX() > -1.2 && ball.GetX() < 0.9 && ball.GetY() < 0 && ball.GetX()> player[0].getX())
    {

        send(Command(ACTION_KICK, Position(1.3, 0.0), 2.6, 1), 0);

    }

   // If robots not behind ball, attack otherwise get behind ball
   if(ball.GetX() > -1.2 && ball.GetX() < 0.9  &&  ball.GetX()<= player[1].getX() && player[1].getPos().DistanceTo(ball.GetPos())+0.05<player[0].getPos().DistanceTo(ball.GetPos()))
   {
       send(Command(ACTION_BEFORE_KICK, ball.GetPos(), Position(1.38,0.0), 2.0), 1);

    } else if(ball.GetX() > -1.2 && ball.GetX() < 0.9  &&  ball.GetX() <= player[0].getX() && player[0].getPos().DistanceTo(ball.GetPos())+0.05<player[1].getPos().DistanceTo(ball.GetPos()) )
   {

      send(Command(ACTION_BEFORE_KICK, ball.GetPos(), Position(1.38,0.0), 2.0), 1);

    }

   // If ball is next to edge just drive robots towards it
  if(ball.GetX() < 1.5 && ball.GetX() > 1.1 && ball.GetY() > 0.4)
  {
      send(Command(ACTION_GOTO, ball.GetPos(), 1.4), 1);

  }


  if(ball.GetX() < 1.5 && ball.GetX() > 1.1 && ball.GetY() < -0.4)
  {
      send(Command(ACTION_GOTO, ball.GetPos(), 1.4), 0);

  }

  // Part of field in front of penalty area when kick is used or alternatively driving
  if(ball.GetX() < 1.2  && ball.GetX() >= 0.9 && fabs(ball.GetY()) < 0.4 && player[1].getPos().DistanceTo(ball.GetPos())<player[0].getPos().DistanceTo(ball.GetPos()) && ball.GetVelocity()< 0.00001)
  {
    send(Command(ACTION_KICK, Position(1.38, 0.0), 2.6, 0.6), 1);

  } else if(ball.GetX() < 1.2  && ball.GetX() >= 0.9 && fabs(ball.GetY()) < 0.4)
  {
    send(Command(ACTION_GOTO, ball.GetPos(), 2), 0);
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
            send(Command(ACTION_GOTO, Position(0.9, 0), 1), 1);
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


void Master::strategy_defensive()
{
  switch (s_case)
  {
    case INIT:
      // Fix this so  it works for both sides
      if (ball.GetPos().GetX() > -0.2)
      {
        cout << "Strategy: Blocking ball" << endl;
        s_case = BLOCK;
      }
      else if (ball.GetPos().GetX() < -0.2)
      {
        cout << "Strategy: Shooting ball at goal" << endl;
        s_case = SHOOT_AT_GOAL;
      }
    case BLOCK:
      {
        bool nearPenaltyDone = tactic_nearpenaltyarea(-0.2);
        if (nearPenaltyDone)
        {
          cout << "Strategy: Wait" << endl;
          resetTVariables();
          s_case = WAIT;
        }
        break;
      }
    case SHOOT_AT_GOAL:
      {
        bool kickAtGoalDone = kickAtGoal();
        if (kickAtGoalDone)
        {
          cout << "Strategy: Wait" << endl;
          resetTVariables();
          s_case = WAIT;
        }
        break;
      }
    case WAIT:
      // If the ball is outside of the goal again, kick ball
      if (ball.GetPos().GetX() > -0.2 && ball.GetVelocity() < 0.01)
      {
        cout << "Strategy: Blocking ball" << endl;
        s_case = BLOCK;
      }
      else if (((ball.GetPos().GetX() > -0.6) ||
                (fabs(ball.GetPos().GetY()) > 0.2 && ball.GetPos().GetX() < -0.6)) &&
               ball.GetVelocity() < 0.01)
      {
        cout << "Strategy: Shooting ball at goal" << endl;
        s_case = SHOOT_AT_GOAL;
      }
      break;
    default:
      cout << "No case for this case in strategy defensive" << endl;
      break;
  }
}

void Master::strategy_demo()
{

  if (referee.GetLeftSideGoals() > referee.GetRightSideGoals())
  {

    //do strategy_defensive();

  }


  //do strategy_offensive();




  /*
   * if ahead on score:
   *    do strategy_defensive();
   * else:
   *    do strategy_offensive();
   */
}
