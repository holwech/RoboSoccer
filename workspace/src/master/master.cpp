#include "master.h"
Master::Master(string& team,
               RTDBConn& DBC,
               Robo& robo0,
               Robo& robo1,
               Robo& robo2,
               Robo& robo3,
               Robo& robo4,
               Robo& robo5,
               RawBall& ball,
               Referee& referee) :
                team(team),
                DBC(DBC),
                robo0(robo0),
                robo1(robo1),
                robo2(robo2),
                robo3(robo3),
                robo4(robo4),
                robo5(robo5),
                ball(ball),
                referee(referee),
                debugTimer()
{
    cout << "Constructing Master variables..." << endl;
    side = RIGHT_SIDE;
    state = REFEREE_INIT;
    for (int i = 0; i <= 5; i++) {
        channels.push_back(Channel());
    }
    updatePositions();
}



void Master::run() {

    cout << "Starting state machine..." << endl;
    debugTimer.start();
    printInfo();
    while(1) {
        /** Timer for printing info about the system, so that it doesn't spam the
          * terminal.
          */
        if (debugTimer.getTime() > 2) {
            printInfo();
            debugTimer.reset();
        }
        state = referee.GetPlayMode();
        switch(state) {
        case REFEREE_INIT:
            break;
        case BEFORE_KICK_OFF:
            runStartPos();
            break;
        case KICK_OFF:
            break;
        case BEFORE_PENALTY:
            beforePenalty();
            break;
        case PENALTY:
            penalty();
            break;
        case PLAY_ON:
            break;
        case PAUSE:
            break;
        case TIME_OVER:
            break;
        default:
            cout << "No case for state." << state << endl;
            break;
        }
    }
}

void Master::updatePositions() {
    positions[0] = robo0.GetPos();
    positions[1] = robo1.GetPos();
    positions[2] = robo2.GetPos();
    positions[3] = robo3.GetPos();
    positions[4] = robo4.GetPos();
    positions[5] = robo5.GetPos();
    ballPos = ball.GetPos();
}


/** Runs the program for goalkeeper.
 * This program is complete and does not need further modifications.
 * This function is missing a way to exit the program.
 */
void Master::runGoalkeeper() {
/*    Position ballPos(ball.GetPos());

    ballPos = ball.GetPos();
    double ballangle = (ball.GetPhi().Deg()+ball.GetPhi().Deg()+ball.GetPhi().Deg()+ball.GetPhi().Deg()+ball.GetPhi().Deg()+ball.GetPhi().Deg())/6;
    double velocityY = sin(ballangle)*ball.GetVelocity();

    ** sleep function in microseconds
     *  Camera sampling rate is 30fps -> 33ms
     *  which means that field info does not change within this time
     */
/*    double nextPos = ball.GetY() + velocityY * (33 / 1000);
    if (nextPos > 0.2) {
        nextPos = 0.2;
    } else if (nextPos < -0.2) {
        nextPos = -0.2;
    }

    double side = (team == "blue" || referee.GetPlayMode() == PENALTY || referee.GetPlayMode() == BEFORE_PENALTY) ? -1.3 : 1.3;
    robo0.GotoXY(side , nextPos, 60, true);
    while (abs(robo0.GetY() - ball.GetY()) > 0.40 && abs(robo0.GetX() - 0.4) > 0.40) {
        usleep(5000);
    }
*/
    int i= 0;
    double ballangle = 0;
    double ballx = 0;
    double bally= 0;
    double delta;

    for(i=0;i<100;i++)
    {
    ballangle = ball.GetPhi().Deg()+ballangle;
    }
    ballangle =  ballangle/100;

    for(i=0;i<100;i++)
    {
    ballx = ballx + ball.GetX();
    bally = bally + ball.GetY();
    }

    ballx = ballx/100;
    bally = bally/100;

    double goalkeeperx = (team == "blue" || referee.GetPlayMode() == PENALTY || referee.GetPlayMode() == BEFORE_PENALTY) ? 1.36 : -1.36;
    double goalkeepery;

    goalkeepery = tan(ballangle*3.141593/180)*(goalkeeperx-ballx)+bally;

    if (goalkeepery ==bally)
        goalkeepery = 0;
    if(goalkeepery>0.269)
        goalkeepery = 0.269;
    if(goalkeepery<-0.245)
        goalkeepery = -0.245;

     robo0.CruisetoXY(goalkeeperx,goalkeepery,150);

     delta = sqrt((robo0.GetX()-goalkeeperx)*(robo0.GetX()-goalkeeperx)+(robo0.GetY()-goalkeepery)*(robo0.GetY()-goalkeepery));

     if(delta<0.03)
      robo0.StopAction();

     while(robo0.GetPos().DistanceTo(ball.GetPos())<0.2)
         robo0.GotoXY(ball.GetX(),ball.GetY(),160);
}

