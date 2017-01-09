#include "master/master.h"

/** This is where all tactics should be included
 *  All tactics should return true when they're DONE
 */

void Master::exampleTactic()
{
  Position target = Position(1.0, 0.0);
  if (player[0].getState() == IDLE && !player[0].isBusy())
  {
    send(Command(ACTION_BEFORE_KICK, ball.GetPos(), target), 0);
  }
  else if (player[0].getPrevState() == BEFORE_KICK && !player[0].isBusy())
  {
    send(Command(ACTION_KICK, target), 0);
  }
  else if (player[0].getPrevState() == KICK && !player[0].isBusy())
  {
    send(Command(ACTION_GOTO, target), 0);
  }
}

/** This function will place one robot at one corner of the field
 *  and then the other robot will pass the ball to this player.
 *  The player will then try to kick at the goal.
 */
bool Master::crossPassAndShoot()
{
  switch (t_state)
  {
      // Position robots accordingly
    case STEP1:
      if(ball.GetY()>0){
          chrossandpassy=-0.5;
      }
      else{
          chrossandpassy=0.5;
      }
      send(Command(ACTION_GOTO, Position(0.7, chrossandpassy), 3 ), 1);
      t_state = STEP2;
      break;
      // Pass the ball to the other robot
    case STEP2:
      if (!player[1].isBusy())
      {
        send(Command(ACTION_BEFORE_KICK, ball.GetPos(), Position(player[1].getX()+0.3,chrossandpassy)), 2);
        t_state = STEP3;
      }
      break;
    case STEP3:
      if (!player[2].isBusy())
      {
        send(Command(ACTION_PASS, Position(player[1].getX()+0.3,chrossandpassy)), 2);
        t_state = STEP4;
     }
      break;
      // Position the receiving robot according to the ball
    case STEP4:
      //       if (!player[2].isBusy()) {
      if (ball.GetVelocity() < 0.00001 && !player[2].isBusy() && !player[1].isBusy())  //wait for the ball stop, if not stopping, the ball.GetPos() will not updating because the state changes.
      {
        usleep(4000000);
        send(Command(ACTION_BEFORE_KICK, ball.GetPos(), Position(1.4, 0.0)), 1);
        t_state = STEP5;
      }
      break;
      // Kick the ball towards the goal

    case STEP5:
      cout << "5-----------" << ball.GetPos() << endl;
      if (!player[1].isBusy())
      {
        send(Command(ACTION_KICK, Position(1.4, 0.0)), 1);
        t_state = STEP6;
      }
      break;
      // When done kicking, terminate tactic
    case STEP6:
      if (!player[1].isBusy())
      {
        return true;
      }
      break;
  }
  return false;
}

/*  bool Master::tactic_nearpenaltyarea()
    02,01,2017, Song
This function of this tactic is if the ball is too near our penalty area (ball.Getx()>1), we should kick it far away from our gate,
in this case i put the (-1.0,0.0) as the target point, and robot2 is choosen to do this.

For the Milestone I think it is almost enough.

Can be improved, in my opinion
1.target point
2.choose the nearest robot (except goalie) to do this function
3.If the ball is on the bottom border, the robot will stuck in the wall
4.and so on
*/
bool Master::tactic_nearpenaltyarea()
{
  // if the ball is too close to our gate/ penalty area
  if (ball.GetX() > 0.65)
  {


    // Position robots accordingly
    switch (t_state)
    {
      case STEP1:

        if (player[0].getPos().DistanceTo(ball.GetPos()) < player[1].getPos().DistanceTo(ball.GetPos()) && player[0].getPos().DistanceTo(ball.GetPos()) < player[2].getPos().DistanceTo(ball.GetPos()))
        {
          robonr = 0;
        }

        if (player[1].getPos().DistanceTo(ball.GetPos()) < player[0].getPos().DistanceTo(ball.GetPos()) && player[1].getPos().DistanceTo(ball.GetPos()) < player[2].getPos().DistanceTo(ball.GetPos()))
        {
          robonr = 1;
        }

        if (player[2].getPos().DistanceTo(ball.GetPos()) < player[0].getPos().DistanceTo(ball.GetPos()) && player[2].getPos().DistanceTo(ball.GetPos()) < player[1].getPos().DistanceTo(ball.GetPos()))
        {
          robonr = 2;
        }

        send(Command(ACTION_BEFORE_KICK, ball.GetPos(), Position(-1.0, 0.0)), robonr);
        t_state = STEP2;
        break;

        // Kick the ball towards the Position(-1.0,0.0)
      case STEP2:
        if (!player[robonr].isBusy())
        {
          send(Command(ACTION_KICK, Position(-1.0, 0.0)), robonr);
          cout << "---------" << endl;
          t_state = STEP6;
        }

        break;
      case STEP3:
        break;
      case STEP4:
        break;
      case STEP5:
        break;
      case STEP6:
        if (!player[robonr].isBusy())
        {
          robonr = 0;
          return true;
        }
        break;


    }
    return false;
  }
  else
  {
    return true;
  }

}

/* Master::tactic_ballchasing()
    02,01,2017, Song
    This function of this tactics is to let a robot (robot2) chase the ball in a way that the end position of the robot
    tries to target the oppsite team's gate(Posotion(-1.27,0.0)).
--------------------------------------------------
    but there is a problem that if the robot reach to its end position.
    it will jump out from tactic and return to master.cpp.
    and show the menu of tactic again. And suddendly in the terminator, it shows automatically 0,
    and shows:
    Busy set to: 0
    Robo in state BEFORE_KICK
    0
    Busy set to:0

    after that program stucks there.
---------------------------------------------------
There is another problem, I am not so sure about why, but I guess that it has something to do with "before kick" function
As far as I know, 2 positions are used for the function "before kick". The first one is for positioning, the sencond
one is for turning. but if the ball moves when the robot have reached the first postion and are going to the second position
The speed of the robot will become very slow,even trumbling, because the second position is only for turning, so it do not have enough strength.
But I am not so sure.
---------------------------------------------------
Can be improved, in my opinion
    1.choose a proper end position for the robots, not only for shooting, but also pass the Ball
    2.choose a proper robot to chase the ball, not only robot2


 */
bool Master::tactic_ballchasing()
{
  send(Command(ACTION_BEFORE_KICK, ball.GetPos(), Position(-1.27, 0.0)), 2); // Position the receiving robot according to the ball
  if (ball.GetVelocity() < 0.00001 && !player[2].isBusy())       //follow the ball until the end position of robot and ball stops
  {
    return true;
  }

  return false;

}
