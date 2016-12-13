#include "player/player.h"

void Player::idle() {
    robo->GotoPos(robo->GetPos());
}

void Player::goTo(Position target) {
    if (robo->GetPos().DistanceTo(target) < 0.2) {
        cout << "State set to IDLE" << endl;
        setState(IDLE);
    } else {
        robo->GotoPos(target);
    }
}

void Player::kick(int power){ //power 0 -> 100
    double distToBall = robo->GetPos().DistanceTo(ball->GetPos());
    if(distToBall < 0.12){
        robo->GotoPos(ball->GetPos());
    }
    else{
        robo->GotoPos(ball->GetPos(), 2);
    }
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
