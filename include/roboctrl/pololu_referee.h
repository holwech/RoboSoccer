#ifndef POLOLU_REFEREE_H_
#define POLOLU_REFEREE_H_

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
using namespace std;        // TODO needed?
using namespace KogniMobil; // TODO needed?

class PololuReferee : public RTDBObj {

protected:
	/** Struct which holds all information that is stored in the RDTB. */
	kogmo_rtdb_subobj_pololu_referee2_t *obj_pololu_referee;
public:

	/** Initializes the status information. */
        PololuReferee(class RTDBConn& DBC, const char* name = "rtdb_referee2", const int& otype = KOGMO_RTDB_OBJTYPE_POLOLU,
                      const int32_t& child_size = 0, char** child_dataptr = NULL);

	void Init();
	void Init(bool client);
        void setBlueReady() {
		this->RTDBRead();
		obj_pololu_referee->blue_ready = Timestamp::getNow();
		this->setDataTimestamp();
		this->RTDBWrite();
        }
        void setRedReady() {
		this->RTDBRead();
		obj_pololu_referee->red_ready = Timestamp::getNow();
		this->setDataTimestamp();
		this->RTDBWrite();
        }

        Timestamp getBlueReady() {
		this->RTDBRead();
		return obj_pololu_referee->blue_ready;
        }

	Timestamp getRedReady(){
		this->RTDBRead();
		return obj_pololu_referee->red_ready;
        }
};

#endif /* POLOLU_REFEREE_H_ */
