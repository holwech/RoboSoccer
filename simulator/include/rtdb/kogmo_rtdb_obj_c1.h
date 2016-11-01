/*! \file kogmo_rtdb_obj_c1.h
 * \brief Objects from C1
 *
 * Copyright (c) 2007 Matthias Goebl <mg*tum.de>
 *     Lehrstuhl fuer Realzeit-Computersysteme (RCS)
 *     Technische Universitaet Muenchen (TUM)
 */

#ifndef KOGMO_RTDB_OBJ_C1_H
#define KOGMO_RTDB_OBJ_C1_H

#include "kogmo_rtdb_obj_packing.h"
#include "kogmo_rtdb_obj_base.h"
#include "kogmo_rtdb_obj_c3.h"

#ifdef __cplusplus
 extern "C" {
 namespace KogniMobil {
#endif


/*! \addtogroup kogmo_rtdb_objects */
/*@{*/



/*! \brief Sollgroessenvorgabe an Autobox
 * DIES IST NOCH ALPHA UND WIRD SICH AENDERN!
 * <mg*tum.de>
 */
typedef PACKED_struct
{
 double lenkrate; // in rad/sekunde
 double lenkwinkelbegrenzung; // in rad
 double geschwindigkeit; // in m/s
} kogmo_rtdb_subobj_c1_vehiclecmd_t;

typedef PACKED_struct
{
  kogmo_rtdb_subobj_base_t base;
  kogmo_rtdb_subobj_c3_sixdof_t sixdof;
  kogmo_rtdb_subobj_c1_vehiclecmd_t vehiclecmd;
} kogmo_rtdb_obj_c1_vehiclecmd_t;

/*! \brief Istgroessen von Autobox
 * DIES IST NOCH ALPHA UND WIRD SICH AENDERN!
 * <mg*tum.de>
 */
typedef PACKED_struct
{
 double lenkwinkel; // in rad
 double geschwindigkeit; // in m/s
 struct {
  uint32_t laengsaktorik  : 1;
  uint32_t queraktorik    : 1;
  uint32_t notaus         : 1;
  uint32_t bremslicht     : 1;
  uint32_t blinker_links  : 1;
  uint32_t blinker_rechts : 1;
  uint32_t abblendlicht   : 1;
  uint32_t fernlicht      : 1;
  uint32_t motor          : 1;
  uint32_t tachosensor    : 1;
 } status;
 double wegstrecke; // kilometerzaehler in [m]
} kogmo_rtdb_subobj_c1_vehiclestat_t;
#define C1_VEHICLESTAT_OLD_SIZEDIFF (sizeof(uint32_t)+sizeof(double))

typedef PACKED_struct
{
  kogmo_rtdb_subobj_base_t base;
  kogmo_rtdb_subobj_c3_sixdof_t sixdof;
  kogmo_rtdb_subobj_c1_vehiclestat_t vehiclestat;
} kogmo_rtdb_obj_c1_vehiclestat_t;



/*@}*/

#ifdef __cplusplus
 }; /* namespace KogniMobil */
 }; /* extern "C" */
#endif


#endif /* KOGMO_RTDB_OBJ_C1_H */
