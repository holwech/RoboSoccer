#include "test_player.h"


Test_player::Test_player(Master& master) : master(master), player(&master.positions, &master.ballPos, &master.channels[0], &master.robo0) {
}

void Test_player::run(){
    while(1) {
        cout << "----- ----- ----- -----" << endl;
        cout << "Choose test program: " << endl;
        cout << "0: Go back" << endl;
        cout << "1: Print to screen" << endl;
        cout << "2: Simple thread test" << endl;
        int program;
        cin >> program;
        bool stop = false;

        switch(program) {
        case 0:
            stop = true;
            break;
        case 1:
            cout << "Program 1 running..." << endl;
            break;
        case 2:
            cout << "Program 2 running..." << endl;
            testThreads();
            break;
        default:
            stop = true;
            break;
        }
        if (stop) {
            break;
        }
    }
    cout << "Leaving test menu" << endl;
}


void Test_player::testThreads() {
    thread threadRobo0(&Player::run, player);
    Command command1(ACTION_GOTO, Position(0.5, 0.5));
    Command command2(ACTION_GOTO, Position(-0.5, 0.5));
    Command command3(ACTION_GOTO, Position(-0.5, -0.5));
    Command command4(ACTION_GOTO, Position(0.5, -0.5));
    int step = 1;
    while(1) {
        master.updatePositions();
        if (player.getState() == IDLE) {
            switch(step) {
            case 1:
                master.send(command1, 0);
                step++;
                break;
            case 2:
                master.send(command2, 0);
                step++;
            case 3:
                master.send(command3, 0);
                step++;
            case 4:
                master.send(command4, 0);
                step = 1;
            }
        }
    }
    threadRobo0.join();
}
