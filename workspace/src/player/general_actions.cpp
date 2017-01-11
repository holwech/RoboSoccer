#include "player/player.h"

void Player::idle() {
    robo.idle();
}

bool Player::goTo(Position target, double speed) {
    if (robo.GetPos().DistanceTo(target) < 0.05) {
        cout << "State set to IDLE" << endl;
        return true; // Done
    } else {
        robo.GotoPos(target, speed);
    }
    return false;
}

bool Player::pass(Position target) {
    double distance=0;
    double speedparam=0;
    switch(pass_state) {
    case A_STEP1:
        // Calculates Speed according to distance
        distance = ball.GetPos().DistanceTo(target);
        speedparam = distance/2;
        passSpeed = speedparam*2.6;
        pass_state = A_STEP2;
        break;
    case A_STEP2: {
        cout << "Pass" << endl;
        bool passDone = kick(target, passSpeed);
        if (passDone) {
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

bool Player::kick(Position target, double speed) {
    bool beforeKickDone = false;
    switch(kick_state) {
    // Go to ball and prepare to kick
    case A_STEP1:
        beforeKickDone = before_kick(ball.GetPos(), target);
        if (beforeKickDone) {
            kick_state = A_STEP2;
        }
        break;
    // Calculate which direction to kick
    case A_STEP2:
        double dirx,diry,length;
        dirx= (target.GetX()-ball.GetX());
        diry= (target.GetY()-ball.GetY());
        length= sqrt((dirx*dirx)+(diry*diry));
        dirx=dirx/length;
        diry=diry/length;
        endKickPos.SetX(ball.GetX()+(dirx / 5));
        endKickPos.SetY(ball.GetY()+(diry / 5));
        cout << "Position: " << endKickPos.GetX() << ", " << endKickPos.GetY() << endl;
        kick_state = A_STEP3;
        break;
    // Do the kick
    case A_STEP3: {
        bool kickDone = goTo(endKickPos, speed);
        if (kickDone) {
            kick_state = A_STEP4;
        }
        break;
    }
    case A_STEP4:
        return true;
    // Kick done
    default:
        cout << "No case for this step in action kick" << endl;
        break;
    }
    return false;
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
    delta = 0.2; // Before 0.2
    Position pos_before_kick_far(0.0,0.0);
    Position pos_before_kick_near(0.0,0.0);
    Position aux_pos_before_kick(0.0,0.0);

    // cout << target_of_kick.AngleOfLineToPos(kick_position).Deg() << endl;

   if(fabs(target_of_kick.AngleOfLineToPos(kick_position).Deg()) < 120 && fabs(target_of_kick.AngleOfLineToPos(kick_position).Deg()) > 60){


       delta = 0.0002*(fabs(target_of_kick.AngleOfLineToPos(kick_position).Deg()) - 90) * (fabs(target_of_kick.AngleOfLineToPos(kick_position).Deg()) - 90) + pow(10,-16);
        // cout << delta << endl;
    }


  if (target_of_kick.GetX() > kick_position.GetX())
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

        pos_before_kick_near.SetX(kick_position.GetX() - 0.3*delta);
        if (target_of_kick.GetY() > kick_position.GetY())
        {
            pos_before_kick_near.SetY(kick_position.GetY() - 0.3*delta * fabs(target_of_kick.GetY() - kick_position.GetY()) / fabs(target_of_kick.GetX() - kick_position.GetX()));
        }
        else
        {
            pos_before_kick_near.SetY(kick_position.GetY() + 0.3*delta * fabs(target_of_kick.GetY() - kick_position.GetY()) / fabs(target_of_kick.GetX() - kick_position.GetX()));
        }

       /* if(abs(delta * fabs(target_of_kick.GetY() - kick_position.GetY()) / fabs(target_of_kick.GetX() - kick_position.GetX())) > 0.1){

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

        } */

        if (kick_position.GetX() > robo.GetX())
        {

           if (robo.GetPos().DistanceTo(pos_before_kick_near) > 0.04)
            {                    
                if (control == 0)
                {
                   robo.GotoPos(pos_before_kick_far,1.3);
                }
                if (robo.GetPos().DistanceTo(pos_before_kick_far) < 0.15){
                    control = 1;
                }

                if (robo.GetPos().DistanceTo(pos_before_kick_near.GetPos()) > 0.05 && control == 1)
                {
                   robo.GotoPos(pos_before_kick_near,0.4);

                }

                if(robo.GetPos().DistanceTo(pos_before_kick_near.GetPos()) <= 0.05){
                    counter = 0;
                    control = 0;
                    return true;
                    //done();
                }

            }

        }
        else{

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

            if (robo.GetPos().DistanceTo(pos_before_kick_near) > 0.1 && control > 0)
            {
                     if (control == 1)
                     {
                        robo.GotoPos(pos_before_kick_far,1.3);
                     }
                     cout << control << endl;
                     if (robo.GetPos().DistanceTo(pos_before_kick_far) < 0.15){
                         control = 2;
                     }

                     if (robo.GetPos().DistanceTo(pos_before_kick_near.GetPos()) > 0.05 && control == 2)
                     {
                        robo.GotoPos(pos_before_kick_near,0.4);
                        if(robo.GetPos().DistanceTo(pos_before_kick_near.GetPos()) <= 0.05){
                            counter = 0;
                            control = 0;
                            return true;
                            //done();
                        }
                     }

              }

        }

    }
    else
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

              pos_before_kick_near.SetX(kick_position.GetX() + 0.3*delta);
              if (target_of_kick.GetY() > kick_position.GetY())
              {
                  pos_before_kick_near.SetY(kick_position.GetY() - 0.3*delta * fabs(target_of_kick.GetY() - kick_position.GetY()) / fabs(target_of_kick.GetX() - kick_position.GetX()));
              }
              else
              {
                  pos_before_kick_near.SetY(kick_position.GetY() + 0.3*delta * fabs(target_of_kick.GetY() - kick_position.GetY()) / fabs(target_of_kick.GetX() - kick_position.GetX()));
              }



           /* if(abs(delta * fabs(target_of_kick.GetY() - kick_position.GetY()) / fabs(target_of_kick.GetX() - kick_position.GetX())) > 0.1){
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
        } */



        if (robo.GetX() > kick_position.GetX())
        {



            if (robo.GetPos().DistanceTo(pos_before_kick_near) > 0.04)
             {
                 if (control == 0)
                 {
                    robo.GotoPos(pos_before_kick_far,1.3);
                 }
                 cout << control << endl;
                 if (robo.GetPos().DistanceTo(pos_before_kick_far) < 0.15){
                     control = 1;
                 }

                 if (robo.GetPos().DistanceTo(pos_before_kick_near.GetPos()) > 0.05 && control == 1)
                 {
                    robo.GotoPos(pos_before_kick_near,0.4);

                 }

                 if(robo.GetPos().DistanceTo(pos_before_kick_near.GetPos()) <= 0.05){
                     counter = 0;
                     control = 0;
                     return true;
                     //done();
                 }

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

            if (robo.GetPos().DistanceTo(pos_before_kick_near) > 0.1 && control > 0)
            {
                     if (control == 1)
                     {
                        robo.GotoPos(pos_before_kick_far,1.3);
                     }
                     cout << control << endl;
                     if (robo.GetPos().DistanceTo(pos_before_kick_far) < 0.15){
                         control = 2;
                     }

                     if (robo.GetPos().DistanceTo(pos_before_kick_near.GetPos()) > 0.05 && control == 2)
                     {
                        robo.GotoPos(pos_before_kick_near,0.4);
                        if(robo.GetPos().DistanceTo(pos_before_kick_near.GetPos()) <= 0.05){
                            counter = 0;
                            control = 0;
                            return true;
                            //done();
                        }
                     }

             }
        }
     }


    return false;    
}

bool Player::old_pass(Position target){

    double dirx,diry,length;

    dirx= (target.GetX()-ball.GetX());
    diry= (target.GetY()-ball.GetY());
    length= sqrt((dirx*dirx)+(diry*diry));
    dirx=dirx/length;
    diry=diry/length;

    Position pos(ball.GetX()+(dirx/80), ball.GetY()+(diry/80));

    if (robo.GetPos().DistanceTo(pos) > 0.1) {
        robo.turn(pos);
        if (ball.GetPos().DistanceTo(target)>=0.7){
            cout << "long distance: " << endl;
            robo.GotoPos(pos,2.5);
        }
        if (ball.GetPos().DistanceTo(target)>=0.5 && ball.GetPos().DistanceTo(target)<0.7){
            cout << "medium distance: " << endl;
            robo.GotoPos(pos,2.0);
        }
        else{
            cout << "short distance: " << endl;
            robo.GotoPos(pos,1.1);
        }


        }
    else //if (robo.isArrived())
    {
        return true; // Done
    }
    return false;

}

bool Player::old_kick(Position target){

    double dirx,diry,length;

    dirx= (target.GetX()-ball.GetX());
    diry= (target.GetY()-ball.GetY());
    length= sqrt((dirx*dirx)+(diry*diry));
    dirx=dirx/length;
    diry=diry/length;
    cout << "dirx: "<< dirx << endl;
    cout << "diry: "<< diry << endl;
    cout << "length: "<< length << endl;

    Position pos(ball.GetX()+(dirx/80), ball.GetY()+(diry/80));

    if (robo.GetPos().DistanceTo(pos) > 0.1) {
        robo.turn(pos);
        robo.GotoPos(pos,2.8);
        cout<<"--------"<<pos<<endl;
        cout<<"--------"<<target<<endl;
    }
    else{
        return true; // Done
    }

    return false;
}
