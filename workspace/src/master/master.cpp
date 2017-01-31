#include "master.h"
#include "master/tactics.cpp"
#include "master/strategies.cpp"

Master::Master(string& team, RTDBConn& DBC, vector<int>& rfNumber) :
                team(team),
                ball(DBC),
                referee(DBC),
                channel({
                    Channel(Command(ACTION_IDLE)),
                    Channel(Command(ACTION_IDLE)),
                    Channel(Command(ACTION_IDLE)),
                    Channel(Command(ACTION_IDLE)),
                    Channel(Command(ACTION_IDLE)),
                    Channel(Command(ACTION_IDLE))
                }),
                player({
                    Player(&channel[0], DBC, rfNumber[0]),
                    Player(&channel[1], DBC, rfNumber[1]),
                    Player(&channel[2], DBC, rfNumber[2]),
                    Player(&channel[3], DBC, rfNumber[3]),
                    Player(&channel[4], DBC, rfNumber[4]),
                    Player(&channel[5], DBC, rfNumber[5])
                }),
                positions(6),
                prevCommand({
                     Command(ACTION_IDLE),
                     Command(ACTION_IDLE),
                     Command(ACTION_IDLE),
                     Command(ACTION_IDLE),
                     Command(ACTION_IDLE),
                     Command(ACTION_IDLE)
                })

{
    state = REFEREE_INIT;
    referee.Init();
    resetTVariables();
    ticker.start();
}



void Master::run() {
    thread threadRobo0(&Player::run, std::ref(player[0]));
    thread threadRobo1(&Player::run, std::ref(player[1]));
    thread threadRobo2(&Player::run, std::ref(player[2]));
    usleep(10000);
    string answer;
    cout << "Enter manual/strategy/normal mode? (m/s/any key) ";
    cin >> answer;
    if(answer == "m") {
        manual();
    } else if (answer == "s") {
        send(Command(ACTION_DEFEND), 0);
        strategies();
    }

    cout << "Now running state machine" << endl;
    send(Command(ACTION_DEFEND), 0);
    while(1) {
        update();
        state = referee.GetPlayMode();
        switch(state) {
        case REFEREE_INIT:
            break;
        case BEFORE_KICK_OFF:
            before_kick_off();
            break;
        case KICK_OFF:
            if((referee.GetSide() == LEFT_SIDE && side==-1) || (referee.GetSide() == RIGHT_SIDE && side==1) || fabs(ball.GetX()) > 0.15){
                strategy_best();
            }
            break;
        case BEFORE_PENALTY:
            GoToBeforePenaltyPosition();
            break;
        case PENALTY:
            ActDuringPenalty();
            break;
        case PLAY_ON:
            strategy_best();
            break;
        case PAUSE:
            break;
        case TIME_OVER:
            break;
        default:
            cout << "No case for this state in master.run" << state << endl;
            break;
        }
        usleep(30000);
    }
    threadRobo0.join();
    threadRobo1.join();
    threadRobo2.join();
}

void Master::GoToBeforePenaltyPosition(){
    resetTVariables();
    if(side== LEFT){ // We are goal keeper during penalty shooting

        send(Command(ACTION_GOTO, Position(-1.19, 0), 1.5, bool(1)), 0);
        send(Command(ACTION_GOTO, Position(0.8, 0.3), 1.5, bool(1)), 1);
        send(Command(ACTION_GOTO, Position(0.8, -0.3), 1.5, bool(1)), 2);

    }else{ // We are attacker during penalty shooting

        send(Command(ACTION_GOTO, Position(0, 0), 1.5, bool(1)), 0);
        send(Command(ACTION_GOTO, Position(1, 0.3), 1.5, bool(1)), 1);
        send(Command(ACTION_GOTO, Position(1, -0.3), 1.5, bool(1)), 2);

    }

}


void Master::ActDuringPenalty(){
    if(side==-1){ // We are goal keeper during penalty shooting

        send(Command(ACTION_DEFEND), 0);

    }else{ // We are attacker during penalty shooting
        kickAtGoal(0,true);
        //send(Command(ACTION_GOTO, Position(0, 0), 1.5, bool(1)), 0);    
    }


}



