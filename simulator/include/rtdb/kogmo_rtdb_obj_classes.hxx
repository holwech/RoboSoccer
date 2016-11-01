/*! \file kogmo_rtdb_obj_classes.hxx
 * \brief List of Classes to handle Objects (C++ Interface).
 *
 * Copyright (c) 2003-2006 Matthias Goebl <mg*tum.de>
 *     Lehrstuhl fuer Realzeit-Computersysteme (RCS)
 *     Technische Universitaet Muenchen (TUM)
 */

#ifndef KOGMO_RTDB_H
# error "Never use <objects/*.hxx> directly; include <kogmo_rtdb.hxx> instead."
#endif


// All: Base Object
#include "kogmo_rtdb_obj_base_classes.hxx"
//#include "kogmo_rtdb_obj_c3_base_classes.hxx"

// C3:
#if defined(KOGMO_RTDB_INCLUDE_C3_OBJECTS) || !defined(KOGMO_RTDB_DONT_INCLUDE_ALL_OBJECTS)
#include "kogmo_rtdb_obj_c3_classes.hxx"
#endif

// A2:
#if defined(KOGMO_RTDB_INCLUDE_A2_OBJECTS) || !defined(KOGMO_RTDB_DONT_INCLUDE_ALL_OBJECTS)
#include "kogmo_rtdb_obj_a2_classes.hxx"
#endif


// ADD YOUR OWN OBJECTS HERE:


// dies ist ein versuch (mg):
// (gibt's irgendeine schickere methode?)
// z.B.: perl -ne '/^class *([^\s]+)/ && print "  DUMPTRY($1);\n"' kogmo_objects/*.hxx
namespace KogniMobil {
 inline std::string anydump(RTDBObj &dumpobj)
 {
   RTDBConn DBC(dumpobj.getDBC());
   #define DUMPTRY(OBJ) { OBJ Obj(DBC); if ( Obj.Copy(dumpobj,1) ) return Obj.dump(); }
   // Der Trick: Obj.Copy() liefert nur true, wenn die TYPE-ID matcht
	//DUMPTRY(C3_RTDB);
 //	DUMPTRY(C3_Process);

	#if defined(KOGMO_RTDB_INCLUDE_C3_OBJECTS) || !defined(KOGMO_RTDB_DONT_INCLUDE_ALL_OBJECTS)
	DUMPTRY(C3_SixDoF);
	DUMPTRY(C3_CamState);
	DUMPTRY(C3_CamCommand);
	DUMPTRY(C3_Text);
	DUMPTRY(C3_GPSData);
	DUMPTRY(C3_PlayerCtrl);
	DUMPTRY(C3_PlayerStat);
	#endif

	#if defined(KOGMO_RTDB_INCLUDE_A2_OBJECTS) || !defined(KOGMO_RTDB_DONT_INCLUDE_ALL_OBJECTS)
	DUMPTRY(A2_RoadKloth);
	DUMPTRY(A2_ImageAnnot);
	DUMPTRY(A2_Camera);
	DUMPTRY(A2_IntCal);
	DUMPTRY(A2_ExtCal);
	#endif
	
	#if defined(KOGMO_RTDB_INCLUDE_A2_OBJECTS) || !defined(KOGMO_RTDB_DONT_INCLUDE_ALL_OBJECTS)
        DUMPTRY(A2_AnyImage); // das steht am Ende, da es "testweise" recht viel Speicher alloziert
	#endif

   //DUMPTRY(YOUR_OBJECT_WITH_A_DUMP_METHOD);
   // nach Ergaenzungen muss man qrtdbctl neu compilieren

   return dumpobj.dump();
 }

}

