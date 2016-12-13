#include "player/player.h"

void Player::defend()
{
    int i= 0;
    double ballangle = 0;
    double ballx = 0;
    double bally= 0;
    double delta;
    Position POS;



    for(i=0;i<10;i++)
    {
    ballangle = ball->GetPhi().Deg()+ballangle/10;
    }

    for(i=0;i<10;i++)
    {
    ballx = ballx + ball->GetX()/10;
    bally = bally + ball->GetY()/10;
    }
    double goalkeeperx =  1.36 ;
    double goalkeepery;

    goalkeepery = tan(ballangle*3.141593/180)*(goalkeeperx-ballx)+bally;

    if (goalkeepery ==bally)
        goalkeepery = 0;
    if(goalkeepery>0.269)
        goalkeepery = 0.269;
    if(goalkeepery<-0.245)
        goalkeepery = -0.245;


    POS.SetX(goalkeeperx);
    POS.SetY(goalkeepery);
    robo->GotoPos(POS);
 //    robo->CruisetoXY(goalkeeperx,goalkeepery,150);


     delta = sqrt((robo->GetX()-goalkeeperx)*(robo->GetX()-goalkeeperx)+(robo->GetY()-goalkeepery)*(robo->GetY()-goalkeepery));

     if(delta<0.05)
      robo->StopAction();

/*     while(robo->GetPos().DistanceTo(ball.GetPos())<0.2)
         robo->GotoXY(ball.GetX(),ball.GetY(),160);
*/
}



void Player::goalkeeperkick(){


}
