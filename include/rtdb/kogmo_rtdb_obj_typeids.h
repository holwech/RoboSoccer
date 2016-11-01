/*! \file kogmo_rtdb_obj_typeids.h
 * \brief List of Objects-Type-IDs
 *
 * Copyright (c) 2007 Matthias Goebl <mg*tum.de>
 *     Lehrstuhl fuer Realzeit-Computersysteme (RCS)
 *     Technische Universitaet Muenchen (TUM)
 *   and Project Partners of SFB/TR 28 "Kognitive Automobile"
 */


#ifndef KOGMO_RTDB_OBJ_TYPEIDS_H
#define KOGMO_RTDB_OBJ_TYPEIDS_H

enum kogmo_rtdb_objtype {
  // Alle: Ein Objekt, das keinen Datenblock enthaelt
  KOGMO_RTDB_OBJTYPE_EMPTY							= 0x000001,
  // Alle: Ein Objekt, das einen beliebig langen, unspezifizierten Datenblock enthaelt
  KOGMO_RTDB_OBJTYPE_FREEDATA						= 0x000002,
 
  //-----------------------------------------------------------
  //	A2
  //-----------------------------------------------------------

  // A2: Spurdaten nach Klothoidenmodell, erste Version:
  KOGMO_RTDB_OBJTYPE_A2_ROADKLOTH_V1				= 0xA20101,
  // Kamera, Orientierung, Parameter und Kalibrierdaten
  KOGMO_RTDB_OBJTYPE_A2_CAMERA						= 0xA20020,
  KOGMO_RTDB_OBJTYPE_A2_INTCAL						= 0xA20021,
  KOGMO_RTDB_OBJTYPE_A2_EXTCAL						= 0xA20022,
  // Ein Videobild, incl Aufloesung, Farbtiefe, etc.
  KOGMO_RTDB_OBJTYPE_A2_IMAGE						= 0xA20030,
  // Annotationen (Liniensegmente) zu einem Videobild
  KOGMO_RTDB_OBJTYPE_A2_IMAGEANNOT					= 0xA20034,

  //-----------------------------------------------------------
  //	C1
  //-----------------------------------------------------------

  // C1: Fahrzeuginterface
  KOGMO_RTDB_OBJTYPE_C1_VEHICLECMD_V1				= 0xC10101,
  KOGMO_RTDB_OBJTYPE_C1_VEHICLEDATA_V1				= 0xC10201,		//change name? Is this kogmo_rtdb_obj_c1_vehiclestat_t ?

  //-----------------------------------------------------------
  //	C3
  //-----------------------------------------------------------

  // C3: Datenbank-interne Objekte
  KOGMO_RTDB_OBJTYPE_C3_ROOT						= 0xC30001,
  KOGMO_RTDB_OBJTYPE_C3_PROCESSLIST					= 0xC30002,
  KOGMO_RTDB_OBJTYPE_C3_PROCESS						= 0xC30003,
  KOGMO_RTDB_OBJTYPE_C3_RTDB						= 0xC30004,
  KOGMO_RTDB_OBJTYPE_C3_RECORDERSTAT					= 0xC30006,
  KOGMO_RTDB_OBJTYPE_C3_RECORDERCTRL					= 0xC30007,
  KOGMO_RTDB_OBJTYPE_C3_PLAYERSTAT					= 0xC30008,
  KOGMO_RTDB_OBJTYPE_C3_PLAYERCTRL					= 0xC30009,
  // Objekt zur Repraesentation einer 3-dimensionalen Lage im Raum
  KOGMO_RTDB_OBJTYPE_C3_SIXDOF						= 0xC30010,
  // Objekt mit Fahrzeug-Rohdaten
  KOGMO_RTDB_OBJTYPE_C3_EGODATA						= 0xC30020,
  // Objekt mit GPS-Rohdaten
  KOGMO_RTDB_OBJTYPE_C3_GPSDATA						= 0xC30021,
  // Objekt mit Lenkwinkelsensorcandaten
  KOGMO_RTDB_OBJTYPE_C3_LWSCAN						= 0xC30030,
  // Objekt mit Log-Message
  KOGMO_RTDB_OBJTYPE_C3_LOGMESSAGE					= 0xC30040,
  KOGMO_RTDB_OBJTYPE_C3_TEXT						= 0xC30050,
  // Kameraplattform Status und Kommando
  KOGMO_RTDB_OBJTYPE_C3_CAMSTATE_V1					= 0xC30101,
  KOGMO_RTDB_OBJTYPE_C3_CAMCOMMAND_V1   			= 0xC30201,

  KOGMO_RTDB_OBJTYPE_SFB_CONTROLMATLABSIM			= 0xFB0008,

  //-----------------------------------------------------------
  // Fuer den SFB/TR 28 sind reserviert: 0x00[ABCDEF]XXXXX
  // Benutzt werden A[01234F] B[12] C[1234] D1 FB AA
  //-----------------------------------------------------------

  // Frei z.B. fuer CoteSys: 0x0CYYXXXX mit YY=Teilprojekt, z.B. 0x0CA10010

};

#endif /* KOGMO_RTDB_OBJ_TYPEIDS_H */
