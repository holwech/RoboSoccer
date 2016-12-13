#include "test_player.h"


Test_player::Test_player(Master& master) : master(&master), player(&master.positions, &master.ball, &master.channels[0], &master.robo0) {
}

void Test_player::run(){
    while(1) {
        cout << "----- ----- ----- -----" << endl;
        cout << "Choose test program: " << endl;
        cout << "0: Go back" << endl;
        cout << "1: Print to screen" << endl;
        cout << "2: Simple thread test" << endl;
        cout << "3: Goalkeeper defending" <<endl;
        cout << "4: Test before kick" << endl;
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
            testThreads2();
            break;
        case 3:
            cout << "Program 3 running..." <<endl;
            goalkeeperingame();
            break;
        case 4:
            cout << "Program 4 running" << endl;
            testBeforeKick();
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


void Test_player::testThreads2() {
    thread threadRobo0(&Player::run, std::ref(player));
    Command command1(ACTION_GOTO, Position(0.5, 0.5));
    Command command2(ACTION_GOTO, Position(-0.5, 0.5));
    cout << "Sending command 1" << endl;
    master->send(command1, 0);
    while(player.getState() != IDLE) {  }
    cout << "Sending command 2" << endl;
    master->send(command2, 0);
    while(1);
    cout << "DONE" << endl;
    threadRobo0.join();
}

void Test_player::testThreads() {
    thread threadRobo0(&Player::run, std::ref(player));
    Command command1(ACTION_GOTO, Position(0.5, 0.5));
    Command command2(ACTION_GOTO, Position(-0.5, 0.5));
    Command command3(ACTION_GOTO, Position(-0.5, -0.5));
    Command command4(ACTION_GOTO, Position(0.5, -0.5));
    int step = 1;
    master->send(command1, 0);
    usleep(10000);
    while(player.getState() != IDLE) {

    }
    master->send(command2, 0);
    cout << "DONE!" << endl;
    while(1) {}
    while(1) {
        master->updatePositions();
        if (player.getState() == IDLE) {
            switch(step) {
            case 1:
                cout << "Sending 1" << endl;
                master->send(command1, 0);
                step++;
                break;
            case 2:
                cout << "Sending 2" << endl;
                master->send(command2, 0);
                step++;
                break;
            case 3:
                cout << "Sending 3" << endl;
                master->send(command3, 0);
                step++;
                break;
            case 4:
                cout << "Sending 4" << endl;
                master->send(command4, 0);
                step = 1;
                break;
            default:
                cout << "Switch in testThreads is fucked up" << endl;
            }
        }
    }
    threadRobo0.join();
}

void Test_player::goalkeeperingame(){
    thread threadRobo0(&Player::run, std::ref(player));
    Command command1(ACTION_DEFEND, Position(0.5, 0.5));
    master->send(command1, 0);
    while(1);
    cout << "DONE" << endl;
    threadRobo0.join();
}

void Test_player::testBeforeKick(){
    thread threadRobo0(&Player::run, std::ref(player));
    Command command1(ACTION_BEFORE_KICK, master->ball.GetPos(), Position(1.27, -0.5));
    master->send(command1, 0);
    cout << "DONE" << endl;
    threadRobo0.join();
}
