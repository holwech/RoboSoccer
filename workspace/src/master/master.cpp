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
    usleep(10000);
    side = RIGHT_SIDE;
    state = REFEREE_INIT;


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

void Master::send(Command command, int roboNum) {
    channel[roboNum].write(command);
}

void Master::updatePositions() {
    positions[0] = player[0].getPos();
    positions[1] = player[1].getPos();
    positions[2] = player[2].getPos();
    positions[3] = player[3].getPos();
    positions[4] = player[4].getPos();
    positions[5] = player[5].getPos();
}
