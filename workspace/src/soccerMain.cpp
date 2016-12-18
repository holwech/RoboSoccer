//============================================================================
// Name        : soccer_client.cpp
// Author      :
// Version     :
// Copyright   : Your copyright notice
// Description : Client for RTDB which controls team 1, Ansi-style
//============================================================================


#include <iostream>
#include <vector>
#include "kogmo_rtdb.hxx"
#include "master/master.h"
#include "test.h"

using namespace std;

int main(void) {
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

    string teamColorAnswer;
    bool teamChosen = false;
    while (!teamChosen) {
        cout << "Which team do you want to control? (b/r) ";
        cin >> teamColorAnswer;
        if (teamColorAnswer == "b" ) {
            cout << "Initiating for blue" << endl;
            teamChosen = true;
        } else if (teamColorAnswer == "r") {
            cout << "Initiating for red" << endl;
            teamChosen = true;
        } else{
            cout << "-b- (blue) or -r- (red) not specified." << endl;
        }
    }


    try {

        /** Establish connection to the RTDB. */
        cout << endl << "Connecting to RTDB..." << endl;
        /** Create the client name with the unique client number*/
        string client_name = "pololu_client_";
        client_name.push_back((char) (client_nr + '0'));
        RTDBConn DBC(client_name.data(), 0.1, "");

        /** Init robot(s) */
        vector<int> rfNumber;
        if (teamColorAnswer == "b") {
            rfNumber.push_back(0);
            rfNumber.push_back(1);
            rfNumber.push_back(2);
            rfNumber.push_back(3);
            rfNumber.push_back(4);
            rfNumber.push_back(5);
        } else {
            rfNumber.push_back(3);
            rfNumber.push_back(4);
            rfNumber.push_back(5);
            rfNumber.push_back(0);
            rfNumber.push_back(1);
            rfNumber.push_back(2);
        }

        // Give objects time to initialize or something.
        // Don't know why this fixes things, but it does.


        Master master(teamColorAnswer, DBC, rfNumber);
        cout << "Master ready..." << endl;

        cout << "----- ----- ----- -----" << endl;
        cout << "Enter test mode y? (y/write anything to start actual program) ";
        string testMode;
        cin >> testMode;

        if (testMode == "y") {
            Test test(master);
            cout << "Test program starting..." << endl;
            test.menu();
        } else {
            cout << "Main program starting..." << endl;
            master.run();
        }
        cout << "Exiting..." << endl;

    } catch (DBError err) {
            cout << "Client died on Error: " << err.what() << endl;
    }


    return 0;
}


