#include "master/master.h"

/** This is where all tactics should be included
 *  All tactics should return true when they're DONE
 */

void Master::t_debugContinue() {
    if (DEBUG) {
        cout << ">> Continue (y)? " << endl;
        string ans;
        cin >> ans;
    }
}

void printStrategy(string str) {
    cout << "\033[1;36m#STRATEGY: " << str << "\033[0m" << endl;
}

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
      // Positions robot accordingly
    case STEP1:
      if(ball.GetY()>0){
          chrossandpassy=-0.5;
      }
      else{
          chrossandpassy=0.5;
      }
      send(Command(ACTION_GOTO, Position(0.7, chrossandpassy), 1.5, 2.0 ), 0);
      t_state = STEP2;
      break;
      // Passes the ball to the other robot

      break;
    case STEP2:
      if (!player[1].isBusy() && !player[0].isBusy())
      {
        send(Command(ACTION_PASS, Position(player[0].getX()+0.3,chrossandpassy)), 1);
        t_state = STEP3;
     }
      break;
      // Positions the receiving robot according to the ball
    case STEP3:
      //       if (!player[2].isBusy()) {
      if (ball.GetVelocity() < 0.0001 && !player[1].isBusy() && !player[0].isBusy())  //wait for the ball stop, if not stopping, the ball.GetPos() will not updating because the state changes.
      {
        send(Command(ACTION_KICK, Position(1.4, 0.0), 2.6, 2.0), 0);
        t_state = STEP5;
      }
      break;
      // Kicks the ball towards the goal

    case STEP4:

      break;
      // When done kicking, terminate tactic
    case STEP5:
      if (!player[0].isBusy())
      {
        return true;
      }
      break;
    case STEP6:

      break;
  }
  return false;
}

bool Master::bounceForward() {
    switch(t_state2) {
    case STEP1: {
        double flipY = ball.GetPos().GetY() > 0 ? 1.0 : -1.0;
        Position target(0.8 * -side, 0.3 * flipY);
        double midpointXDist = (ball.GetPos().GetX() - target.GetX()) / 2;
        double midpointX = target.GetX() + midpointXDist;
        Position targetWall(midpointX, 0.89 * flipY);
        send(Command(ACTION_KICK, targetWall, 2.0, 1.5), 1);
        send(Command(ACTION_GOTO, Position(midpointX, ball.GetPos().GetY()), 2.0), 2);
        t_state2 = STEP2;
        break;
    }
    case STEP2:
        if (!player[1].isBusy() && !player[2].isBusy()) {
            send(Command(ACTION_GOTO, Position(0.8 * side, 0.0), 2.0), 1);
            t_state2 = STEP3;
        }
        break;
    case STEP3:
        if (!player[1].isBusy() && !player[2].isBusy()) {
            t_state2 = STEP4;
        }
        break;
    case STEP4:
        return kickAtGoal();
        break;
    default:
        cout << "No case for this state in bounceForward" << endl;
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
bool Master::tactic_nearpenaltyarea(double threshold, int playerNum)
{
  // if the ball is too close to our gate/ penalty area
  if (ball.GetX() > threshold)
  {


    // Position robots accordingly
    switch (t_state)
    {
      case STEP1: {
        if (playerNum == -1) {
            closestRobo = getClosest();
        } else {
            closestRobo = playerNum;
        }
        t_state = STEP2;
        break;
      }
      case STEP2:
        send(Command(ACTION_KICK, Position(-1.0, ball.GetPos().GetY()), 2.5, 2.0), closestRobo);
        t_state = STEP3;
        break;
      case STEP3:
        if (!player[closestRobo].isBusy())
        {
          return true;
        }
        if (playerNum == -1) {
            checkClosest(closestRobo);
        }
        break;
      default:
        cout << "No case for this step in tactic nearpenaltyarea" << t_state << endl;
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

/**
 *	This tactic finds the closest robot to the ball and kicks the ball
 * 	towards the goal without hitting the goalkeeper.
 * 	Provide a player number to do the goal kick with a spesific player.
 * 	If a number is not provided, it will choose the closest one.
 */
bool Master::kickAtGoal(int playerNum, bool is_penalty) {
    switch(t_state) {
    // Find closest robo to ball
    case STEP1: {
        if (playerNum == -1) {
            closestRobo = getClosest();
        } else {
            closestRobo = playerNum;
        }
        //cout << "Closest robo is: " << closestRobo << endl;
        masterPrint("kickAtGoal: STEP1 DONE");
        t_state = STEP2;
        break;
    }
    case STEP2: {
        if (playerNum == -1) {
            cout << "Closest robo in STEP3 is: " << closestRobo << endl;
        }
        t_target = Position(1.27 * -side, 0);
        if (otherKeeperInGoalArea() == 1) {
            Position keeperPos = getOtherKeeperPos();
            double modifier = 0;
            if (keeperPos.GetY() > 0) {
                modifier = -0.1;
            } else {
                modifier = 0.1;
            }
            t_target.SetY(keeperPos.GetY() + modifier);
        }
        if(is_penalty){
            send(Command(ACTION_KICK, t_target, 2.5, 1.4), closestRobo);
        }else{
            send(Command(ACTION_KICK, t_target, 2.5, 2.0), closestRobo);
        }
        masterPrint("kickAtGoal: STEP2 DONE");
        t_state = STEP3;
        break;
    }
    case STEP3:
        if (!player[closestRobo].isBusy()) {
            masterPrint("kickAtGoal: DONE");
            return true;
        }
        if (playerNum == -1) {
            checkClosest(closestRobo);
        }
        break;
    default:
        cout << "No case in kickAtGoal" << endl;
        break;
    }
    return false;
}

bool Master::throughPass() {
    switch(t_state2) {
    case STEP1:{
        closestRobo = getClosestToTeamGoal();
        notClosestRobo = getNotClosestToTeamGoal();
        Position receivingPos(0.8 * -side, 0.6);
        t_target = Position(0.8 * -side, 0.4);
        if (ball.GetPos().GetY() < 0.0) {
            receivingPos.SetY(-0.6);
            t_target.SetY(-0.4);
        }
        send(Command(ACTION_GOTO, receivingPos, 2.5, true), notClosestRobo);
        send(Command(ACTION_KICK, t_target, 2.5, 2.0), closestRobo);
        printStrategy("throughPass: STEP1 DONE");
        t_state2 = STEP2;
        break;
    }
    case STEP2:
        if (!player[closestRobo].isBusy()) {
            printStrategy("throughPass: STEP2 DONE");
            t_state2 = STEP3;
        }
        break;
    case STEP3: {
        bool kickDone = kickAtGoal(notClosestRobo);
        if (kickDone) {
            printStrategy("throughPass: DONE");
            return true;
        }
        break;
    }
    default:
        cout << "No case in throughPass" << endl;
        break;
    }
    return false;
}
