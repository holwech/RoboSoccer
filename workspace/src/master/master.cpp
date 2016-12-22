#include "master.h"
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
}



void Master::run() {
    thread threadRobo0(&Player::run, std::ref(player[0]));
    thread threadRobo1(&Player::run, std::ref(player[1]));
    thread threadRobo2(&Player::run, std::ref(player[2]));
    usleep(1000);
    string answer;
    cout << "Enter manual mode? (y/n) ";
    cin >> answer;
    state = referee.GetPlayMode();
    if(answer == "y") {
        manual();
    }

    cout << "Starting state machine..." << endl;
    while (1) {
       exampleTactic();
    }
    /* The referee getPlayMode is not working right now for some reason
     * Therefore the state machine has been replaced by a simple for loop for now
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
    */
    threadRobo0.join();
    threadRobo1.join();
    threadRobo2.join();
}

void Master::exampleTactic() {
    Position target = Position(1.29, 0.0);
    if (player[0].getState() == IDLE && !player[0].isBusy()) {
        send(Command(ACTION_BEFORE_KICK, ball.GetPos(), target), 0);
    } else if (player[0].getPrevState() == BEFORE_KICK && !player[0].isBusy()){
        send(Command(ACTION_KICK, target), 0);
    } else if (player[0].getPrevState() == KICK && !player[0].isBusy()) {
        send(Command(ACTION_GOTO, Position(-1.0, -0.5)), 0);
    }
    cout << player[0].getState() << endl;
}


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
        cout << "	5. DEMO STATEGY" << endl;
        cout << "	6. PASS" << endl;
        cout << "	7. POSITION_AND_PASS" << endl;
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
            strategy_defensive();
            break;
        case 6:
            send(Command(ACTION_PASS, positions[2]),robot);
            break;            
        case 7:
            exampleTactic();
            break;
        default:
            cout << "No action created for this choice yet in master.manual" << endl;
            break;
        }
        usleep(1000);

    }
}

void strategy_offensive(){

}
void Master::strategy_defensive(){
    //send(Command(ACTION_DEFEND), 0);
    //send(Command(ACTION_BLOCK_BALL), 1);
    send(Command(ACTION_BEFORE_KICK), 1);
}

void Master::strategy_demo(){
    /*
     * if ahead on score:
     * 		do strategy_defensive();
     * else:
     * 		do strategy_offensive();
     */
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
