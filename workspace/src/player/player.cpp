#include "player.h"
#include "player/goalkeeper.cpp"

Player::Player(vector<Position>* positions, RawBall* ball, Channel* channel, Robo* robo) :  positions(positions), ball(ball), channel(channel), robo(robo) {
    state = IDLE;
    ballangle = 0;
    ballx = 0;
    bally = 0;
    control = 0;
    delta = 0.09;
    aux_pos_before_kick = Position(0.0, 0.0);
    pos_before_kick = Position(0.0, 0.0);
}

void Player::run() {
   cout << "Robo thread started" << endl;
   while(1) {
       robo->driveWithCA();
       switch(state) {
       case IDLE:
           break;
       case BEFORE_PASS:
           break;
       case PASS:
           break;
       case GOTO:
           goTo();
           break;
       case BEFORE_KICK:
           before_kick(command.base, command.target);
           break;
       case KICK:
           break;
       case BLOCK_BALL:
           break;
       case DEFEND:
           defend();
           break;
       case KICK_OUT:
           break;
       case TEST:
           break;
       default:
           cout << "Case for state: " << state << endl;
           break;
       }
       readCommand();
       usleep(10);
       //cout << "State: " << state << endl;
   }
}

void Player::readCommand() {
    std::lock_guard<std::mutex> lock(mutex);
    if (channel->isRead()) {
        return;
    }
    command = channel->read();

    switch(command.action) {
    case ACTION_GOTO:
        cout << "Robo in state GOTO" << endl;
        cout << "GOTO: " << command.target.GetX() << ", " << command.target.GetY() << endl;
        setState(GOTO);
        robo->GotoPos(command.target);
        break;
    case ACTION_TEST:
        setState(TEST);
        break;
    case ACTION_IDLE:
        cout << "Robo in state IDLE" << endl;
        setState(IDLE);
        robo->GotoPos(robo->GetPos());
        break;
    case ACTION_DEFEND:
        setState(DEFEND);
        break;
    case ACTION_BEFORE_KICK:
        setState(BEFORE_KICK);
        break;
    default:
        cout << "No case for this state: " << state << endl;
        break;
    }

}


void Player::goTo() {
    if (robo->GetPos().DistanceTo(command.target) < 0.2) {
        cout << "State set to IDLE" << endl;
        setState(IDLE);
    }
}

/**
void moveSquare() {
    Position pos1 = {0.5, 0.5};
    Position pos2 = {-0.5, 0.5};
    Position pos3 = {-0.5, -0.5};
    Position pos4 = {0.5, -0.5};
    int step = 0;
    Position targetPos;
    while(1) {
        robo->updatePids(targetPos);
        robo->updatePositions();
        robo->driveWithCA();
        if(master.robo.GetPos().DistanceTo(targetPos) < 0.2){

        }
    }

}
*/

PState Player::getState() {
    return state.load();
}

PState Player::getPrevState() {
    return prevState.load();
}

void Player::setState(PState newState) {
    prevState.store(state);
    state.store(newState);
}


Player::Player(Player&& other) {
    std::lock_guard<std::mutex> lock(other.mutex);
    positions = std::move(other.positions);
    ball = std::move(other.ball);
    channel = std::move(other.channel);
    command = std::move(other.command);
    robo = std::move(other.robo);
    prevState.store(std::move(other.prevState.load()));
    state.store(std::move(state.load()));
}

Player::Player(const Player& other) {
    std::lock_guard<std::mutex> lock(other.mutex);
    positions = other.positions;
    ball = other.ball;
    channel = other.channel;
    command = other.command;
    robo = other.robo;
    prevState.store(other.prevState.load());
    state.store(state.load());
}

Player& Player::operator = (Player&& other) {
    std::lock(mutex, other.mutex);
    std::lock_guard<std::mutex> self_lock(mutex, std::adopt_lock);
    std::lock_guard<std::mutex> other_lock(other.mutex, std::adopt_lock);
    positions = std::move(other.positions);
    ball = std::move(other.ball);
    channel = std::move(other.channel);
    command = std::move(other.command);
    robo = std::move(other.robo);
    prevState.store(std::move(other.prevState.load()));
    state.store(std::move(state.load()));
    return *this;
}

Player& Player::operator = (const Player& other) {
    std::lock(mutex, other.mutex);
    std::lock_guard<std::mutex> self_lock(mutex, std::adopt_lock);
    std::lock_guard<std::mutex> other_lock(other.mutex, std::adopt_lock);
    positions = other.positions;
    ball = other.ball;
    channel = other.channel;
    command = other.command;
    robo = other.robo;
    prevState.store(other.prevState.load());
    state.store(state.load());
    return *this;
}


