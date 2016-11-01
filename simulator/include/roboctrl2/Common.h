/**
 * \file Common.h
 * \brief global common data types or macros for the whole project.
 */

#ifndef _COMMON_H_
#define _COMMON_H_

#include <cassert>
#include "kogmo_rtdb.hxx"
#include "robo_control.h"


/**
 * NOTE NOTE NOTE! INSERT THE STATEMENT SHOWN BELOW IN ANY FILE WHICH IS NOT MEANT TO BE
 * A PART OF THE MAIN PROGRAM, E.G. FOR TESTING ONLY!!!
 *
 * This define is used to indicate, that this file "Common.h" belongs to the real program of
 * soccer C group and NOT being in a testbench. To use it, make the following statement in
 * EVERY FILE THAT IS NOT PART OF THE REAL PROGRAM; BUT ONLY NEEDED FOR TESTING:
 *
 * #ifdef SOCCERC_IS_MAIN
 * 		assert(false);
 * #endif
 *
 * This way the compiler will yield an error, when you are accidently including any file, whose
 * purpose was testing only!!!
 */
#ifndef SOCCERC_IS_MAIN
	#define SOCCERC_IS_MAIN 1	///< undef this. if you want to use this header in a test bench.
#endif

#define PERIOD_SEC ((double)30E-3)	///< base period where all actions are triggered: approx. 30Hz.

#define MAX_ROBOS 3	///< number of robos in total on the field per team


/**
 * Coordinate definitions on playground.
 */
#define FIELD_X_MIN		-1.435
#define FIELD_X_MAX		 1.405
#define FIELD_Y_MIN		-0.89
#define FIELD_Y_MAX		 0.88

#define PENALTY_AREA_LEFT_X_MIN		FIELD_X_MIN
#define PENALTY_AREA_LEFT_X_MAX		-1.220
#define PENALTY_AREA_LEFT_Y_MIN		-0.334
#define PENALTY_AREA_LEFT_Y_MAX		 0.334

#define PENALTY_AREA_RIGHT_X_MIN	1.190
#define PENALTY_AREA_RIGHT_X_MAX	FIELD_X_MAX
#define PENALTY_AREA_RIGHT_Y_MIN	-0.334
#define PENALTY_AREA_RIGHT_Y_MAX	 0.334


/**
 * This enumeration used to signal the side of the game field we are playing on.
 *
 *   - if SIDE_LEFT  -> our side = {x_coord | x_coord<0, x_coord in Real [-1.42,1.42]}. standing in front of the field, facing the windows: LEFT.
 *   - if SIDE_RIGHT -> our side = {x_coord | x_coord>0, x_coord in Real [-1.42,1.42]}. standing in front of the field, facing the windows: RIGHT.
 */
enum game_side {SIDE_UNKNOWN, SIDE_LEFT, SIDE_RIGHT};

/**
 * our team color
 */
enum color_enum {COLOR_UNKNOWN, COLOR_RED, COLOR_BLUE};

/**
 * the actors (Goalkeeper, Tactics, Execution) may have the following states:
 */
enum action_status_type {
	STATUS_IDLE = 0,
	STATUS_BUSY
};

#endif // _TYPES_H_

