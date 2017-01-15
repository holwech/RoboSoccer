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
                positions(6) {
    state = REFEREE_INIT;
    referee.Init();
    resetTVariables();
}



void Master::run() {
    thread threadRobo0(&Player::run, std::ref(player[0]));
    thread threadRobo1(&Player::run, std::ref(player[1]));
    thread threadRobo2(&Player::run, std::ref(player[2]));
    usleep(1000);
    string answer;
    cout << "Enter manual/strategy/normal mode? (m/s/any key) ";
    cin >> answer;
    if(answer == "m") {
        manual();
    } else if (answer == "s") {
        strategies();
    }

    cout << "Starting state machine..." << endl;
    while(1) {
        updateSide();
        updatePositions();
        state = referee.GetPlayMode();
        switch(state) {
        case REFEREE_INIT:
            break;
        case BEFORE_KICK_OFF:
            break;
        case KICK_OFF:
            exampleTactic();
            break;
        case BEFORE_PENALTY:
            break;
        case PENALTY:
            break;
        case PLAY_ON:
            break;
        case PAUSE:
            break;
        case TIME_OVER:
            break;
        default:
            cout << "No case for this state in master.run" << state << endl;
            break;
        }
    }
    threadRobo0.join();
    threadRobo1.join();
    threadRobo2.join();
}

/** Sends a command to a given robot. Assumes robo 0 if number is out of bounds */
void Master::send(Command command, int roboNum) {
    if (roboNum > 2 || roboNum < 0) {
        cout << "Robo " << roboNum << " does not exist" << endl;
        roboNum = 0;
    }
    if (command.action != ACTION_IDLE) {
        player[roboNum].setBusy(true);
    }
    channel[roboNum].write(command);
}

void Master::updatePositions() {
    for (int i = 0; i < 6; i++) {
        positions[i] = player[i].getPos();
    }
    for (int i = 0; i < 6; i++) {
        player[i].update(positions);
    }
}

void Master::strategyController() {
    // Do something to choose strategies
    switch(s_state) {
    case BALANCED:
        // Run the balanced strategy
        break;
    case AGGRESSIVE:
        break;
    case DEFENSIVE:
        break;
    default:
        cout << "No strategy for this state" << endl;
        break;
    }
}

/** Add your strategies or tactics here. (Yes, I know, misleading function name) */
void Master::strategies() {
    int answer;
    cout << "Select one of the following strategies/tactics: "<<endl;
    cout << "	1. Cross and Pass" << endl;
    cout << "   2. Tactic_nearpenaltyarea"<<endl;
    cout << "   3. Tactic_ballchasing"<<endl;
    cout << "   4. Strategy_defensive"<<endl;
    cout << "   5. Strategy_offensive"<<endl;
    cout << "	6. kickAtGoal" << endl;
    cout << "   7. Strategy_offensive2"<<endl;
    cout << "	8. Monitor positions" << endl;

    cin >> answer;
    while(1) {
        updateSide();
        updatePositions();
        switch(answer) {
        case 1:
            tacticDone = crossPassAndShoot();
            if (tacticDone) { answer = -1; }
            break;
        case 2:
            tacticDone = tactic_nearpenaltyarea(0.65);
            if (tacticDone) { answer = -1; }
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
            tacticDone = kickAtGoal();
            if (tacticDone) { answer = -1; }
            break;   
        case 7:
            strategy_offensive2();
            break;
        case 8:
            break;
        default:
            cout << "No case for this state yet (in strategies function), or tactic terminated" << endl;
            cout << "Select one of the following strategies/tactics: "<<endl;
            cout << "   1. Cross and Pass" << endl;
            cout << "   2. Tactic_nearpenaltyarea"<<endl;
            cout << "   3. Tactic_ballchasing"<<endl;
            cout << "   4. Strategy_defensive"<<endl;
            cout << "   5. strategy_offensive"<<endl;
            cout << "   6. kickAtGoal" << endl;
            cout << "   7. Strategy_offensive2"<<endl;
            cout << "	8. Monitor positions" << endl;
            resetTVariables();
            cin >> answer;
            break;
        }
        usleep(10000);
    }
}

// Use this function for single actions only. No strategies or tactics. It won't work.
void Master::manual() {
    int answer;
    int robot;
    double posX, posY, speed;
    while(1) {
        updateSide();
        updatePositions();
        cout << "Choose an action" << endl;
        cout << "	0. EXIT" << endl;
        cout << "	1. ACTION_GOTO" << endl;
        cout << "	2. BEFORE_KICK" << endl;
        cout << "	3. KICK" << endl;
        cout << "	4. DEFEND" << endl;
        cout << "	5. PASS" << endl;
        cout << "	6. LISTEN TO REFEREE" << endl;
        cout << "	7. TEST BALL CLASS" << endl;

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
            send(Command(ACTION_GOTO, Position(posX, posY), speed), robot);
            break;
        case 2:
            send(Command(ACTION_BEFORE_KICK, ball.GetPos(), Position(1.2, 0),0.4), robot);
            break;
        case 3:
            cout << "Speed: ";
            cin >> speed;
            send(Command(ACTION_KICK, Position(1.27, 0), speed, 1), robot);
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
    s_state = BALANCED;
    s_case = INIT;
    t_state = STEP1;
    tacticDone = false;
    cps_state = 1;
    closestRobo = 0;
    maxDistance = 10;
    chrossandpassy = 0; // used for tactics: Chross and Pass
    robonr = 0; // used for tactics: near_penalty
}




