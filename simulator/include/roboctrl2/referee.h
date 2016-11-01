#ifndef REFEREE_RTDB_CONNECTION_H_
#define REFEREE_RTDB_CONNECTION_H_

#include <iostream>
#include <deque>
#include <string>

//kogmo_rtdb includes
#include "kogmo_rtdb_obj_base_classes.hxx"
#include "kogmo_rtdb_obj_typeids.h"
#include "kogmo_rtdb_obj_base.h"
#include "pololu_referee.h"
#include "share.h"
#include "types.h"

using namespace KogniMobil;

/** Referee class which handles the connection with the referee process via the RTDB.
 *  An object of this class must be polled continuously to obey the referees commands
 *
 *  If the referee is in init state GetPlayMode() will return REFEREE_INIT
 *  If the person who control the referee decides which team has to play on the left side, the state changes to
 *  BEFORE_KICK_OFF.
 *
 *  At this time you can receive GetBlueSide(). The result is either LEFT_SIDE or RIGHT_SIDE
 *  You need to know your own team so that you can make use of this information, e.g. you are team red and receive that
 *  blue plays on the left side => you play on right side.
 *
 *  If you want to know which team may do the kick of then use GetSide(). It returns LEFT_SIDE or RIGHT_SIDE and you have to
 *  logically think what that means for your team.
 *
 *  Now you should drive to kick-off positions and then do SetReady(int team) including your team (Blue=0, Red=1)

 *  If there is any change of sides or another kick-off, the referee will change his state to BEFORE_KICK_OFF and you can then
 *  get the information by GetBlueSide() and GetSide().
 *
 *  While playing the referee will stay in state PLAY_ON. If the time is over you will receive the state TIME_OVER and a following
 *  BEFORE_KICK_OFF
 */
class Referee : public RTDBObj
{
protected:
	/** Struct which holds all information that is stored in the RDTB. */
	kogmo_rtdb_subobj_pololu_referee_t *obj_pololu_referee;
public:
	/** */
	Referee(class RTDBConn& DBC, const char* name = "rtdb_referee", const int& otype =
			KOGMO_RTDB_OBJTYPE_POLOLU, const int32_t& child_size = 0,
			char** child_dataptr = NULL);
	/** Initializes the referee status. */
	void Init(void);



	/** Retrieves the current play mode from the referee.
	 *  for more help concerning the PlayModes look up the enum ePlayMode in share.h
	 */
	ePlayMode GetPlayMode(void)
	{
		this->UpdateFromRTDB();
		return obj_pololu_referee->playMode;
	};
	/** Retrieves the current score of the left side. */
	int GetLeftSideGoals(void)
	{
		this->UpdateFromRTDB();
		return obj_pololu_referee->leftSideGoals;
	};
	/** Retrieves the current score of the right side. */
	int GetRightSideGoals(void)
	{
		this->UpdateFromRTDB();
		return obj_pololu_referee->rightSideGoals;
	};
	/** Retrieves the current value of the side.
	 *  This is used to find out which side has kick-off or penalty in the following way:
	 *
	 *  @return 0 means left, 1 means right
	 *
	 *  ePlayMode = BLUE_LEFT or RED_LEFT and ePlayMode = KICK_OFF:
	 *		GetSide() returns the side of the Team which has kick-off.
	 *
	 *	in all other ePlayMode's the return of GetSide is not defined
	 *	(and does not matter)
	 *
	 */
	eSide GetSide(void)
	{
		this->UpdateFromRTDB();
		return obj_pololu_referee->side;
	};

	eSide GetBlueSide(void) {
		this->UpdateFromRTDB();
		return obj_pololu_referee->blue;
	}
	/** Here you can set if you are ready.
	 * @param[in] side Has to be 0 if you are blue, 1 if you are red.
	 */
	void SetReady(int side) {
		if (side == 0) {
			cout << "Blue ready" << endl;
			referee2->setBlueReady();
		}
		else {
			cout << "Red ready" << endl;
			referee2->setRedReady();
		}
		this->setDataTimestamp();
		this->RTDBWrite();
	};

	/** Here you can set ready the blue team */
	void SetBlueReady(void) {
		//cout << "Blue ready" << endl;
		referee2->setBlueReady();
	};

	/** Here you can set ready the red team */
	void SetRedReady(void)  {
		//cout << "Red ready" << endl;
		referee2->setRedReady();
	};
private:
	void UpdateFromRTDB(void)
	{
		this->RTDBRead();
	}
	PololuReferee *referee2;
};





#endif /* REFEREE_RTDB_CONNECTION_H_ */
