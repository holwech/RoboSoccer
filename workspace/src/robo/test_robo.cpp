#include "test_robo.h"
#include "robo.h"
void Test_robo::run(){
    while(1) {
        cout << "Choose test program: " << endl;
        cout << "0: Return to program" << endl;
        cout << "1: test driveWithCA()" << endl;
        cout << "2: test goalieDrive()" << endl;
        int program;
        cin >> program;
        bool stop = false;

        switch(program) {
        case 0:
            stop = true;
            break;
        case 1:
            cout << "Program 1 running..." << endl;
            testDriveWithCA();
            break;
        case 2:
            cout << "Program 2 running..." << endl;
            testGoalieDrive();
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

void Test_robo::testDriveWithCA(){
    cout << "Reached function testDriveWithCA()..." << endl;
    Position posA(0.5, 0.0);
    Position posB(-0.5, 0.0);
    Robo &testrobo = master.robo[0];
    testrobo.GotoPos(posA);
    while(1){
        testrobo.driveWithCA();
        usleep(10000);
        if (testrobo.GetPos().DistanceTo(posA) < 0.1){
            testrobo.GotoPos(posB);
        }
        else if(testrobo.GetPos().DistanceTo(posB) < 0.1){
            testrobo.GotoPos(posA);
        }

    }
}

void Test_robo::testGoalieDrive(){
    cout << "Reached function testDriveWithCA()..." << endl;
    Position posA(0.5, 0.0);
    Position posB(-0.5, 0.0);
    Robo &testrobo = master.robo[0];
    testrobo.GotoPos(posA);
    while(1){
        testrobo.goalieDrive();
        usleep(10000);
        if (testrobo.GetPos().DistanceTo(posA) < 0.1){
            testrobo.GotoPos(posB);
        }
        else if(testrobo.GetPos().DistanceTo(posB) < 0.1){
            testrobo.GotoPos(posA);
        }

    }
}