/** Sends a command to a given robot. Assumes robo 0 if number is out of bounds */
void Master::send(Command command, int roboNum) {
    // If the prev command is the same as the current one,
    // do nothing, so to not spam the system with messages
    /*
    if (prevCommand[roboNum] == command) {
        cout << "Command: " << action_names[command.action] << " rejected" << endl;
        prevCommand[roboNum] = command;
        return;
    }*/
    if (DEBUG && (prevCommand[roboNum].action != command.action)) {
        cout << "\033[1;31m#MASTER: Sending action to robo #" << roboNum << ": " << action_names[command.action] << "\033[0m" << endl;
    }
    if (roboNum > 2 || roboNum < 0) {
        cout << "Robo " << roboNum << " does not exist" << endl;
        roboNum = 0;
    }
    if (command.action != ACTION_IDLE) {
        player[roboNum].setBusy(true);
    }
    channel[roboNum].write(command);
    prevCommand[roboNum] = command;
}

void Master::updatePositions() {
    for (int i = 0; i < 6; i++) {
        positions[i] = player[i].getPos();
    }
    for (int i = 0; i < 6; i++) {
        player[i].update(positions);
    }
}


void Master::masterPrint(string str) {
    cout << "\033[1;31m#MASTER: " << str << "\033[0m" << endl;
}

/** Add your strategies or tactics here. (Yes, I know, misleading function name) */
void Master::strategies() {
    int answer = -1;
    closestRobo = 0;

    while(1) {
        update();
       if (ticker.getTime() > std::chrono::duration<double, std::milli>(5000)) {
           cout << "Master alive!" << endl;
            ticker.reset();
       }
        switch(answer) {
        case 1:
            tacticDone = crossPassAndShoot();
            if (tacticDone) { answer = -1; }
            break;
        case 2:
            tacticDone = tactic_nearpenaltyarea(0.65);
            //if (tacticDone) { answer = -1; }
            break;
        case 3:
            tacticDone = tactic_ballchasing();
            if (tacticDone) { answer = -1; }
            break;    
        case 4:
            strategy_defensive();
            usleep(10);
            break;
        case 5:
            strategy_offensive();
            break;
        case 6:
            tacticDone = kickAtGoal(1);
            if (tacticDone) {
                resetTVariables(); }
            break;   
        case 7:
            strategy_offensive2();
            break;
        case 8: {
            send(Command(ACTION_GOTO, ball.predictInY(0.0), 1.5), 0);
            break;
        }
        case 9:
            tacticDone = bounceForward();
            if (tacticDone) { answer = -1; }
            break;
        case 10:
            closestRobo = setClosest(closestRobo, true, true);
            cout << "Closest robo is: " << closestRobo << endl;
            sleep(1);
            break;
        case 11:
            strategy_best();
            usleep(10);
            break;
        default:
            cout << "Select one of the following strategies/tactics: "<<endl;
            cout << "	1. Cross and Pass" << endl;
            cout << "	2. Tactic_nearpenaltyarea"<<endl;
            cout << "	3. Tactic_ballchasing"<<endl;
            cout << "	4. Strategy_defensive"<<endl;
            cout << "	5. strategy_offensive"<<endl;
            cout << "	6. kickAtGoal" << endl;
            cout << "	7. Strategy_offensive2"<<endl;
            cout << "	8. Test random stuff" << endl;
            cout << "	9. bounce forward" << endl;
            cout << "	10. test closest robo" << endl;
            cout << "	11. Strategy best" << endl;
            resetTVariables();
            cin >> answer;
            break;
        }
        usleep(30000);
    }
}

