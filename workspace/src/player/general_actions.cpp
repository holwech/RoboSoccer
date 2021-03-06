#include "player/player.h"
#include "ball/ball.h"
void Player::idle()
{
  if (deviceNr == 4) {
      //cout << "idle" << endl;
  }
  robo.idle();
}

bool Player::stop() {
  return true;
}

bool Player::goTo(Position target, double speed, bool ca)
{
  robo.GotoPos(target, speed, ca);
  if (deviceNr == 4) {
      //cout << "goto " << target << endl;
  }
  if (robo.isArrived(0.04))
  {
    return true; // Done
  }
  return false;
}

bool Player::pass(Position target)
{
  double distance = 0;
  double speedparam = 0;
  switch (pass_state)
  {
    case A_STEP1:
      // Calculates Speed according to distance
      distance = ball.GetPos().DistanceTo(target);
      speedparam = distance;
      passSpeed = speedparam * 2.0;
      pass_state = A_STEP2;
      break;
    case A_STEP2:
      {
        bool passDone = kick(target, passSpeed, 2.0);
        if (passDone)
        {
          return true;
        }
        break;
      }
    default:
      cout << "No case for this step in action pass function" << endl;
      break;
  }

  return false;
}

bool Player::kick(Position target, double speed, double approach_speed)
{
  bool beforeKickDone = false;
  switch (kick_state)
  {
      // Go to ball and prepare to kick
    case A_STEP1:
      beforeKickDone = before_kick_improved(ball.GetPos(), target, approach_speed);
      if (beforeKickDone)
      {
        cout << "before kick finished" << endl;
        playerPrint("KICK: STEP1 DONE, before kick finished");
        kick_state = A_STEP2;
      }
      break;
      // Calculate which direction to kick
    case A_STEP2:
      double dirx, diry, length;
      dirx = (target.GetX() - ball.GetX());
      diry = (target.GetY() - ball.GetY());
      length = sqrt((dirx * dirx) + (diry * diry));
      dirx = dirx / length;
      diry = diry / length;
      endKickPos.SetX(ball.GetX() + (dirx / 5));
      endKickPos.SetY(ball.GetY() + (diry / 5));
      cout << "Position: " << endKickPos.GetX() << ", " << endKickPos.GetY() << endl;
      playerPrint("KICK: STEP2 DONE");
      kick_state = A_STEP3;
      break;
      // Do the kick
    case A_STEP3:
      {
        bool kickDone = goTo(endKickPos, speed);
        if (kickDone)
        {
            playerPrint("KICK: KICK DONE");
            return true;
        }
        break;
      }
    default:
      cout << "No case for this step in action kick" << endl;
      break;
  }
  return false;
}

void Player::drivingKick(Position target)
{
  Position ballPos = ball.GetPos();
  double vecX = target.GetX() - ballPos.GetX();
  double vecY = target.GetY() - ballPos.GetY();
  vecX /= (vecX + vecY) * 5; //Scaling to get equal distance every time
  vecY /= (vecX + vecY) * 5;
  Position prePos1 = ballPos;
  Position prePos2 = ballPos;
  Position postPos = ballPos;
  prePos1 += Position(2 * vecX, 2 * vecY);
  prePos2 += Position(vecX, vecY);
  postPos -= Position(vecX, vecY);
  Position wantedPos;
  if (phase == 0)
  {
    wantedPos = prePos1;
  }
  else if (phase == 1)
  {
    wantedPos = prePos2;
  }
  else if (phase == 2)
  {
    wantedPos = postPos;
  }
  cout << "Count before init: " << phase << endl;
  if (robo.GetPos().DistanceTo(wantedPos) < 0.2)
  {
    if (phase == 0)
    {
      phase = 1;
    }
    else if (phase == 1)
    {
      phase = 2;
    }
    else if (phase == 2)
      done();
    phase = 0;
  }
  if (phase >= 1)
  {
    robo.GotoPos(wantedPos, phase);
  }
  else
  {
    robo.GotoPos(wantedPos);
  }
  cout << "Count after function: " << phase << endl;

}


