/*! \file kogmo_rtdb_obj_c3_camplatform.h
 * \brief C3 Objects for Camera Platform Control
 *
 * Copyright (c) 2003-2006 Matthias Goebl <mg*tum.de>
 *     Lehrstuhl fuer Realzeit-Computersysteme (RCS)
 *     Technische Universitaet Muenchen (TUM)
 */

#ifndef KOGMO_RTDB_OBJ_C3_CAMPLATFORM_H
#define KOGMO_RTDB_OBJ_C3_CAMPLATFORM_H

#include "kogmo_rtdb_obj_packing.h"
#include "kogmo_rtdb_obj_base.h"

#ifdef __cplusplus
 extern "C" {
 namespace KogniMobil {
#endif

/*! \addtogroup kogmo_rtdb_objects */
/*@{*/


/*! \brief Object with Cameraplattform Status
 */
typedef struct
{
 double left_yaw; // in degrees, positive is to the right (drivers view). 0 is straigt forward view.
 double right_yaw; // in degrees, positive is to the right (drivers view). 0 is straigt forward view.
 double tele_yaw; // in degrees, positive is to the right (drivers view). 0 is straigt forward view.
 double tele_pitch; // in degrees, positive is the upward direction (drivers view). 0 is straigt forward view.
 struct
    {
      uint32_t left_valid : 1; // irgendwann schon mal einen wert bekommen
      uint32_t left_update : 1; // in diesem rtdb commit wurde dieser wert aktualisiert
      uint32_t left_ismoving : 1;  // achse bewegt sich
      uint32_t left_motpos_absolute : 1; //TPU-Null === Encoder-Null

      uint32_t right_valid : 1; // irgendwann schon mal einen wert bekommen
      uint32_t right_update : 1; // in diesem rtdb commit wurde dieser wert aktualisiert
      uint32_t right_ismoving : 1;  // achse bewegt sich
      uint32_t right_motpos_absolute : 1; //TPU-Null === Encoder-Null

      uint32_t tele_valid : 1; // irgendwann schon mal einen wert bekommen
      uint32_t tele_update : 1; // in diesem rtdb commit wurde dieser wert aktualisiert
      uint32_t tele_ismoving : 1;  // achse bewegt sich
      uint32_t tele_motpos_absolute : 1; //TPU-Null === Encoder-Null
      
      uint32_t pitch_valid : 1; // irgendwann schon mal einen wert bekommen
      uint32_t pitch_update : 1; // in diesem rtdb commit wurde dieser wert aktualisiert
      uint32_t pitch_ismoving : 1;  // achse bewegt sich
      uint32_t pitch_motpos_absolute : 1; //TPU-Null === Encoder-Null
      
    } status;
} kogmo_rtdb_subobj_c3_camstate_t;

/*! \brief Full Object with Cameraplattform Status
 */
typedef struct
{
  kogmo_rtdb_subobj_base_t base;
  kogmo_rtdb_subobj_c3_camstate_t camstate;
} kogmo_rtdb_obj_c3_camstate_t;



/*! \brief Object with Cameraplattform Command
 */
typedef struct
{
 double left_yaw; // in radians, positive is to the right (drivers view). 0 is straigt forward view.
 double right_yaw; // in radians, positive is to the right (drivers view). 0 is straigt forward view.
 double tele_yaw; // in radians, positive is to the right (drivers view). 0 is straigt forward view.
 double tele_pitch; // in radians, positive is the upward direction (drivers view). 0 is straigt forward view.
 uint8_t status_cycle; //in ticks, cycle time of the cyclic encoder position message
 struct
    {
      uint32_t left_update : 1;  // in diesem rtdb commit wurde dieser wert aktualisiert -> Kommando an entsprechende Achse
      uint32_t right_update : 1; // in diesem rtdb commit wurde dieser wert aktualisiert -> Kommando an entsprechende Achse
      uint32_t tele_update : 1;  // in diesem rtdb commit wurde dieser wert aktualisiert -> Kommando an entsprechende Achse
      uint32_t pitch_update : 1; // in diesem rtdb commit wurde dieser wert aktualisiert -> Kommando an entsprechende Achse
      uint32_t cycle_update : 1; // in diesem rtdb commit wurde dieser wert aktualisiert -> Kommando an entsprechende Achse
      uint32_t left_tpu_zero : 1; // TPU fuer Motor auf 0 setzen
      uint32_t right_tpu_zero : 1; // TPU fuer Motor auf 0 setzen
      uint32_t tele_tpu_zero : 1; // TPU fuer Motor auf 0 setzen
      uint32_t pitch_tpu_zero : 1; // TPU fuer Motor auf 0 setzen
    } command;
 uint8_t left_init_mode;
 uint8_t right_init_mode;
 uint8_t tele_init_mode;
} kogmo_rtdb_subobj_c3_camcommand_t;

/*! \brief Full Object with Cameraplattform Command
 */
typedef struct
{
  kogmo_rtdb_subobj_base_t base;
  kogmo_rtdb_subobj_c3_camcommand_t camcommand;
} kogmo_rtdb_obj_c3_camcommand_t;
















/*@}*/

#ifdef __cplusplus
 }; /* namespace KogniMobil */
 }; /* extern "C" */
#endif

#endif /* KOGMO_RTDB_OBJ_C3_CAMPLATFORM_H */
