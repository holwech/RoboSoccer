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
#include <math.h>
#include <cmath>
using namespace std;

int main(void) {
	//--------------------------------- Init --------------------------------------------------

	/** Use client number according to your account number!
	 *
	 *	This is necessary in order to assure that there are unique
	 *	connections to the RTDB.
	 *
	 */
        const int client_nr = 9;

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
                Position pos1(0,0);
                Position pos2(1, -0.7);
                Position pos3(1, 0.7);

                /*double DistToPos=0;
                double robospeed;
                double lastDistToPos=0;

                const double low_speed = 0.04;*/
                double t=0;
                double speed=180;
                Angle driveang;
                int clear=1;
                double mover=0.3;
                //Angle avoidang=3.1415926/12;
                //nowdest is the destnition of current time step
                Position nowdest;
                for (t=0;t<10;t=t+0.05){
                    cout<<t<<endl;
                    nowdest=pos1;
                    driveang=atan((nowdest.GetY()-robo.GetPos().GetY())/(nowdest.GetX()-robo.GetPos().GetX()))/3.1415926*180;
                    while((driveang.Deg()-robo.GetPhi().Deg()>20||driveang.Deg()-robo.GetPhi().Deg()<-20)&&clear==1){
                         robo.Turn((driveang.Deg()-robo.GetPhi().Deg()));
                         usleep(100000);

                         //cout<<driveang.Deg()<<"driveang"<<endl;
                         //cout<<robo.GetPhi().Deg()<<"robo"<<endl;
                         cout<<driveang.Deg()-robo.GetPhi().Deg()<<"diff"<<endl;
                    }
                    robo.MoveMs(speed+3,speed,140,140);
                    if ((robo.GetX()-robo3.GetX())*(robo.GetX()-robo3.GetX())+(robo.GetY()-robo3.GetY())*(robo.GetY()-robo3.GetY())<mover*mover){
                        robo.MoveMs(-180,-180,200,200);
                        usleep(100000);
                    }
                    for (int i=5;i<21;i++){
                        if (((robo.GetPos().GetX()+0.05*cos(driveang.Rad())-robo3.GetPos().GetX())*(robo.GetPos().GetX()+0.05*cos(driveang.Rad())-robo3.GetPos().GetX())+(robo.GetPos().GetY()+0.05*sin(driveang.Rad())-robo3.GetPos().GetY())*(robo.GetPos().GetY()+0.05*sin(driveang.Rad())-robo3.GetPos().GetY()))<mover) {
                        clear=0;
                                ;}
                    }
                    cout<<clear<<endl;
                    if ((clear==0)) {
                        //clear=1;
                        cout<<"changing direct"<<endl;
                        //j=0;
                        //driveang=driveang+avoidang;
                        //for (int i=1;i<21;i++)
                        //{if (((robo.GetPos().GetX()+0.05*cos(driveang.Rad())-robo3.GetPos().GetX())*(robo.GetPos().GetX()+0.05*cos(driveang.Rad())-robo3.GetPos().GetX())+(robo.GetPos().GetY()+0.05*sin(driveang.Rad())-robo3.GetPos().GetY())*(robo.GetPos().GetY()+0.05*sin(driveang.Rad())-robo3.GetPos().GetY()))<mover*mover) {clear=0;}
                        //;
                        //}
                        //j=1;
                        cout<<driveang<<endl;
                        cout<<clear<<endl;
                        clear=1;
                        robo.Turn(30);
                        usleep(100000);
                        robo.MoveMs(speed+3,speed,140,140);
                    }



                    usleep(100000);
                    cout<<"move"<<endl;

                }


/*		while (1) {

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

	} catch (DBError err) {
		cout << "Client died on Error: " << err.what() << endl;
	}
	cout << "ende" << endl;
	return 0;
}