bool Player::before_kick_improved(Position kick_position, Position target_of_kick, double before_kick_speed){

    robo.setPrecise(false);
    robo.setAvoidBall(true);

    edges edge = ball.nearEdge();
    bool done = false;
    switch(edge){
    case E_NONE:
    case E_BOTTOM:
    case E_BOTTOM_LEFT:
    case E_BOTTOM_RIGHT:
    case E_LEFT:
    case E_RIGHT:
    case E_TOP:
    case E_TOP_LEFT:
    case E_TOP_RIGHT:
        done = angeled_behind_ball(target_of_kick, before_kick_speed);
        break;
    default:
        cout << "No case for this edge in before_kick_improved" << endl;
        done = angeled_behind_ball(target_of_kick, before_kick_speed);
        break;
    }
    if (done) {
        robo.setAvoidBall(false);
        robo.setPrecise(false);
    }
    return done;
}

bool Player::angeled_behind_ball(Position targetPos, double speed){
    Position ballPos = ball.RawBall::GetPos();
    double pos_behind_ball_x;
    double pos_behind_ball_y;
    Position direction = Position(ballPos.GetX() - targetPos.GetX(), ballPos.GetY() - targetPos.GetY() );
    double scale = 0.1;
    double length = direction.DistanceTo(Position(0,0));
    Position pos_behind_ball;

    switch (state_before_kick){
    case STEP1:{
        /*double speedModifier = 0.5;
        if (!ball.isStopped()) {
            speedModifier = 0.7;
        }*/
        pos_behind_ball_x = ballPos.GetX() + direction.GetX() * 3 * scale / length;
        pos_behind_ball_y = ballPos.GetY() + direction.GetY() * 3 * scale / length;
        pos_behind_ball = Position(pos_behind_ball_x, pos_behind_ball_y);
        // Reduce speed close to turning point
        robo.GotoPos(pos_behind_ball, speed);
        /*
        if (robo.isArrived(0.2)) {
            robo.GotoPos(pos_behind_ball, speed * 0.2);
        } else {
            robo.GotoPos(pos_behind_ball, speed);
        }
        */
        // Arrived at turning point
        if(robo.isArrived(0.1)){
            playerPrint("BEFORE_KICK: STEP1 Done");
            state_before_kick = STEP2;
            lengthToBall = robo.GetPos().DistanceTo(ball.RawBall::GetPos());
        }
        break;
    }
    case STEP2:
        //if the ball has moved far away, go back to step1
        if ( lengthToBall +0.05 < robo.GetPos().DistanceTo(ball.RawBall::GetPos()) ){
            state_before_kick = STEP1;
            playerPrint("BEFORE_KICK: Ball too far away, going to STEP1");
        }

        pos_behind_ball_x = ballPos.GetX() + direction.GetX() * scale / length;
        pos_behind_ball_y = ballPos.GetY() + direction.GetY() * scale / length;
        pos_behind_ball = Position(pos_behind_ball_x, pos_behind_ball_y);
        robo.GotoPos(pos_behind_ball, speed);
        if (robo.isArrived(0.1)) {
            //cout << endl << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl << endl;
            playerPrint("BEFORE_KICK: Done");
            return true;
        }
        break;
    default:
        cout << "No case for this state in angeled_behind_ball" << endl;
        break;
    }
    //cout << "In angeled_behind_ball, pos: " << pos_behind_ball << endl;
    return false;
}


