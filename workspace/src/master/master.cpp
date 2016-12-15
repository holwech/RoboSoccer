#include "master.h"
Master::Master(string& team, RTDBConn& DBC, RawBall& ball, Referee& referee, vector<Robo>& robo, vector<int>& rfNumbers) :
                team(team), DBC(DBC), ball(ball), referee(referee), robo(robo), positions(6), debugTimer() {
    usleep(10000);
    cout << "Setting master variables..." << endl;
    side = RIGHT_SIDE;
    state = REFEREE_INIT;

    usleep(1000);

    for (int i = 0; i <= 5; i++) {
        Command newCommand(ACTION_IDLE);
        channels.push_back(Channel(newCommand));
    }
    cout << "Channels initialized..." << endl;

    updatePositions();
}



void Master::run() {

    cout << "Starting state machine..." << endl;
    while(1) {
        /** Timer for printing info about the system, so that it doesn't spam the
          * terminal.
          */
        state = referee.GetPlayMode();
        switch(state) {
        case REFEREE_INIT:
            break;
        case BEFORE_KICK_OFF:
            break;
        case KICK_OFF:
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
            cout << "No case for state." << state << endl;
            break;
        }
    }
}

vector<Position> Master::teamPositions() {
    return vector<Position>(positions[0], positions[1]);
}

vector<Position> Master::otherTeamPositions() {
    return vector<Position>(positions[2], positions[1]);
}

void Master::send(Command command, int roboNum) {
    channels[roboNum].write(command);
}

void Master::updatePositions() {
    positions[0] = robo[0].GetPos();
    positions[1] = robo[1].GetPos();
    positions[2] = robo[2].GetPos();
    positions[3] = robo[3].GetPos();
    positions[4] = robo[4].GetPos();
    positions[5] = robo[5].GetPos();
    ballPos = ball.GetPos();
}
