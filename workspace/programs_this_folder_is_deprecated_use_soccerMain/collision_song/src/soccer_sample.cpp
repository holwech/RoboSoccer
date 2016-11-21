//============================================================================
// Name        : soccer_client_1.cpp
// Author      :
// Version     :
// Copyright   : Your copyright notice
// Description : Client for RTDB which controls team 1, Ansi-style
//============================================================================


#include <time.h>
#include <iostream>
#include "kogmo_rtdb.hxx"
#include "robo_control.h"

using namespace std;


int main(void) {
	//--------------------------------- Init --------------------------------------------------

	/** Use client number according to your account number!
	 *
	 *	This is necessary in order to assure that there are unique
	 *	connections to the RTDB.
	 *
	 */
        const int client_nr = 12;

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
        //int rfcomm_nr = 1;
      			
        /*

	// uncomment this if you want to select manually your robot
	// key input not available from QtCreator console, only from linux console.

	cout << "Specify rfcomm number of the robot you want to move: ";
	while (1) {
		cin >> rfcomm_nr;
		if (!((rfcomm_nr >= 0) && (rfcomm_nr <= 7))) {
			cout << "Please specify valid rfcomm number between 0 and 7"
					<< endl;
		} else {
			break;
		}
        }
        */

	try {

		/** Establish connection to the RTDB.
		 *
		 *  The connection to the RTDB is necessary in order to get access
		 *  to the control and the status of the robots which are both stored
		 *  in the RTDB.
		 *
		 *  In the RTDB there are also informations about the ball and the
		 *  other robot positions.
		 *
		 */
		cout << endl << "Connecting to RTDB..." << endl;
		/** Create the client name with the unique client number*/
		string client_name = "pololu_client_";
		client_name.push_back((char) (client_nr + '0'));
		RTDBConn DBC(client_name.data(), 0.1, "");

		/** Create a new RoboControl object.
		 *
		 *  This is the basis for any communication with the robot.
		 *
		 *  We need to hand over the RTDB connection (DBC) and the rfcomm
		 *  number of the robot we want to control.
		 */
                RoboControl robo(DBC, 0);
                RoboControl robo1(DBC, 1);
                RoboControl robo2(DBC, 2);
                RoboControl robo3(DBC, 3);
                RoboControl robo4(DBC, 4);
                RoboControl robo5(DBC, 5);

		/** Now let's print out some information about the robot... */
                //uint8_t mac[6];
                //robo.GetMac(mac);
                //cout << "Robo @ rfcomm" << rfcomm_nr << " with Mac: ";
                //for (int j = 0; j < 5; j++)
        //		cout << hex << (int) mac[j] << ":";
        //	cout << hex << (int) mac[5] << endl;

                //cout << "\t accuvoltage: " << dec << (int) robo.GetAccuVoltage()
                //		<< "mV" << endl;
		cout << "\t initial position: " << robo.GetPos() << endl;
		cout << "\t initial rotation: " << robo.GetPhi() << endl;

		/** Create a ball object
		 *
		 *  This ball abject gives you access to all information about the ball
		 *  which is extracted from the cam.
		 *
		 */
		RawBall ball(DBC);
		/** lets print this information: */
		cout << "Ball informations:" << endl;
		cout << "\t initial position: " << ball.GetPos() << endl;
		/** Notice that the rotation here refers to the moving direction of the ball.
		 *  Therefore if the ball does not move the rotation is not defined.
		 */
		cout << "\t initial direction: " << ball.GetPhi() << endl;
		cout << "\t initial velocity: " << ball.GetVelocity() << endl;

		//-------------------------------------- Ende Init ---------------------------------

		/** Define four positions which form a rectangle...
		 *
		 */
                Position pos1(-1, -0.7);
                Position pos2(1, -0.7);
                Position pos3(1, 0.7);
                Position pos4(-1, 0.7);

//                robo.MoveDist(1);
//                robo.MoveDist(-1);
//                robo.MoveMsBlocking(100,-100,3000);

 /*               double DistToPos=0;
                double robospeed;
                double lastDistToPos=0;
                const double low_speed = 0.04;
*/
    //            robo.CruisetoXY(0,0,160,false);
    //            robo3.GotoXY(0,0,160,true);
    //            robo.MoveMs(100,-100,3000);
//robo1.GotoXY(1,0,160,true);
/*
                uint32_t tt;
                robo4.MoveMs(-100,100,3000);
                robo4.MoveMs(100,+100,2000);

                robo4.GotoXY(1,0,160,true);

                tt=robo4.GetMsToGo();

                cout<<tt<<endl;
*/
// robo5.GotoPos(pos3);
//                double a;
/*
 while(1)
                {
                    robo5.GotoXY(-1,0.7,160,true);
 //                   robo5.GotoPos(pos3);
 //                   robo4.MoveMs(101,100,1);
 //                    robo5.GotoPos(pos3);
                }
*/
                while(1)
                {
                    robo4.GotoPos(pos2);
                    robo5.GotoPos(pos4);
                    while(robo4.GetPos().DistanceTo(pos2)>0.1&&robo5.GetPos().DistanceTo(pos4)>0.1);
                    usleep(50000);
                    robo4.GotoPos(pos1);
                    robo5.GotoPos(pos3);
                    while(robo4.GetPos().DistanceTo(pos1)>0.1&&robo5.GetPos().DistanceTo(pos3)>0.1);
                    usleep(50000);
                }
/*
       while(1)
{
           while(1)
           {
    robo4.GotoPos(pos1);
    usleep(1000);
    robo4.MoveMs(101,100,3);
    usleep(1000);
    robo5.GotoPos(pos3);
    usleep(1000);
    robo5.MoveMs(100,100,3);
    if(robo4.GetPos().DistanceTo(pos1)<0.1&&robo5.GetPos().DistanceTo(pos3)<0.1)
       {
//        cout<< 'break'<<endl;
        break;
    }

}
           while(1)
           {
    robo4.GotoPos(pos2);
    usleep(1000);
    robo4.MoveMs(101,100,3);
    usleep(1000);
    robo5.GotoPos(pos4);
    usleep(1000);
    robo5.MoveMs(100,100,3);
    usleep(1000);
    if(robo4.GetPos().DistanceTo(pos2)<0.1&&robo5.GetPos().DistanceTo(pos4)<0.1)
        break;
}
}
*/
/*    if(robo4.GetPos().DistanceTo(pos1)<0.1)
    {
        robo4.StopAction();

    }
    */
 //   robo.GotoXY(-1,0,160,false);
//    robo.



/*
                while (1) {

       //             robo.CruisetoXY(0,0,160,false);
      //              robo.GotoXY(0,0,160,true);
 //                   robo.MoveMs(-100,-100,3000);
 //                   robo1.MoveMs(100,100,100);


                        cout << "Moving to 1: " << pos1 << endl;
                        robo3.GotoXY(pos1.GetX(), pos1.GetY(), 160, true);

                        do
                        {
                            DistToPos = robo3.GetPos().DistanceTo(pos1);
                            robospeed = robospeed*0.8+(lastDistToPos-DistToPos)*0.2;
                            lastDistToPos = DistToPos;
                            usleep(100000);
                            
                        } while ((DistToPos > 0.15)&&(robospeed>low_speed));
                        
                        if (robospeed<=low_speed)
                        {
                            robo3.MoveMs(-100,-100,1000);
                            sleep(1);
                        }

                        cout << "robospeed= " << robospeed << endl << endl;
                        cout << "Pos= [" << robo3.GetX() << ";"<< robo3.GetY()<<"]"<< endl << endl;

                        robospeed=1;
                        lastDistToPos=0;

                        cout << "Moving to 2: " << pos2 << endl << endl;
                        robo3.GotoXY(pos2.GetX(), pos2.GetY(), 160, true);

                        do
                        {
                            DistToPos = robo3.GetPos().DistanceTo(pos2);
                            robospeed = robospeed*0.8+(lastDistToPos-DistToPos)*0.2;
                            lastDistToPos = DistToPos;
                            usleep(100000);
                        } while ((DistToPos >  0.15)&&(robospeed>low_speed));
                        
                        if (robospeed<=low_speed)
                        {
                            robo3.MoveMs(-100,-100,1000);
                        }
                        
                        cout << "robospeed= " << robospeed << endl << endl;
                        cout << "Pos= [" << robo3.GetX() << ";"<< robo3.GetY()<<"]"<< endl << endl;

                        robospeed=1;
                        lastDistToPos=0;

                        cout << "Moving to 3: " << pos3 << endl << endl;
                        robo3.GotoXY(pos3.GetX(), pos3.GetY(), 160, true);

                        do
                        {
                            DistToPos = robo3.GetPos().DistanceTo(pos3);
                            robospeed = robospeed*0.8+(lastDistToPos-DistToPos)*0.2;
                            lastDistToPos = DistToPos;
                            usleep(100000);
                        } while ((DistToPos >  0.15)&&(robospeed>low_speed));
                        if (robospeed<=low_speed)
                        {
                            robo3.MoveMs(-100,-100,1000);
                            sleep(1);
                        }
                        cout << "robospeed= " << robospeed << endl << endl;
                        cout << "Pos= [" << robo3.GetX() << ";"<< robo3.GetY()<<"]"<< endl << endl;

                        robospeed=1;
                        lastDistToPos=0;


                        cout << "Moving to 4: " << pos4 << endl << endl;
                        robo3.GotoXY(pos4.GetX(), pos4.GetY(), 160, true);
                        do
                        {
                            DistToPos = robo3.GetPos().DistanceTo(pos4);
                            robospeed = robospeed*0.8+(lastDistToPos-DistToPos)*0.2;
                            lastDistToPos = DistToPos;
                            usleep(100000);
                        } while ((DistToPos >  0.15)&&(robospeed>low_speed));
                        if (robospeed<=low_speed)
                        {
                            robo3.MoveMs(-100,-100,1000);
                            sleep(1);
                        }

                        cout << "robospeed= " << robospeed << endl << endl;
                        cout << "Pos= [" << robo3.GetX() << ";"<< robo3.GetY()<<"]"<< endl << endl;

                        robospeed=1;
                        lastDistToPos=0;

       }
*/

        }catch (DBError err) {
		cout << "Client died on Error: " << err.what() << endl;
	}
	cout << "ende" << endl;
	return 0;
}

