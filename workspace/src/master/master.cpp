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
    side = RIGHT_SIDE;
    state = REFEREE_INIT;
    referee.Init();

    /** Init tactic variables */
    t_state = STEP1;
    tacticDone = false;
    cps_state = 1;
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

/** Add your strategies or tactics here. (Yes, I know, misleading function name) */
void Master::strategies() {
    int answer;
    cout << "Select one of the following strategies/tactics: ";
    cout << "	1. exampleTactic" << endl;
    cin >> answer;
    while(1) {
        updatePositions();
        switch(answer) {
        case 1:
            tacticDone = crossPassAndShoot();
            if (tacticDone) { answer = -1; }
            break;
        default:
            cout << "No case for this state yet (in strategies function), or tactic terminated" << endl;
            cout << "Select one of the following strategies/tactics: ";
            cout << "	1. exampleTactic" << endl;
            cin >> answer;
            break;
        }
    }
}

// Use this function for single actions only. No strategies or tactics. It won't work.
void Master::manual() {
    int answer;
    int robot;
    double posX, posY, speed;
    while(1) {
        cout << "Choose an action" << endl;
        cout << "	0. EXIT" << endl;
        cout << "	1. ACTION_GOTO" << endl;
        cout << "	2. BEFORE_KICK" << endl;
        cout << "	3. KICK" << endl;
        cout << "	4. DEFEND" << endl;
        cout << "	5. PASS" << endl;
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
            send(Command(ACTION_BEFORE_KICK, ball.GetPos(), Position(1.27, 0)), robot);
            break;
        case 3:
            /**cout << "x: ";
            cin >> posX;
            cout << "y: ";
            cin >> posY;*/

            send(Command(ACTION_KICK, Position(1.27, 0)), robot);
            break;
        case 4:
            send(Command(ACTION_DEFEND), robot);
            break;
        case 5:
            send(Command(ACTION_PASS, positions[2]),robot);
            break;            
        default:
            cout << "No action created for this choice yet in master.manual" << endl;
            break;
        }

    }
}

void Master::resetTVariables() {
    t_state = STEP1;
    tacticDone = false;
    cps_state = 1;
}




