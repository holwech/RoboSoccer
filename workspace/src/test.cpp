#include "test.h"

Test::Test(Master& master) : master(&master), ta(master), tr(master)
{
}

void Test::menu(){
    while(1) {
        cout << "----- ----- ----- -----" << endl;
        cout << "Choose part to test: " << endl;
        cout << "0: Exit program" << endl;
        cout << "1: Run player test menu" << endl;
        cout << "2: Run robo test menu" << endl;
        int program;
        cin >> program;
        bool stop = false;
        switch(program) {
        case 0:
            stop = true;
            break;
        case 1:
            ta.run();
            break;
        case 2:
            tr.run();
            break;
        default:
            stop = true;
            break;
        }
        if (stop) {
            break;
        }
    }
    cout << "leaving test menu" << endl;
}
