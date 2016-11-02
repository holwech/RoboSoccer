/*! \file kogmo_rtdb_obj_defs.h
 * \brief List of Objects-Definitions to Include (C-Interface).
 *
 * Copyright (c) 2003-2006 Matthias Goebl <mg*tum.de>
 *     Lehrstuhl fuer Realzeit-Computersysteme (RCS)
 *     Technische Universitaet Muenchen (TUM)
 *   and Project Partners of SFB/TR 28 "Kognitive Automobile"
 */

#ifndef KOGMO_RTDB_H
# error "Never use <objects/*.h> directly; include <kogmo_rtdb.h> instead."
#endif

// Define Object Type-IDs
#include "kogmo_rtdb_obj_typeids.h"

// All: Base Object (always needed)
#include "kogmo_rtdb_obj_base.h"


// Wer Probleme mit allen Objektdefinitionen hat,
// kann ganz einfach direkt seine objektdefinitionen includen.
// Dann wird automatisch nur das eingebunden was wirklich benötigt wird 
// z.B:
// #include "kogmo_rtdb_obj_a2_classes.hxx"
//
// Wichtig:
// Wenn eine Header-Datei andere Definitionen braucht, muss man die eben einbinden.
// Und damit eine Datei nicht 2x eingebunden wird, muss man die gesamte Definition (bzw File) in einen ifdef-Block setzen:
// #ifndef KOGMO_RTFB_OBJ_B1_H
// #ifndef KOGMO_RTDB_OBJ_B1_H
// ..
// #endif
//
// Beispiel:
// In "kogmo_rtdb_obj_sfb_sim.h" wird auf die Matrix4x4-Definition von "kogmo_rtdb_obj_sfb.h zur�ckgegriffen.
// Deshalb enth�lt "kogmo_rtdb_obj_sfb_sim.h" jetzt ein "#include "kogmo_rtdb_obj_sfb.h"


// Objects defined by Subproject "C3":
#if defined(KOGMO_RTDB_INCLUDE_C3_OBJECTS) || !defined(KOGMO_RTDB_DONT_INCLUDE_ALL_OBJECTS)
#include "kogmo_rtdb_obj_c3.h"
#endif

// A2:
#if defined(KOGMO_RTDB_INCLUDE_A2_OBJECTS) || !defined(KOGMO_RTDB_DONT_INCLUDE_ALL_OBJECTS)
#include "kogmo_rtdb_obj_a2.h"
#endif

// C1:
#if defined(KOGMO_RTDB_INCLUDE_C1_OBJECTS) || !defined(KOGMO_RTDB_DONT_INCLUDE_ALL_OBJECTS)
#include "kogmo_rtdb_obj_c1.h"
#endif

// ADD YOUR OWN OBJECTS HERE:
// ...