/**
  * This program needs to be fixed as it is not following the specs given on Moodle.
  * PLEASE FIX!!
  */

void Master::penaltyShoot(){
     cout << "Starting penalty." << endl;
     Position pos3(ball.GetX(), ball.GetY());
     robo0.GotoXY(pos3.GetX(), pos3.GetY(), 100, true);
     while (robo0.GetPos().DistanceTo(pos3) > 0.1) usleep(50000); //sleep function in microseconds

     Position pos4(ball.GetX() - 0.3, ball.GetY());
     robo0.GotoXY(pos4.GetX(), pos4.GetY(), 160, true);
     while (robo0.GetPos().DistanceTo(pos4) > 0.1) usleep(50000); //sleep function in microseconds


    robo0.Kick(100,0.0);
}

void Master::beforePenalty() {
    Position corner1(1.2,-0.8);
    Position corner2(1.2,0.8);
    shotCompleted = false;
    shooterInitFirstStepDone = false;
    if((referee.GetSide() == 0 && team == "blue") ||
        (referee.GetSide() == 1 && team == "red")){        // our turn to shoot
        shotCompleted = false;
        Position pos1;
        if(!shooterInitFirstStepDone){
            pos1 = Position(0.0,ball.GetY());
            if(robo0.GetPos().DistanceTo(pos1) < 0.2 ){
                shooterInitFirstStepDone = true;
            }
        } else{
            pos1 = Position(ball.GetX() + 0.1 , ball.GetY());
        }


        if (robo1.GetPos().DistanceTo(corner1) > 0.20 ||
               robo2.GetPos().DistanceTo(corner2) > 0.20 ||
               robo0.GetPos().DistanceTo(pos1) > 0.20) {

            robo0.GotoXY(pos1.GetX(), pos1.GetY(), 30, true);
            robo1.GotoXY(1.0, -0.8, 30, true);
            robo2.GotoXY(1.0, 0.8, 30, true);
            usleep(50000);
        }
    } else {                         // our turn to defend
        Position pos1(-1.3,0.0);
        if (robo1.GetPos().DistanceTo(corner1) > 0.20 ||
               robo2.GetPos().DistanceTo(corner2) > 0.20 ||
               robo0.GetPos().DistanceTo(pos1) > 0.20) {
            usleep(50000);
            robo0.GotoXY(pos1.GetX(), pos1.GetY(), 30, true);
            robo1.GotoXY(1.2, -0.8, 30, true);
            robo2.GotoXY(1.2, 0.8, 30, true);
        }
    }
}

void Master::penalty() {
    shooterInitFirstStepDone = false;
    if((referee.GetSide() == 0 && team == "blue") ||
       (referee.GetSide() == 1 && team == "red")){
        if(!shotCompleted){
            penaltyShoot();
        }
        shotCompleted = true;
    }else{
        runGoalkeeper();
    }
}

void Master::updateFieldSide(){
    eSide blueSide = referee.GetBlueSide();
    if( (team == "blue" && blueSide == LEFT_SIDE) || (team == "red" && blueSide == RIGHT_SIDE)){
        side = LEFT_SIDE;
    }else{
        side = RIGHT_SIDE;
    }
}


/**
  * This program is maybe complete (I think?). Test and reread specs again to make sure.
  *
  */
void Master::runStartPos() {

    if ((referee.GetSide() == 0 && team == "blue") ||
        (referee.GetSide() == 1 && team == "red")) {
        Position start1;
        Position start2;
        Position start3;
        if (side == LEFT_SIDE) {
            start1 = Position(-0.4, -0.3);
            start2 = Position(-1.0, 0.0);
            start3 = Position(-0.15, 0);
        } else {
            start1 = Position(0.4, -0.3);
            start2 = Position(1.0, 0.0);
            start3 = Position(0.15, 0);
        }

        if (robo0.GetPos().DistanceTo(start1) > 0.20 ||
            robo1.GetPos().DistanceTo(start2) > 0.20 ||
            robo2.GetPos().DistanceTo(start3) > 0.20) {

           usleep(50000);
           robo0.GotoXY(start1.GetX(), start1.GetY(), 80, true);
           robo1.GotoXY(start2.GetX(), start2.GetY(), 80, true);
           robo2.GotoXY(start3.GetX(), start3.GetY(), 80, true);
        }
    }
}

void Master::printInfo() {
    string stateNames[8] = {"Referee init", "Before kick-off", "Kick-off", "Before penalty",
                            "Penalty", "Play on", "Pause", "Time over"};
    cout << "====================" << endl;
    cout << "= State: " << stateNames[state] << endl;
    cout << "====================" << endl;
}

