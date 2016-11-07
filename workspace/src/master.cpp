#include "master.h"

Master::Master()
{
}

void init() {
    /** Use client number according to your account number!
     *
     *	This is necessary in order to assure that there are unique
     *	connections to the RTDB.
     *
     */

    int client_nr;

    cout << endl << "Please write your client number: ";
    cin >> client_nr;

    /** Type in the rfcomm number of the robot you want to connect to.
     *  The numbers of the robots you are connected to can be found on the
     *  screen when you connected to them.
     *
     *  The red robots' number will be in the range of 3 to 5
     *  The blue robots' number will be in the range from 0 to 2
     *
     *  The robots are always connected to the lowest free rfcomm device.
     *  Therefore if you have two blue robots connected the will be
     *  connected to rfcomm number 0 and number 1...
     *
     */

    string answer;
    bool cont = false;
    while (!cont) {
        cout << "Which team do you want to control? (blue/red) ";
        cin >> answer;
        if (answer == "blue" || answer == "red") {
            cont = true;
            cout << "Initiating for " << answer << endl;
        }
    }
}
