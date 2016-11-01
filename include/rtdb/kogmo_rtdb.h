/*! \file kogmo_rtdb.h
 * \brief Master-Header to access the Realtime Vehicle Database
 *
 * Copyright (c) 2003-2006 Matthias Goebl <mg*tum.de>
 *     Lehrstuhl fuer Realzeit-Computersysteme (RCS)
 *     Technische Universitaet Muenchen (TUM)
 */

#ifndef KOGMO_RTDB_H
#define KOGMO_RTDB_H

#ifndef KOGMO_RTDB_REV
#define KOGMO_RTDB_REV 432
#endif

// internal
typedef struct {
  char dummy[80];
} kogmo_rtdb_handle_t;

#include "kogmo_rtdb_types.h"
#include "kogmo_rtdb_funcs.h"

#ifndef KOGMO_RTDB_DONT_INCLUDE_ALL_OBJECTS
#include "kogmo_rtdb_obj_defs.h"
#include "kogmo_rtdb_obj_funcs.h"
#endif

#ifndef WIN32
#include "kogmo_rtdb_utils.h"
#endif

#endif /* KOGMO_RTDB_H */