void Player::before_kick(Position kick_position, Position target_of_kick)
{


  // while (1){


    if (target_of_kick.GetX() > kick_position.GetX())
    {

      pos_before_kick.SetX(kick_position.GetX() - delta);

      if (target_of_kick.GetY() > kick_position.GetY())
      {
        pos_before_kick.SetY(kick_position.GetY() - delta * fabs(target_of_kick.GetY() - kick_position.GetY()) / fabs(target_of_kick.GetX() - kick_position.GetX()));
      }
      else
      {
        pos_before_kick.SetY(kick_position.GetY() + delta * fabs(target_of_kick.GetY() - kick_position.GetY()) / fabs(target_of_kick.GetX() - kick_position.GetX()));
      }


      if (kick_position.GetX() > robo->GetX())
      {

        if (robo->GetPos().DistanceTo(pos_before_kick) > 0.1)
        {
          robo->GotoPos(pos_before_kick);
          cout << "Test 1" << endl;


        }

      }
      else
      {

        if (kick_position.GetY() > 0)
        {

          aux_pos_before_kick.SetX(kick_position.GetX());
          aux_pos_before_kick.SetY(kick_position.GetY() - 0.15);

        }
        else
        {

          aux_pos_before_kick.SetX(kick_position.GetX());
          aux_pos_before_kick.SetY(kick_position.GetY() + 0.15);

        }


        if (robo->GetPos().DistanceTo(aux_pos_before_kick.GetPos()) > 0.1)
        {

          robo->GotoPos(aux_pos_before_kick);
          if (robo->GetPos().DistanceTo(aux_pos_before_kick.GetPos()) < 0.12)
          {
            control = 1;
          }

        }

        if (robo->GetPos().DistanceTo(pos_before_kick.GetPos()) > 0.1 && control == 1)
        {
          robo->GotoPos(pos_before_kick);
          cout << "Test 2" << endl;
        }

      }

    }
    else
    {


      pos_before_kick.SetX(kick_position.GetX() + delta);

      if (target_of_kick.GetY() > kick_position.GetY())
      {
        pos_before_kick.SetY(kick_position.GetY() - delta * fabs(target_of_kick.GetY() - kick_position.GetY()) / fabs(target_of_kick.GetX() - kick_position.GetX()));
      }
      else
      {
        pos_before_kick.SetY(kick_position.GetY() + delta * fabs(target_of_kick.GetY() - kick_position.GetY()) / fabs(target_of_kick.GetX() - kick_position.GetX()));
      }


      if (robo->GetX() > kick_position.GetX())
      {

        if (robo->GetPos().DistanceTo(pos_before_kick.GetPos()) > 0.1)
        {
          robo->GotoPos(pos_before_kick.GetPos());
          cout << "Test 3" << endl;
        }
      }
      else
      {


        if (kick_position.GetY() > 0)
        {

          aux_pos_before_kick.SetX(kick_position.GetX());
          aux_pos_before_kick.SetY(kick_position.GetY() - 0.15);


        }
        else
        {

          aux_pos_before_kick.SetX(kick_position.GetX());
          aux_pos_before_kick.SetY(kick_position.GetY() + 0.15);


        }


        if (robo->GetPos().DistanceTo(aux_pos_before_kick.GetPos()) > 0.1)
        {
          robo->GotoPos(aux_pos_before_kick);
          if (robo->GetPos().DistanceTo(aux_pos_before_kick.GetPos()) < 0.12)
          {
            control = 1;
          }

        }

        if (robo->GetPos().DistanceTo(pos_before_kick.GetPos()) > 0.1 && control == 1)
        {

          robo->GotoPos(pos_before_kick);
          cout << "Test 4" << endl;
        }

      }
    }
  //} //For while(1)
/*

  if (robo->GetPos().DistanceTo(pos_before_kick.GetPos()) <= 0.1) /////////
  {
  ang = robo->GetPos().AngleOfLineToPos(target_of_kick);
  cout << ang.Deg() << endl;
  if (robo->GetPos().DistanceTo(pos_before_kick.GetPos()) <= 0.20)
  {

      ang = robo->GetPos().AngleOfLineToPos(target_of_kick);
      cout << ang.Deg() << endl;
      int i = 1;

      for (i = 1; i < 10000; i++)
      {
        robo->TurnAbs(ang);

      }
  }

    while((ang.Deg()-robo->GetPhi().Deg())*(ang.Deg()-robo->GetPhi().Deg()) > 0.1)
    {
        if(fabs(robo->GetSpeedLeft()==robo->GetSpeedRight()) < 0.01)
        {
            usleep(2000);
            break;
        }
    }


    cout << "Turn complete" << endl;

    robo->MoveMs(250, 250, 500);
} ///////// */




}
