#ifndef POLOLU_LOCALIZE_H_
#define POLOLU_LOCALIZE_H_


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



/**
 */
class PololuLocalize : public RTDBObj
{
protected:
	/** Struct which holds all information that is stored in the RDTB. */
	kogmo_rtdb_subobj_pololu_localize_t *obj_pololu_localize;
public:

	/** Initializes the status information. */
	PololuLocalize(class RTDBConn& DBC, const char* name = "", const int& otype =
			KOGMO_RTDB_OBJTYPE_POLOLU, const int32_t& child_size = 0,
			char** child_dataptr = NULL);

	// reading access to the status variables
	Position GetPos(int device) {
		if (device < 6) {
			return obj_pololu_localize->pos[device];
		}
		else {
			return Position(1000,1000);
		}
	};
	Angle GetPhi(int device) {
		if (device < 6) {
			return obj_pololu_localize->phi[device];
		}
		else {
			return Angle(0);
		}
	}
	// writing access to the status variables
	void SetPos(int device, Position pos) {
		if (device < 6) {
			obj_pololu_localize->pos[device] = pos;
		}
	}
	void SetPhi(int device, Angle phi) {
		if (device < 6) {
			obj_pololu_localize->phi[device] = phi;
		}
	}
};



#endif /* POLOLU_LOCALIZE_H_ */
