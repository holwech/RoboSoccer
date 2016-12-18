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


}



void Master::run() {

    cout << "Starting state machine..." << endl;
    while(1) {
        updatePositions();
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

void Master::send(Command command, int roboNum) {
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