bool Player::before_kick(Position kick_position, Position target_of_kick, double before_kick_speed)
{
  delta = 0.2; // Before 0.2
  Position pos_before_kick_far(0.0, 0.0);
  Position pos_before_kick_near(0.0, 0.0);
  Position aux_pos_before_kick(0.0, 0.0);
  // Default before_kick_speed is 0.4

  // This function takes into consideration all cases: robot between target and ball, robot has to change the side relative to the ball to shoot, etc.
  // This explains the complex structure of the code.
  // The slope of the line between kick_position and target_of_kick is calculated and is used to determine where to position behind the ball before kick


  // 1. Adjust delta according to angle

  if (fabs(target_of_kick.AngleOfLineToPos(kick_position).Deg()) < 120 && fabs(target_of_kick.AngleOfLineToPos(kick_position).Deg()) > 60)
  {
    delta = 0.00015 * (fabs(target_of_kick.AngleOfLineToPos(kick_position).Deg()) - 90) * (fabs(target_of_kick.AngleOfLineToPos(kick_position).Deg()) - 90) + pow(10, -100000000);
  }


  if (target_of_kick.GetX() > kick_position.GetX()) //1st major part: If target_of_kick.GetX() > kick_position.GetX()
  {

    ////// Create the position for the robot to go to behind the ball: near and far
    pos_before_kick_far.SetX(kick_position.GetX() - delta);
    if (target_of_kick.GetY() > kick_position.GetY())
    {
      pos_before_kick_far.SetY(kick_position.GetY() - delta * fabs(target_of_kick.GetY() - kick_position.GetY()) / fabs(target_of_kick.GetX() - kick_position.GetX()));
    }
    else
    {
      pos_before_kick_far.SetY(kick_position.GetY() + delta * fabs(target_of_kick.GetY() - kick_position.GetY()) / fabs(target_of_kick.GetX() - kick_position.GetX()));
    }

    pos_before_kick_near.SetX(kick_position.GetX() - 0.3 * delta);
    if (target_of_kick.GetY() > kick_position.GetY())
    {
      pos_before_kick_near.SetY(kick_position.GetY() - 0.3 * delta * fabs(target_of_kick.GetY() - kick_position.GetY()) / fabs(target_of_kick.GetX() - kick_position.GetX()));
    }
    else
    {
      pos_before_kick_near.SetY(kick_position.GetY() + 0.3 * delta * fabs(target_of_kick.GetY() - kick_position.GetY()) / fabs(target_of_kick.GetX() - kick_position.GetX()));
    }

    if(fabs(pos_before_kick_near.GetX()) > 1.2 || fabs(pos_before_kick_near.GetY()) > 0.70 || fabs(pos_before_kick_far.GetX()) > 1.2 || fabs(pos_before_kick_far.GetY()) > 0.70)
    { /////// Deal with exceptions: shooting position outside of field -> Just drive towards ball

        if (robo.GetPos().DistanceTo(ball.GetPos()) > 0.05)
        {
          robo.GotoPos(ball.GetPos(), 2.2);
          //cout << "Testfall" << endl;
          //cout << ball.GetX() << endl;
          //cout << ball.GetY() << endl;
        }

        if (robo.GetPos().DistanceTo(ball.GetPos()) <= 0.05)
        {
          counter = 0;
          control = 0;
          return true;
          //done();
        }

    }else{ //// Case: Position is inside the field

    if (kick_position.GetX() > robo.GetX()) // Easy case: robot is already behind ball
    {

      if (robo.GetPos().DistanceTo(pos_before_kick_near) > 0.04)
      {
        if (control == 0)
        {
          robo.GotoPos(pos_before_kick_far, 1.3);
        }
        if (robo.GetPos().DistanceTo(pos_before_kick_far) < 0.15) // Drive to position far away from ball
        {
          control = 1;
        }

        if (robo.GetPos().DistanceTo(pos_before_kick_near.GetPos()) > 0.05 && control == 1) // Drive to position near ball
        {
          robo.GotoPos(pos_before_kick_near, before_kick_speed);

        }

        if (robo.GetPos().DistanceTo(pos_before_kick_near.GetPos()) <= 0.05) // Condition to end function
        {
          counter = 0;
          control = 0;
          return true;
          //done();
        }

      }

    }
    else // Robot has to get behind ball
    {

      if (kick_position.GetY() > 0) // Create auxiliary point on trajectory to get behind ball without touching it
      {

        aux_pos_before_kick.SetX(kick_position.GetX() - 0.2);
        aux_pos_before_kick.SetY(kick_position.GetY() - 0.35);

      }
      else
      {

        aux_pos_before_kick.SetX(kick_position.GetX() - 0.2);
        aux_pos_before_kick.SetY(kick_position.GetY() + 0.35);

      }


      if (robo.GetPos().DistanceTo(aux_pos_before_kick.GetPos()) > 0.1) //Start approchaing the ball as seen above if already behind
      {

        robo.GotoPos(aux_pos_before_kick, 1.5);
        if (robo.GetPos().DistanceTo(aux_pos_before_kick.GetPos()) < 0.12)
        {
          control = 1;
        }

      }

      if (robo.GetPos().DistanceTo(pos_before_kick_near) > 0.1 && control > 0)
      {
        if (control == 1)
        {
          robo.GotoPos(pos_before_kick_far, 1.3);
        }
        if (robo.GetPos().DistanceTo(pos_before_kick_far) < 0.15)
        {
          control = 2;
        }

        if (robo.GetPos().DistanceTo(pos_before_kick_near.GetPos()) > 0.05 && control == 2)
        {
          robo.GotoPos(pos_before_kick_near, before_kick_speed);
          if (robo.GetPos().DistanceTo(pos_before_kick_near.GetPos()) <= 0.05) // Condition to end function
          {
            counter = 0;
            control = 0;
            return true;
            //done();
          }
        }
      }
    }
   }
  }
  else //2nd major part. It is nearly mirrored version of the 1st major part. If target_of_kick.GetX() < kick_position.GetX()
  {

    ///// Create the position for the robot to go to behind the ball: near and far
    pos_before_kick_far.SetX(kick_position.GetX() + delta);
    if (target_of_kick.GetY() > kick_position.GetY())
    {
      pos_before_kick_far.SetY(kick_position.GetY() - delta * fabs(target_of_kick.GetY() - kick_position.GetY()) / fabs(target_of_kick.GetX() - kick_position.GetX()));
    }
    else
    {
      pos_before_kick_far.SetY(kick_position.GetY() + delta * fabs(target_of_kick.GetY() - kick_position.GetY()) / fabs(target_of_kick.GetX() - kick_position.GetX()));
    }

    pos_before_kick_near.SetX(kick_position.GetX() + 0.3 * delta);
    if (target_of_kick.GetY() > kick_position.GetY())
    {
      pos_before_kick_near.SetY(kick_position.GetY() - 0.3 * delta * fabs(target_of_kick.GetY() - kick_position.GetY()) / fabs(target_of_kick.GetX() - kick_position.GetX()));
    }
    else
    {
      pos_before_kick_near.SetY(kick_position.GetY() + 0.3 * delta * fabs(target_of_kick.GetY() - kick_position.GetY()) / fabs(target_of_kick.GetX() - kick_position.GetX()));
    }


    if(fabs(pos_before_kick_near.GetX()) > 1.2 || fabs(pos_before_kick_near.GetY()) > 0.70 || fabs(pos_before_kick_far.GetX()) > 1.2 || fabs(pos_before_kick_far.GetY()) > 0.70)
    {/////// Deal with exceptions: shooting position outside of field

        if (robo.GetPos().DistanceTo(ball.GetPos()) > 0.05)
        {
            robo.GotoPos(ball.GetPos(), 2.2);
            //cout << "Testfall" << endl;
            //cout << ball.GetX() << endl;
            //cout << ball.GetY() << endl;
        }

        if (robo.GetPos().DistanceTo(ball.GetPos()) <= 0.05) // Condition to end function
        {
          counter = 0;
          control = 0;
          return true;
          //done();
        }

    }else{

    if (robo.GetX() > kick_position.GetX())// Easy case: robot is already behind ball
    {



      if (robo.GetPos().DistanceTo(pos_before_kick_near) > 0.04) // Drive to position far away from ball
      {
        if (control == 0)
        {
          robo.GotoPos(pos_before_kick_far, 1.3);
        }

        if (robo.GetPos().DistanceTo(pos_before_kick_far) < 0.15) // Drive to position near ball
        {
          control = 1;
        }

        if (robo.GetPos().DistanceTo(pos_before_kick_near.GetPos()) > 0.05 && control == 1)
        {
          robo.GotoPos(pos_before_kick_near, before_kick_speed);

        }

        if (robo.GetPos().DistanceTo(pos_before_kick_near.GetPos()) <= 0.05) // Condition to end function
        {
          counter = 0;
          control = 0;
          return true;
          //done();
        }

      }

    }
    else // Robot has to get behind ball
    {


      if (kick_position.GetY() > 0) // Create auxiliary point on trajectory to get behind ball without touching it
      {

        aux_pos_before_kick.SetX(kick_position.GetX() + 0.2);
        aux_pos_before_kick.SetY(kick_position.GetY() - 0.35);


      }
      else
      {

        aux_pos_before_kick.SetX(kick_position.GetX() + 0.2);
        aux_pos_before_kick.SetY(kick_position.GetY() + 0.35);

      }

      if (robo.GetPos().DistanceTo(aux_pos_before_kick.GetPos()) > 0.1) //Start approchaing the ball as seen above if already behind
      {
        robo.GotoPos(aux_pos_before_kick, 1.5);
        if (robo.GetPos().DistanceTo(aux_pos_before_kick.GetPos()) < 0.12)
        {
          control = 1;
        }

      }

      if (robo.GetPos().DistanceTo(pos_before_kick_near) > 0.1 && control > 0)
      {
        if (control == 1)
        {
          robo.GotoPos(pos_before_kick_far, 1.3);
        }
        //cout << control << endl;
        if (robo.GetPos().DistanceTo(pos_before_kick_far) < 0.15)
        {
          control = 2;
        }

        if (robo.GetPos().DistanceTo(pos_before_kick_near.GetPos()) > 0.05 && control == 2)
        {
          robo.GotoPos(pos_before_kick_near, before_kick_speed);
          if (robo.GetPos().DistanceTo(pos_before_kick_near.GetPos()) <= 0.05) // Condition to end function
          {
            counter = 0;
            control = 0;
            return true;
            //done();
          }
        }
      }
    }
  }
}
  return false;
}


