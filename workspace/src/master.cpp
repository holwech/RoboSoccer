#include "master.h"

Master::Master(string& team,
               RTDBConn& DBC,
               RoboControl& robo0,
               RoboControl& robo1,
               RoboControl& robo2,
               RawBall& ball,
               Referee& referee) :
                team(team),
                DBC(DBC),
                robo0(robo0),
                robo1(robo1),
                robo2(robo2),
                ball(ball),
                referee(referee)
{
    referee.Init();
    side = RIGHT_SIDE;
    state = STATE_MENU;
}

void Master::run() {
    while(1) {
        updateFieldSide();
        switch(state) {
        case STATE_MENU:
            menu();
            break;
        case STATE_GOALKEEPER:
            runGoalkeeper();
            break;
        case STATE_PENALTY:
            runPenalty();
            break;
        case STATE_STARTPOS:
            runStartPos();
            break;
        default:
            break;
        }
    }
}

void Master::menu() {
    bool cont = true;
    string input;
    while(cont) {
        cont = false;
        cout << "Choose programs penalty, goalkeeper, or starting position (p/g/s/stop): ";
        cin >> input;
        if (input == "p") {
            cout << "Starting penalty program." << endl;
            state = STATE_PENALTY;
        } else if (input == "g") {
            cout << "Starting goalkeeper program." << endl;
            state = STATE_GOALKEEPER;
        } else if (input == "s") {
            cout << "Starting starting position program." << endl;
            state = STATE_STARTPOS;
        } else if (input == "stop") {
            cont = false;
        } else {
            cont = true;
            cout << "Not an alternative, try again." << endl;
        }
    }
}


/** Runs the program for goalkeeper.
 * This program is complete and does not need further modifications.
 * This function is missing a way to exit the program.
 */
void Master::runGoalkeeper() {
    Position ballPos(ball.GetPos());

    ballPos = ball.GetPos();
    double ballangle = ball.GetPhi().Deg();
    double velocityY = sin(ballangle)*ball.GetVelocity();

    /** sleep function in microseconds
     *  Camera sampling rate is 30fps -> 33ms
     *  which means that field info does not change within this time
     */
    double nextPos = ball.GetY() + velocityY * (33 / 1000);
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
}

/**
  * This program needs to be fixed as it is not following the specs given on Moodle.
  * PLEASE FIX!!
  */

void Master::penaltyShoot(){
    if (referee.GetPlayMode()== PENALTY){
         cout << "Starting penalty." << endl;

//         Position pos1(ball.GetX()+0.5, ball.GetY());
//         cout << "Moving to " << pos1 << endl << endl;
//         robo0.GotoXY(pos1.GetX(), pos1.GetY(), 50, true);
//         while (robo0.GetPos().DistanceTo(pos1) > 0.05) usleep(50000); //sleep function in microseconds

//         Position pos2(ball.GetX()+0.3, ball.GetY());
//         cout << "Moving to " << pos2 << endl << endl;
//         robo0.GotoXY(pos2.GetX(), pos2.GetY(), 80, true);
//         while (robo0.GetPos().DistanceTo(pos2) > 0.1) usleep(50000); //sleep function in microseconds

         Position pos3(ball.GetX(), ball.GetY());
         cout << "Moving to " << pos3 << endl << endl;
         robo0.GotoXY(pos3.GetX(), pos3.GetY(), 100, true);
         while (robo0.GetPos().DistanceTo(pos3) > 0.1) usleep(50000); //sleep function in microseconds

         Position pos4(ball.GetX() - 0.3, ball.GetY());
         cout << "Moving to " << pos4 << endl << endl;
         robo0.GotoXY(pos4.GetX(), pos4.GetY(), 160, true);
         while (robo0.GetPos().DistanceTo(pos4) > 0.1) usleep(50000); //sleep function in microseconds

         //Position pos3(ball.GetX()+0.5,ball.GetY());
         //cout << "Moving to " << pos3 << endl << endl;
         //robo.GotoXY(pos3.GetX(), pos3.GetY(), 100, true);
         //while (robo.GetPos().DistanceTo(pos3) > 0.1) usleep(50000);



        //cout << "Moving to " << pos4 << endl << endl;
        //roboball.GetPos().GotoXY(pos4.GetX(), pos4.GetY(), 60, true);
        //while (robo.GetPos().DistanceTo(pos4) > 0.10) usleep(50000);

        robo0.Kick(100,0.0);
    }
}

void Master::runPenalty() {
    Position corner1(1.2,-0.8);
    Position corner2(1.2,0.8);

    //SideToShoot = referee.GetSide(); // 0 = left , 1 = right
//    while(true){
//        cout << referee.GetSide() << endl;
//        usleep(500000);
//    }

    static bool shotCompleted = false;
    static bool shooterInitFirstStepDone = false;
    cout << referee.GetPlayMode() << " " << shooterInitFirstStepDone << " " << referee.GetSide() << endl;

    switch(referee.GetPlayMode()){
    case BEFORE_PENALTY:
        if((referee.GetSide() == 0 && team == "blue") ||
            (referee.GetSide() == 1 && team == "red")){        // our turn to shoot
            shotCompleted = false;
            Position pos1;
            if(!shooterInitFirstStepDone){
                pos1 = Position(0.0,ball.GetY());
                if(robo0.GetPos().DistanceTo(pos1) < 0.2 ){
                    shooterInitFirstStepDone = true;
                }
            }
            else{
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




        }
        else{                         // our turn to defend
            Position pos1(-1.3,0.0);
            cout << "Moving to " << pos1 << endl << endl;
            if (robo1.GetPos().DistanceTo(corner1) > 0.20 ||
                   robo2.GetPos().DistanceTo(corner2) > 0.20 ||
                   robo0.GetPos().DistanceTo(pos1) > 0.20) {
                usleep(50000);
                robo0.GotoXY(pos1.GetX(), pos1.GetY(), 30, true);
                robo1.GotoXY(1.2, -0.8, 30, true);
                robo2.GotoXY(1.2, 0.8, 30, true);
            }
        }
        break;

    case PENALTY:
        shooterInitFirstStepDone = false;
        if( (referee.GetSide() == 0 && team == "blue") ||
            (referee.GetSide() == 1 && team == "red" ) ){
            if(!shotCompleted){
                penaltyShoot();
            }
            shotCompleted = true;
        }else{
            runGoalkeeper();
        }
        break;
    case REFEREE_INIT:
        state = STATE_MENU;
    default:
        break;
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

    cout << referee.GetSide() << "Inside runStartPos()" << endl;

    if (referee.GetPlayMode() == BEFORE_KICK_OFF) {
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
        } else {
            state = STATE_MENU;
        }
    }
}