// Use this function for single actions only. No strategies or tactics. It won't work.
void Master::manual() {
    int answer;
    int robot;
    double posX, posY, speed, approachSpeed;
    bool ca;
    while(1) {
        update();
        cout << "Choose an action" << endl;
        cout << "	0. EXIT" << endl;
        cout << "	1. ACTION_GOTO" << endl;
        cout << "	2. BEFORE_KICK" << endl;
        cout << "	3. KICK" << endl;
        cout << "	4. DEFEND" << endl;
        cout << "	5. PASS" << endl;
        cout << "	6. LISTEN TO REFEREE" << endl;
        cout << "	7. TEST BALL CLASS" << endl;
        cout << "	8. STOP" << endl;
        cout << "	9. TEST POS_TO_BOUNCE" << endl;
        cout << " 10.Test of before penalty" << endl;
         cout << " 11.Test of shoot penalty" << endl;

        cin >> answer;
        cout << "Which robot? (0-2)" << endl;
        cin >> robot;
        switch(answer) {
        case 0:
            return;
        case 1:
            cout << "x: ";
            cin >> posX;
            cout << "y: ";
            cin >> posY;
            cout << "Speed (1-2 recommended): " << endl;
            cin >> speed;
            cout << "Collision avoidance (1/0): ";
            cin >> ca;
            send(Command(ACTION_GOTO, Position(posX, posY), speed, ca), robot);
            break;
        case 2:
            cout << "Approach speed: ";
            cin >> approachSpeed;
            send(Command(ACTION_BEFORE_KICK, ball.GetPos(), Position(1.2, 0), approachSpeed), robot);
            break;
        case 3:
            cout << "Speed: ";
            cin >> speed;
            cout << "Approach speed: ";
            cin >> approachSpeed;
            send(Command(ACTION_KICK, Position(1.27, 0), speed, approachSpeed), robot);
            break;
        case 4:
            send(Command(ACTION_DEFEND), robot);
            break;
        case 5:
            send(Command(ACTION_PASS, Position(1.2,0)),robot);
            break;
        case 6:
            while(1){
                updateSide();
                printRefereeStats();
                sleep(1);
            }
            break;
        case 7: {
            test_ball testBallHandler(ball);
            testBallHandler.test(side);
            break;
        }
        case 8:
            send(Command(ACTION_GOTO, Position(1.0, 0.0), 1.5), 0);
            sleep(2);
            send(Command(ACTION_STOP), 0);
            break;
        case 9:
            send(Command(ACTION_BLOCK_BALL, Position(0,0) ), 1);
            break;
        case 10:
            GoToBeforePenaltyPosition();
            break;
        case 11:
            ActDuringPenalty();
            break;
        default:
            cout << "No action created for this choice yet in master.manual" << endl;
            break;
        }

    }
}


void Master::printRefereeStats(){
    cout << "Referee stats: " << endl;
    cout << "\tPlaymode: " << referee.GetPlayMode() << endl;
    cout << "\tGetSide: " << referee.GetSide() << endl;
    cout << "\tGetBlueSide: " << referee.GetBlueSide() << endl;
    cout << "\tCalculated side: " << side << endl;
}

void Master::update() {
    updateSide();
    updatePositions();
    ball.updateSample();
}

void Master::updateSide(){
    if (team == "b"){
        if (referee.GetBlueSide() == 0){
            side = LEFT;
        }
        else{
            side = RIGHT;
        }
    }
    else{
        if (referee.GetBlueSide() == 0){
            side = RIGHT;
        }
        else{
            side = LEFT;
        }
    }
    for(int i = 0; i < (int)player.size(); i++){
        player[i].side = side;
    }
}

void Master::resetTVariables() {
    s_case = INIT;
    t_state = STEP1;
    t_state2 = STEP1;
    tacticDone = false;
    cps_state = 1;
    closestRobo = -1;
    maxDistance = 10;
    chrossandpassy = 0; // used for tactics: Chross and Pass
}

// Gives the number of the closest robot
int Master::getClosest(bool withKeeper) {
    if (withKeeper && (player[0].getPos().DistanceTo(ball.GetPos()) < player[1].getPos().DistanceTo(ball.GetPos()))) {
      return 0;
    } else if (player[1].getPos().DistanceTo(ball.GetPos()) < player[2].getPos().DistanceTo(ball.GetPos())) {
      return 1;
    } else {
      return 2;
    }
}

// Gets the player further away
int Master::getNotClosest() {
    if (player[1].getPos().DistanceTo(ball.GetPos()) > player[2].getPos().DistanceTo(ball.GetPos())) {
      return 1;
    } else {
      return 2;
    }
}