bool Player::old_pass(Position target)
{

  double dirx, diry, length;

  dirx = (target.GetX() - ball.GetX());
  diry = (target.GetY() - ball.GetY());
  length = sqrt((dirx * dirx) + (diry * diry));
  dirx = dirx / length;
  diry = diry / length;

  Position pos(ball.GetX() + (dirx / 80), ball.GetY() + (diry / 80));

  if (robo.GetPos().DistanceTo(pos) > 0.1)
  {
    robo.turn(pos);
    if (ball.GetPos().DistanceTo(target) >= 0.7)
    {
      cout << "long distance: " << endl;
      robo.GotoPos(pos, 2.5);
    }
    if (ball.GetPos().DistanceTo(target) >= 0.5 && ball.GetPos().DistanceTo(target) < 0.7)
    {
      cout << "medium distance: " << endl;
      robo.GotoPos(pos, 2.0);
    }
    else
    {
      cout << "short distance: " << endl;
      robo.GotoPos(pos, 1.1);
    }


  }
  else //if (robo.isArrived())
  {
    return true; // Done
  }
  return false;

}

bool Player::old_kick(Position target)
{

  double dirx, diry, length;

  dirx = (target.GetX() - ball.GetX());
  diry = (target.GetY() - ball.GetY());
  length = sqrt((dirx * dirx) + (diry * diry));
  dirx = dirx / length;
  diry = diry / length;
  cout << "dirx: " << dirx << endl;
  cout << "diry: " << diry << endl;
  cout << "length: " << length << endl;

  Position pos(ball.GetX() + (dirx / 80), ball.GetY() + (diry / 80));

  if (robo.GetPos().DistanceTo(pos) > 0.1)
  {
    robo.turn(pos);
    robo.GotoPos(pos, 2.8);
    cout << "--------" << pos << endl;
    cout << "--------" << target << endl;
  }
  else
  {
    return true; // Done
  }

  return false;
}


bool Player::block_ball(double speed) {
    robo.setAvoidBall(true);
    switch(block_state) {
    case A_STEP1: {
        double xLine = ball.GetPos().GetX() + 0.3 * side;
        if (fabs(xLine) > 1.2) {
            xLine = 1.2 * side;
        }
        block_target = ball.predictInY(xLine);
        if(goTo(block_target, speed)) {
            block_state = A_STEP2;
        }
        break;
    }
    case A_STEP2:
        if (fabs(ball.GetPos().GetX()) > fabs(robo.GetPos().GetX())) {
            block_state = A_STEP1;
        } else {
            robo.setAvoidBall(false);
            return true;
        }
        break;
    default:
        cout << "No case for this state in block_ball in player" << endl;
        break;
    }
    robo.setAvoidBall(false);
    return false;
}
