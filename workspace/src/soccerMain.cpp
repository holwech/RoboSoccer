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
#include "robo/robo.h"
#include "master/master.h"
#include "referee.h"
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
        int rfNumber0, rfNumber1, rfNumber2, rfNumber3, rfNumber4, rfNumber5;
        if (teamColorAnswer == "b") {
            rfNumber0 = 0;
            rfNumber1 = 1;
            rfNumber2 = 2;
            rfNumber3 = 3;
            rfNumber4 = 4;
            rfNumber5 = 5;
        } else {
            rfNumber0 = 3;
            rfNumber1 = 4;
            rfNumber2 = 5;
            rfNumber3 = 0;
            rfNumber4 = 1;
            rfNumber5 = 2;
        }
        Robo robo0(DBC, rfNumber0);
        Robo robo1(DBC, rfNumber1);
        Robo robo2(DBC, rfNumber2);
        Robo robo3(DBC, rfNumber3);
        Robo robo4(DBC, rfNumber4);
        Robo robo5(DBC, rfNumber5);
        cout << "Robots are ready..." << endl;

        robo0.setVariables(robo1, robo2, robo3, robo4, robo5);
        robo1.setVariables(robo0, robo2, robo3, robo4, robo5);
        robo2.setVariables(robo0, robo1, robo3, robo4, robo5);
        //following only needed for milestone 2
        robo3.setVariables(robo4, robo5, robo0, robo1, robo2);
        robo4.setVariables(robo3, robo5, robo0, robo1, robo2);
        robo5.setVariables(robo3, robo4, robo0, robo1, robo2);
        /** Create a ball object */
        RawBall ball(DBC);
        cout << "Ball ready..." << endl;


        Referee referee(DBC);
        referee.Init();
        cout << "Referee ready..." << endl; 

        // Give objects time to initialize or something.
        // Don't know why this fixes things, but it does.
        usleep(1000);
        Master master(teamColorAnswer, DBC, robo0, robo1, robo2, robo3, robo4, robo5, ball, referee);
        cout << "Master ready..." << endl;

        cout << "----- ----- ----- -----" << endl;
        cout << "Enter test mode? (y/write anything to skip) ";
        string testMode;
        cin >> testMode;

        if (testMode == "y") {
            Test test(master);
            cout << "Test program starting..." << endl;
            test.testMenu();
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