int Master::getClosestToTeamGoal() {
    if (player[1].getPos().DistanceTo(Position(1.27 * side, 0.0)) < player[2].getPos().DistanceTo(Position(1.27 * side, 0.0))) {
      return 1;
    } else {
      return 2;
    }
}

int Master::getNotClosestToTeamGoal() {
    if (player[1].getPos().DistanceTo(Position(1.27 * side, 0.0)) > player[2].getPos().DistanceTo(Position(1.27 * side, 0.0))) {
      return 1;
    } else {
      return 2;
    }
}


// Gives the number of the closest robo and also stops the what the current robot is doing
// and resets the tactic if reset is set to true
int Master::setClosest(int currClosest, bool resetTactic, bool withKeeper) {
    int newClosest = getClosest(withKeeper);
    if (currClosest != newClosest && currClosest != -1) {
        //cout << "NEW CLOSEST ROBO: " << newClosest << " OLD: " << currClosest << endl;
        send(Command(ACTION_STOP), currClosest);
        if (resetTactic) {
           t_state = STEP1;
        }
    }
    return newClosest;
}

// Checks wether it is closest or not. If it has change, the tactic will reset
void Master::checkClosest(int currClosest, bool withKeeper) {
    setClosest(currClosest, true, withKeeper);
}

// Returns the position of the other teams keeper
Position Master::getOtherKeeperPos() {
    for (int p = 3; p < 6; p++) {
        double posX = positions[p].GetX();
        double posY = positions[p].GetY();
        if (fabs(posY) < 0.340 && fabs(posX) > 1.2) {
            return positions[p];
        }
    }
    return Position(0.0, 0.0);
}

// Return 1 if keeper in goal area. 0 if no keeper in goal area. -1 if multiple players
// were registered in the goal area
int Master::otherKeeperInGoalArea() {
    bool keeperInArea = false;
    for (int p = 3; p < 6; p++) {
        double posX = positions[p].GetX();
        double posY = positions[p].GetY();
        if (fabs(posY) < 0.340 && fabs(posX) > 1.2) {
            if (keeperInArea == true) {
                return -1;
            } else {
                keeperInArea = true;
            }
        }
    }
    if (keeperInArea == false) {
        return 0;
    } else {
        return 1;
    }
}



//send(Command(ACTION_GOTO, Position(x, y), 1.0), robonumber);
void Master::before_kick_off(){

    if (referee.GetSide()==0) {
        //we kick the ball at left, so we take a attack position at left
        if (side == LEFT){
            send(Command(ACTION_GOTO, Position(-1.36, 0), 1.0, true), 0);
            send(Command(ACTION_GOTO, Position(-0.2, -0.2), 1.0, true), 1);
            send(Command(ACTION_GOTO, Position(-0.1, 0.3), 1.0, true), 2);
        }
        //enemy kick the ball at left, so we take a defend position at right
        else {
            send(Command(ACTION_GOTO, Position(1.36, 0), 1.0, true), 0);
            send(Command(ACTION_GOTO, Position(0.2, 0), 1.0, true), 1);
            send(Command(ACTION_GOTO, Position(0.6, 0), 1.0, true), 2);
        }

    }

    else {
        //we kick the ball at right, so we take a attack position at right
        if (side == RIGHT){
            send(Command(ACTION_GOTO, Position(1.36, 0), 1.0, true), 0);
            send(Command(ACTION_GOTO, Position(0.2, -0.2), 1.0, true), 1);
            send(Command(ACTION_GOTO, Position(0.1, 0.3), 1.0, true), 2);
        }
        //enemy kick the ball at right, so we take a defend position at left
        else {
            send(Command(ACTION_GOTO, Position(-1.36, 0), 1.0, true), 0);
            send(Command(ACTION_GOTO, Position(-0.2, 0), 1.0, true), 1);
            send(Command(ACTION_GOTO, Position(-0.6, 0), 1.0, true), 2);
        }
    }
    if (!player[0].isBusy()&&!player[1].isBusy()&&!player[2].isBusy()){
        if (team=='b'){
            referee.SetBlueReady();
        }
        else{
            referee.SetRedReady();
        }

    }

}


























