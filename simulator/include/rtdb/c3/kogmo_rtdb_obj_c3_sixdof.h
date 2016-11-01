/*! \file kogmo_rtdb_obj_c3_sixdof.h
 * \brief 6DoF Object from C3
 *
 * Copyright (c) 2003-2006 Matthias Goebl <mg*tum.de>
 *     Lehrstuhl fuer Realzeit-Computersysteme (RCS)
 *     Technische Universitaet Muenchen (TUM)
 */

#ifndef KOGMO_RTDB_OBJ_C3_SIXDOF_H
#define KOGMO_RTDB_OBJ_C3_SIXDOF_H

#include "kogmo_rtdb_obj_packing.h"
#include "kogmo_rtdb_obj_base.h"

#ifdef __cplusplus
 extern "C" {
 namespace KogniMobil {
#endif

/*! \addtogroup kogmo_rtdb_objects */
/*@{*/



/*! \brief The Part of the Object that holds Position and Orientation.
 * Object with a Position and Orientation in 3D
 */
typedef struct
{
 // Postition
 double x;  // [m]
 double y;  // [m]
 double z;  // [m]
 // Orientation
 double yaw;  // [rad]
 double pitch;  // [rad]
 double roll;  // [rad]
} kogmo_rtdb_subobj_c3_sixdof_t;

/*! \brief Full Object with only a Position and Orientation in 3D
 */
typedef struct
{
  kogmo_rtdb_subobj_base_t base;
  kogmo_rtdb_subobj_c3_sixdof_t sixdof;
} kogmo_rtdb_obj_c3_sixdof_t;




/*@}*/

#ifdef __cplusplus
 }; /* namespace KogniMobil */
 }; /* extern "C" */
#endif

#endif /* KOGMO_RTDB_OBJ_C3_SIXDOF_H */
