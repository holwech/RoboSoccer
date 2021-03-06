
#include "player/player.h"
#include "cmath"

#define GOALKEEPER_SPEED 1.5
#define GOALKEEPER_KICK_SPEED 2

void Player::defend_tom(){
    //int x=ball.GetX();
    double x = ball.GetX();
    double y = ball.GetY();
    Angle dir=ball.GetPhi();

    if (y<-0.23 || y>0.23 || x>=1.35){
        y = ball.GetY();
        dir=ball.GetPhi();
        cout << "dir:" << dir << endl;
        cout << "staying" << endl;
    }
    else{
        y = ball.GetY();
        dir = ball.GetPhi();
        if(dir > 0 ){
            robo.GotoPos(1.35,ball.GetY()+0.1);
        }
        if(dir < 0){
            robo.GotoPos(1.35,ball.GetY()-0.1);
        }
        else{
            robo.GotoPos(1.35,ball.GetY());
        }
    }
}

void Player::getNextGoalkeeperState(){
    //Right close corner
    Position ballpos = ball.GetPos();
    switch(Gstate){
    case BLOCK_WINDOW:
    case BLOCK_NOT_WINDOW:
    case DYNAMIC_DEFEND:
    case GOALKEEPER_STOP:
    case GOALKEEPER_KICK:
        // check if ball in corners towards window
        if(ball.inGoalArea() && abs(ballpos.GetX()) > 1.4){
            Gstate = GOALKEEPER_STOP;
        }
        else if(ball.isStopped() && ball.inGoalArea(side) && (fabs(robo.GetPos().GetX())-fabs(ballx))>0.05){
            Gstate = GOALKEEPER_KICK;
        }

        else if(ballpos.GetX()*side > 1.4-abs(ballpos.GetY()) ){
            if(ballpos.GetY() < 0){
                Gstate = BLOCK_WINDOW;
            }
            else{
                Gstate = BLOCK_NOT_WINDOW;
            }
        }
        else{
            Gstate = DYNAMIC_DEFEND;
        }
    default:
        break;
    }
}

void Player::defend(){
    double goalypos_x  = 0;
    double goalypos_y  = 0;
    double ballangle = 0;
    double ballx = 0;
    double bally = 0;
    bool debug = false;
    Position pos;
    if (side == LEFT){
        goalypos_x = -1.36;
    }
    else{
        goalypos_x = 1.40;
    }
    //get position and angle of the ball

    ballangle = ball.GetPhi().Deg()+ballangle;
    Position ballPos = ball.GetPos();
    ballx = ballPos.GetX(); //------change
    bally = ballPos.GetY();//-------change

    getNextGoalkeeperState();
    switch(Gstate){
    case BLOCK_WINDOW:
        if(debug){
            cout << "In BLOCK_WINDOW" << endl;
        }
        goalypos_y = -0.19;
        pos.SetX(goalypos_x);
        pos.SetY(goalypos_y);
        if(robo.GetPos().DistanceTo(pos) < 0.03){
            robo.stop();
        }else{

            robo.GotoPos(pos, GOALKEEPER_SPEED);

        }
        break;
    case BLOCK_NOT_WINDOW:
        if(debug){
            cout << "In BLOCK_NOT_WINDOW" << endl;
        }
        goalypos_y = 0.19;
        pos.SetX(goalypos_x);
        pos.SetY(goalypos_y);
        if(robo.GetPos().DistanceTo(pos) < 0.03){
            robo.stop();
        }else{

            robo.GotoPos(pos, GOALKEEPER_SPEED);

        }
        break;
    case GOALKEEPER_STOP:
        if(debug){
            cout << "In GOALKEEPER_STOP" << endl;
        }
        robo.stop();
        break;
    case DYNAMIC_DEFEND:
        if(debug){
            cout << "In DYNAMIC_DEFEND" << endl;
        }
        // towards our goal
        if(side == LEFT){
            if (!(ball.GetPhi().Deg()<90 && ball.GetPhi().Deg()>-90 && abs(ballx) < 1.30)){
                goalypos_y = tan(ballangle*M_PI/180)*((goalypos_x-0.047*side)-ballx)+bally;
            }
            else{
                goalypos_y = bally;
            }
        }
        else if(side == RIGHT){
            if(ball.GetPhi().Deg()<90 && ball.GetPhi().Deg()>-90 && abs(ballx) <1.30){
                goalypos_y = tan(ballangle*M_PI/180)*((goalypos_x-0.047*side)-ballx)+bally;
            }
            else{
                goalypos_y = bally;
            }

        }
        if (goalypos_y > 0.19){
            goalypos_y = 0.19;
        }
        else if (goalypos_y < -0.19){
            goalypos_y = -0.19;
        }
        pos.SetX(goalypos_x);
        pos.SetY(goalypos_y);

        robo.GotoPos(pos, GOALKEEPER_SPEED);

        break;
    case GOALKEEPER_KICK:
        pos.SetX(ballx);
        pos.SetY(bally);
        robo.GotoPos(pos, GOALKEEPER_KICK_SPEED);
        break;
    default:
        break;
    }
}


    /*
    int i= 0;
    double ballangle = 0;
    double ballx = 0;
    double bally = 0;
    //double delta;
    Position POS(0,0);
    double goalkeepery = 0;
    Position Mid;
    double goalkeeperx;

    //initial goalkeeper position depending on which side
    Mid = Position(1.4*side,0);
    goalkeeperx = 1.4*side;

    //get position and angle of the ball
    for(i=0;i<10;i++)
    {
        ballangle = ball.GetPhi().Deg()+ballangle;
    }
    ballangle = ballangle/10;
    for(i=0;i<10;i++)
    {
        ballx += ball.GetX();
        bally += ball.GetY();
    }
    ballx = ballx/10;
    bally = bally/10;

    if(ball.isStopped() && ball.inGoalArea() && (fabs(goalkeeperx)-fabs(ballx))>0 && (fabs(goalkeeperx)-fabs(ballx))<0.2 )
    {
        robo.CruisetoXY(ballx,bally,200);
    }

    else if(ball.isStopped() && ball.inGoalArea() && (fabs(goalkeeperx)-fabs(ballx))<0 )
    {
        robo.StopAction();
    }

    else
    {
    // if the ball stops
        if (ball.isStopped())
        {
            //goalkeepery = 0;
        }

    // if the ball is too near to our penalty area, goalkeepery = bally
        if(ballx>1.1||ballx<-1.1)
        {
            goalkeepery = bally;
        }

    // if the ball is moving towards our gate, goalie moves. If the ball is moving far from gate, goalie goes to the initial position

        if(ball.GetPhi().Deg()<90 && ball.GetPhi().Deg()>-90 && side)
        {
            goalkeepery = tan(ballangle*M_PI/180)*((goalkeeperx-0.047*side)-ballx)+bally;
        }

        else if(!(ball.GetPhi().Deg()<90 && ball.GetPhi().Deg()>-90) && !side)
        {
            goalkeepery = tan(ballangle*M_PI/180)*((goalkeeperx-0.047*side)-ballx)+bally;
        }





        if(goalkeepery>0.16)
            goalkeepery = 0.16;
        else if(goalkeepery<-0.18)
            goalkeepery = -0.18;


        POS.SetX(goalkeeperx);
        POS.SetY(goalkeepery);

    //        robo.GotoPos(POS, 0.9);
        robo.CruisetoXY(goalkeeperx,goalkeepery,140);



    }
    */





