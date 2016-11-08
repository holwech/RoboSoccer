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
    state = STATE_MENU;
}

void Master::run() {
    string input;
    while(1) {
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
    double velocityy = sin(ballangle)*ball.GetVelocity();

    /** sleep function in microseconds
     *  Camera sampling rate is 30fps -> 33ms
     *  which means that field info does not change within this time
     */
    double nextPos = ball.GetY() + velocityy * (33 / 1000);
    if (nextPos > 0.2) {
        nextPos = 0.2;
    } else if (nextPos < -0.2) {
        nextPos = -0.2;
    }
    double side = (team == "blue") ? -1.3 : 1.3;
    robo0.GotoXY(side , nextPos, 50, true);
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

         Position pos1(ball.GetX()+0.5, ball.GetY());
         cout << "Moving to " << pos1 << endl << endl;
         robo0.GotoXY(pos1.GetX(), pos1.GetY(), 50, true);
         while (robo0.GetPos().DistanceTo(pos1) > 0.05) usleep(50000); //sleep function in microseconds

         Position pos2(ball.GetX()+0.3, ball.GetY());
         cout << "Moving to " << pos2 << endl << endl;
         robo0.GotoXY(pos2.GetX(), pos2.GetY(), 80, true);
         while (robo0.GetPos().DistanceTo(pos2) > 0.1) usleep(50000); //sleep function in microseconds

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
    string turn;
    while (true){
        cout << referee.GetSide() << endl;
        usleep(100000);
    }
    do{
    cout << "Penalty! \nShooting or defending?(shoot/defend): ";
    cin >> turn;
    }while ( !(turn == "shoot" || turn == "defend") );


    while(true){

        while (referee.GetPlayMode()== BEFORE_PENALTY){
            Position pos1(0.0,0.0);
            cout << "Moving to " << pos1 << endl << endl;
            robo0.GotoXY(pos1.GetX(), pos1.GetY(), 60, true);
            robo1.GotoXY(1.2, -0.8, 100, false);
            robo2.GotoXY(1.2, 0.8, 100, false);
            while (robo1.GetPos().DistanceTo(corner1) > 0.10 ||
                   robo2.GetPos().DistanceTo(corner2) > 0.10 ||
                   robo0.GetPos().DistanceTo(pos1) > 0.05) {
                usleep(50000);
            }
        }
        if(turn == "shoot"){
            penaltyShoot();
            turn = "defend";
        }else{
                state = STATE_GOALKEEPER;
            turn == "shoot";
        }

    }

}

/**
  * This program is maybe complete (I think?). Test and reread specs again to make sure.
  *
  */
void Master::runStartPos() {

    while (referee.GetPlayMode()==REFEREE_INIT){ }

    if (referee.GetPlayMode()==BEFORE_KICK_OFF) {
        Position start1(-0.4, -0.3);
        Position start2(-1.0, 0.0);
        Position start3(-0.15, 0);

        while (1) {
            robo0.GotoXY(start1.GetX(), start1.GetY(), 100, false);
            robo1.GotoXY(start2.GetX(), start2.GetY(), 100, false);
            robo2.GotoXY(start3.GetX(), start3.GetY(), 100, false);
            while (robo0.GetPos().DistanceTo(start1) > 0.10 ||
                   robo1.GetPos().DistanceTo(start2) > 0.10 ||
                   robo2.GetPos().DistanceTo(start3) > 0.10) {
                   usleep(50000);
            }
        }
    }
    cout << "Leaving starting position program." << endl;
}



