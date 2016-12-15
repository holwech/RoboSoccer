#include "player/player.h"

void Player::idle() {
    robo->GotoPos(robo->GetPos());
}

void Player::goTo(Position target) {
    if (robo->GetPos().DistanceTo(target) < 0.2) {
        cout << "State set to IDLE" << endl;
        done();
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
        robo->GotoPos(ball->GetPos(),2);

    }

}

void Player::before_kick(Position kick_position, Position target_of_kick)
{


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

        if(abs(delta * fabs(target_of_kick.GetY() - kick_position.GetY()) / fabs(target_of_kick.GetX() - kick_position.GetX())) > 0.1){

            delta = 0.8*delta;

            pos_before_kick.SetX(kick_position.GetX() - delta);
            if (target_of_kick.GetY() > kick_position.GetY())
            {
                pos_before_kick.SetY(kick_position.GetY() - delta * fabs(target_of_kick.GetY() - kick_position.GetY()) / fabs(target_of_kick.GetX() - kick_position.GetX()));
            }
            else
            {
                pos_before_kick.SetY(kick_position.GetY() + delta * fabs(target_of_kick.GetY() - kick_position.GetY()) / fabs(target_of_kick.GetX() - kick_position.GetX()));
            }

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

                aux_pos_before_kick.SetX(kick_position.GetX() - 0.35);
                aux_pos_before_kick.SetY(kick_position.GetY() - 0.35);

            }
            else
            {

                aux_pos_before_kick.SetX(kick_position.GetX() - 0.35);
                aux_pos_before_kick.SetY(kick_position.GetY() + 0.35);

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

        if(abs(delta * fabs(target_of_kick.GetY() - kick_position.GetY()) / fabs(target_of_kick.GetX() - kick_position.GetX())) > 0.1){

            delta = 0.8*delta;

            pos_before_kick.SetX(kick_position.GetX() + delta);

            if (target_of_kick.GetY() > kick_position.GetY())
            {
                pos_before_kick.SetY(kick_position.GetY() - delta * fabs(target_of_kick.GetY() - kick_position.GetY()) / fabs(target_of_kick.GetX() - kick_position.GetX()));
            }
            else
            {
                pos_before_kick.SetY(kick_position.GetY() + delta * fabs(target_of_kick.GetY() - kick_position.GetY()) / fabs(target_of_kick.GetX() - kick_position.GetX()));
            }
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

                aux_pos_before_kick.SetX(kick_position.GetX() + 0.35);
                aux_pos_before_kick.SetY(kick_position.GetY() - 0.35);


            }
            else
            {

                aux_pos_before_kick.SetX(kick_position.GetX() + 0.35);
                aux_pos_before_kick.SetY(kick_position.GetY() + 0.35);

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


  if(robo->GetPos().DistanceTo(pos_before_kick.GetPos()) <= 0.1)
    {

      robo->turn(target_of_kick);

      }


}
