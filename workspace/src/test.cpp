#include "test.h"

Test::Test(Master& master) : master(&master)
{
}

void Test::menu(){
    while(1) {
        cout << "----- ----- ----- -----" << endl;
        cout << "Choose part to test: " << endl;
        cout << "0: Exit program" << endl;
        int program;
        cin >> program;
        bool stop = false;
        switch(program) {
        case 0:
            stop = true;
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
