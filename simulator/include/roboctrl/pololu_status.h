#ifndef POLOLU_STATUS_H_
#define POLOLU_STATUS_H_


#include <iostream>
#include <unistd.h>		//für usleep (µSecounds) -> schläft die übergeben Anzahl von µSekunden
#include <deque>
#include <string>
#include <math.h>

//kogmo_rtdb includes
#include "kogmo_rtdb_obj_base_classes.hxx"
#include "kogmo_rtdb_obj_typeids.h"
#include "kogmo_rtdb_obj_base.h"

//#include "pololu_control.h"
#include "share.h"
#include "types.h"

#include "angle.h"
#include "position.h"

//-------------------------------------------------- class -----------------------------------------------------
using namespace std;
using namespace KogniMobil;

/** Class for the exchange of the status of the robots via the RTDB
 * The PololuStatus class holds all informations about the status of the robot.
 * To get the most recent status-information you need to update the PololuStatus object
 * by reading the current status from the RTDB.
 * The status of the robot (in the RTDB) is always held up to date by the
 * pololu_rtdb_module which writes into the RTDB.
 *
 * So the steps for retrieving the most recent status information is the following:
 * - Read the current status from the RTDB (.RTDBRead())
 * - Then the PololuStatus object contains this information and you can access all of them
 *  with the Get*-methodes
 *
 * However it should not be necessary to do this on your own, as the PololuControl
 * class does this for you for all necessary information you need.
 * Thus it is recommended to use the PololuControl or the RoboControl class
 * (which inherits from PololuControl) to get stauts information from the robot!
 *
 *  @author Julian Straub
 *  @date 31.08.2009
 *
 */
class PololuStatus : public RTDBObj
{
protected:
	/** Struct which holds all information that is stored in the RDTB. */
	kogmo_rtdb_subobj_pololu_status_t *obj_pololu_status;
public:

	/** Initializes the status information. */
	PololuStatus(class RTDBConn& DBC, const char* name = "", const int& otype =
			KOGMO_RTDB_OBJTYPE_POLOLU, const int32_t& child_size = 0,
			char** child_dataptr = NULL);

	// reading access to the status variables
	void GetMac(uint8_t (&mac)[6]) 	{ for(int i = 0; i < 6; i++) mac[i] = obj_pololu_status->mac[i];};
	uint8_t GetDistanceSensors(void) 				{ return obj_pololu_status->distanceSensors;};
	uint32_t GetAccuVoltage(void) 					{ return obj_pololu_status->accuVoltage;};
	eMovingStatus GetMovingStatus(void)				{ return obj_pololu_status->movingStatus;};
	eLocalizationStatus GetLocalizationStatus(void)	{ return obj_pololu_status->localizationStatus;};
	eModuleStatus GetModuleStatus(void)				{ return obj_pololu_status->moduleStatus;};
	int32_t GetSpeedLeft(void)						{ return obj_pololu_status->leftSpeed;};
	int32_t GetSpeedRight(void)						{ return obj_pololu_status->rightSpeed;};
	uint32_t GetMsToGo(void)						{ return obj_pololu_status->msToGo;};
	double GetX(void)								{ return obj_pololu_status->x;};
	double GetY(void)								{ return obj_pololu_status->y;};
	Angle GetPhi(void)								{ return obj_pololu_status->phi;};
	Position GetPos(void)							{ return Position(obj_pololu_status->x,obj_pololu_status->y);};

	// writing access to the status variables
	void SetMac (const uint8_t (&mac)[6])							{ for (int i = 0; i < 6; i++) obj_pololu_status->mac[i] = mac[i];};
	void SetDistanceSensors (const uint8_t distanceSensors)			{ obj_pololu_status->distanceSensors = distanceSensors;};
	void SetAccuVoltage (const uint32_t accuVoltage)				{ obj_pololu_status->accuVoltage = accuVoltage;};

	void SetMovingStatus (const eMovingStatus movingStatus)						{ obj_pololu_status->movingStatus = movingStatus;};
	void SetLocalizationStatus (const eLocalizationStatus localizationStatus) 	{ obj_pololu_status->localizationStatus = localizationStatus;};
	void SetModuleStatus (const eModuleStatus moduleStatus)						{ obj_pololu_status->moduleStatus = moduleStatus;};
	void SetSpeedLeft (const int32_t leftSpeed)									{ obj_pololu_status->leftSpeed = leftSpeed;};
	void SetSpeedRight (const int32_t rightSpeed)								{ obj_pololu_status->rightSpeed = rightSpeed;};
	void SetMsToGo (const uint32_t msToGo)										{ obj_pololu_status->msToGo = msToGo;};


	void SetX(double x) 	{ obj_pololu_status->x = x;};
	void SetY(double y) 	{ obj_pololu_status->y = y;};
	void SetPos( Position const &pos)
	{
		obj_pololu_status->x = pos.GetX();
		obj_pololu_status->y = pos.GetY();
	};
	void SetPhi(Angle const &phi)	{ obj_pololu_status->phi = phi;};

};



#endif /* POLOLU_STATUS_H_ */
