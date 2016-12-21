#include "player/player.h"

void Player::idle() {
    robo.idle();
}

void Player::goTo(Position target) {
    if (robo.GetPos().DistanceTo(target) < 0.05) {
        cout << "State set to IDLE" << endl;
        done();
    } else {
        robo.GotoPos(target);
    }
}

void Player::kick(Position target){

    double dirx,diry,length;

    dirx= (target.GetX()-ball.GetX());
    diry= (target.GetY()-ball.GetY());
    length= sqrt((dirx*dirx)+(diry*diry));
    dirx=dirx/length;
    diry=diry/length;
    cout << "dirx: "<< dirx << endl;
    cout << "diry: "<< diry << endl;
    cout << "length: "<< length << endl;

    Position pos(ball.GetX()+(dirx/100), ball.GetY()+(diry/100));

    if (robo.GetPos().DistanceTo(pos) > 0.1) {
        robo.turn(pos);
        robo.GotoPos(pos,1.9);
    }
    else{
        done();
    }


}
void Player::drivingKick(Position target){
    Position ballPos = ball.GetPos();
    double vecX = target.GetX() - ballPos.GetX();
    double vecY = target.GetY() - ballPos.GetY();
    vecX /= (vecX+vecY)*5; //Scaling to get equal distance every time
    vecY /= (vecX+vecY)*5;
    Position prePos1 = ballPos;
    Position prePos2 = ballPos;
    Position postPos = ballPos;
    prePos1 += Position(2*vecX, 2*vecY);
    prePos2 += Position(vecX, vecY);
    postPos -= Position(vecX, vecY);
    Position wantedPos;
    if(phase == 0){
        wantedPos = prePos1;
    }else if(phase == 1){
        wantedPos = prePos2;
    }else if(phase == 2){
        wantedPos = postPos;
    }
    cout << "Count before init: " << phase << endl;
    if(robo.GetPos().DistanceTo(wantedPos) < 0.2){
        if(phase == 0){
            phase = 1;
        }
        else if(phase == 1){
            phase = 2;
        }
        else if(phase == 2)
            done();
            phase = 0;
    }
    if (phase >= 1){
        robo.GotoPos(wantedPos, phase);
    }else{
        robo.GotoPos(wantedPos);
    }
    cout << "Count after function: " << phase << endl;

}

bool Player::before_kick(Position kick_position, Position target_of_kick)
{
    delta = 0.09;

  if (target_of_kick.GetX() > kick_position.GetX())
    {
        ////// Create the position for the robot to go to behind the ball
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



        if (kick_position.GetX() > robo.GetX())
        {

            if (robo.GetPos().DistanceTo(pos_before_kick) > 0.1)
            {
                robo.GotoPos(pos_before_kick,1);
                cout << "Test 1" << endl;
                cout << pos_before_kick << endl;


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


            if (robo.GetPos().DistanceTo(aux_pos_before_kick.GetPos()) > 0.1)
            {

                robo.GotoPos(aux_pos_before_kick,1.5);
                if (robo.GetPos().DistanceTo(aux_pos_before_kick.GetPos()) < 0.12)
                {
                    control = 1;
                }

            }

            if (robo.GetPos().DistanceTo(pos_before_kick.GetPos()) > 0.1 && control == 1)
            {
                robo.GotoPos(pos_before_kick,1.5);
                cout << "Test 2" << endl;
            }

        }

    }
    else
    {
        ////// Create the position for the robot to go to behind the ball
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



        if (robo.GetX() > kick_position.GetX())
        {

            if (robo.GetPos().DistanceTo(pos_before_kick.GetPos()) > 0.1)
            {
                robo.GotoPos(pos_before_kick.GetPos(),1.5);
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
            if (robo.GetPos().DistanceTo(aux_pos_before_kick.GetPos()) > 0.1)
            {
                robo.GotoPos(aux_pos_before_kick,1.5);
                if (robo.GetPos().DistanceTo(aux_pos_before_kick.GetPos()) < 0.12)
                {
                    control = 1;
                }

            }

            if (robo.GetPos().DistanceTo(pos_before_kick.GetPos()) > 0.1 && control == 1)
            {

                robo.GotoPos(pos_before_kick,1.5);
                cout << "Test 4" << endl;
            }

        }
       //return false;
    }


  if(robo.GetPos().DistanceTo(pos_before_kick.GetPos()) <= 0.1)
    {
      counter++;
      robo.turn(target_of_kick);
      if (counter > 100){
          counter = 0;
          return true;
      }
    }

    return false;
}
