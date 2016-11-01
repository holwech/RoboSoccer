/*! \file kogmo_rtdb_obj_c3_test.h
 * \brief Test/Old Objects from C3
 *
 * Copyright (c) 2003-2006 Matthias Goebl <mg*tum.de>
 *     Lehrstuhl fuer Realzeit-Computersysteme (RCS)
 *     Technische Universitaet Muenchen (TUM)
 */

#ifndef KOGMO_RTDB_OBJ_C3_TEST_H
#define KOGMO_RTDB_OBJ_C3_TEST_H

#include "kogmo_rtdb_obj_packing.h"
#include "kogmo_rtdb_obj_base.h"

#ifdef __cplusplus
 extern "C" {
 namespace KogniMobil {
#endif

/*! \addtogroup kogmo_rtdb_objects */
/*@{*/


////////////////////// DO NOT USE kogmo_rtdb_obj_lwscan!
/////////////////////////// SEE C1/C2 !!!
/*! \brief The Part of the Object that holds the actual Steering Wheel Position and Velocity of a CAN-Sensor.
 * Object with a Stering Wheel Angle and Steering Whell Velocity
 */
typedef struct
{
 // Postition
 double angle;
 // Angle Velocity
 double speed;
 // Internal sensor status
 unsigned int stat;
 // MsgCount
 unsigned int msg_count;
 // Checksum
  unsigned int chck_sum;
} kogmo_rtdb_subobj_c3_lwscan_t;

/*! \brief Full Object with only a Position and Orientation in 3D
 */
typedef struct
{
  kogmo_rtdb_subobj_base_t base;
  kogmo_rtdb_subobj_c3_lwscan_t lwscan;
} kogmo_rtdb_obj_c3_lwscan_t;





////////////////////// DO NOT USE kogmo_rtdb_obj_c3_egodata!
/////////////////////////// SEE C1/C2 !!!
enum kogmo_rtdb_obj_c3_egodata_gear {
  C3_EGODATA_GEAR_IDLE      = 0,
  C3_EGODATA_GEAR_REVERSE   = -1,
  C3_EGODATA_GEAR_FORWARD   = 1, // wenn nicht naeher bekannt
  C3_EGODATA_GEAR_FORWARD_1 = 11,
  C3_EGODATA_GEAR_FORWARD_2 = 12, // usw..
};

/*! \brief Raw Ego(motion) Data, needs to be processed first to be usable for other projects
 * This is primarily for recording raw sensor data
 */
typedef struct
{
 float speed;         //!< current speed [m/s]
 float accel;         //!< current acceleration [m/s^2]
 float distance;      //!< covered distance [m]
 float distance_time; //!< time to cover above distance [s]
 enum kogmo_rtdb_obj_c3_egodata_gear gear; //!< Current Gear
 struct
 {
   uint32_t headlight : 1;        //!<
   uint32_t dimmed_headlight : 1; //!<
   uint32_t indicator_right : 1;  //!<
   uint32_t indicator_left : 1;   //!<
 } lights;
 uint32_t test_sensors;           //!< fuer Tests
 // TODO: + Lenkwinkelsensoren, Raddrehzahlsensoren..
} kogmo_rtdb_subobj_c3_egodata_t;

/*! \brief Full Object with Raw Egodata
 */
typedef struct
{
  kogmo_rtdb_subobj_base_t base;
  kogmo_rtdb_subobj_c3_egodata_t egodata;
} kogmo_rtdb_obj_c3_egodata_t;


/*@}*/

#ifdef __cplusplus
 }; /* namespace KogniMobil */
 }; /* extern "C" */
#endif

#endif /* KOGMO_RTDB_OBJ_C3_TEST_H */
