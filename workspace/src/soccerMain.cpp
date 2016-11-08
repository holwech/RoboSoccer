//============================================================================
// Name        : soccer_client.cpp
// Author      :
// Version     :
// Copyright   : Your copyright notice
// Description : Client for RTDB which controls team 1, Ansi-style
//============================================================================


#include <time.h>
#include <iostream>
#include <vector>
#include "kogmo_rtdb.hxx"
#include "robo_control.h"
#include "master.h"
#include "referee.h"

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
        cout << "Which team do you want to control? (blue/red) ";
        cin >> teamColorAnswer;
        if (teamColorAnswer == "blue" || teamColorAnswer == "red") {
            teamChosen = true;
            cout << "Initiating for " << teamColorAnswer << endl;
        }
        else{
            cout << "-blue- or -red- not specified." << endl;
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
        int rfNumber0, rfNumber1, rfNumber2;
        if (teamColorAnswer == "blue") {
            rfNumber0 = 0;
            rfNumber1 = 1;
            rfNumber2 = 2;
        } else {
            rfNumber0 = 3;
            rfNumber1 = 4;
            rfNumber2 = 5;
        }

        RoboControl robo0(DBC, rfNumber0);
        RoboControl robo1(DBC, rfNumber1);
        RoboControl robo2(DBC, rfNumber2);
        cout << "Robots are ready..." << endl;


        /** Create a ball object */
        RawBall ball(DBC);
        cout << "Ball ready..." << endl;


        Referee referee(DBC);
        cout << "Referee ready..." << endl;

        Master master(teamColorAnswer, DBC, robo0, robo1, robo2, ball, referee);
        cout << "Master ready..." << endl;
        cout << "Program starting..." << endl;

        master.run();



    } catch (DBError err) {
            cout << "Client died on Error: " << err.what() << endl;
    }

    cout << "Exiting..." << endl;
    return 0;
}


