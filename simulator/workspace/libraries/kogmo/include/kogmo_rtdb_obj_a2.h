#ifndef KOGMO_RTDB_OBJ_A2_H
#define KOGMO_RTDB_OBJ_A2_H

#include "kogmo_rtdb_obj_base.h"
#include "kogmo_rtdb_obj_c3.h"

#ifdef __cplusplus
 extern "C" {
 namespace KogniMobil {
#endif


/*! \addtogroup kogmo_rtdb_objects */ /*@{*/
// Maximalanzahl der Segmente im folgenden Klothoidenmodell
#define A2_ROADKLOTH_SEGMAX 5

/*! \brief Eigenfahrspur als Klothoide modelliert
 * Modellbeschreibung wie folgt/zu finden in: ...
 */
typedef struct
{
 float y;
   //!< Ablage in m
 float phi;
   //!< Gierwinkel in rad
 int n;
   //!< Anzahl der Segmente (d.h. wieviele Eintraege der folgenden Arrays
   //!< gueltig sind)
 float l [A2_ROADKLOTH_SEGMAX];
   //!< Segmentlaenge
 float c0 [A2_ROADKLOTH_SEGMAX];
   //!< Kruemmung
 float c1 [A2_ROADKLOTH_SEGMAX];
   //!< Kruemmungsaenderung
 float b0 [A2_ROADKLOTH_SEGMAX];
   //!< Breite
 float b1 [A2_ROADKLOTH_SEGMAX];
   //!< Breitenaenderung
} kogmo_rtdb_subobj_a2_roadkloth_t;

/*! \brief Vollstaendiges Objekt mit Spurdaten
 */
typedef struct
{
  kogmo_rtdb_subobj_base_t base;
    //!< Basisdaten
  kogmo_rtdb_subobj_c3_sixdof_t sixdof;
    //!< Relativpostion zum projizierten Fahrzeug-Schwerpunkt-Koordinatensystem
  kogmo_rtdb_subobj_a2_roadkloth_t roadkloth;
    //!< Die Daten des eigentlichen Klothoidenmodells
} kogmo_rtdb_obj_a2_roadkloth_t; 



/*! \brief Kamera-Objekt (Vaterknoten)
 * Dieses Objekt sollte sich zur Laufzeit nicht aendern und
 * wird von grabdma angelegt.
 * Kind-Objekte (Knoten) muessen sein:
 * - a2_intcal
 * - a2_extcal
 * - a2_image
 * Zur Not (z.B. alte Videos) ist es auch erlaubt,
 * das Kamera-Objekt unter a2_image zu haengen.
 */
typedef struct
{
 uint64_t euid; // Unique ID der Kamera
 char descr[4096]; // Beschreibung der Kamera (Name/Besitzer/Verbauort)
} kogmo_rtdb_subobj_a2_camera_t;

typedef struct
{
  kogmo_rtdb_subobj_base_t base;
  kogmo_rtdb_subobj_a2_camera_t camera;
} kogmo_rtdb_obj_a2_camera_t; 




/*! \brief Intrinsische Kamerakalibrierdaten
 * Dieses Objekt sollte sich "nicht oft" aendern (max alle paar Sekunden),
 * wird von grabdma angelegt mit defaultwerten aus cams.h.
 * Fuer hoehere Genauigkeit sollte eine Kalibrierung angestossen werden.
 * Dieses Objekt hat als Vater ein kogmo_rtdb_obj_a2_camera_t.
 */
typedef struct
{
 // absoluter Zeitpunkt der Kalibrierung soll in base.data_ts stehen!
 float quality; // "Qualitaetsmass der Kalibrierdaten", Wertebereich noch nicht eindeutig festgelegt, sicher ist aber: 0=Daten ungueltig
 // Kamera-fixe Werte aus Datenblatt: 
 int dx; // Sensorgroesse in Pixel in X-Richtung [in Pixel] (z.B. 640)
 int dy; // Sensorgroesse in Pixel in Y-Richtung (z.B. 480)
 float sx; // Sensorelementpixelgroesse X [in m] (z.B. 6e-6 == 6um)
 float sy; // Sensorelementpixelgroesse Y (z.B. 6e-6 == 6um)
 // Kalibrierte Werte:
 float cx; // Hauptpunkt X [in Pixel] (z.B. 322, evtl. Subpixel-genau!)
 float cy; // Hauptpunkt Y (z.B. 245)
 float f;  // Brennweite [in m] (z.B. 0.016 == 16e-3 == 16mm Objektiv) [in pixel: f_px = f/sx, dabei aber fuellfaktor als problem]
 float k;  // Linsenverzerrung radial (z.B. -500) [0 wenn nicht verwendet]
} kogmo_rtdb_subobj_a2_intcal_t;

typedef struct
{
  kogmo_rtdb_subobj_base_t base;
  kogmo_rtdb_subobj_a2_intcal_t intcal;
} kogmo_rtdb_obj_a2_intcal_t; 


/*! \brief Extrinsische Kamerakalibrierung
 * Dieses Objekt aendert sich immer dann, wenn die Kamera bewegt wird (Plattform),
 * wird von grabdma angelegt mit defaultwerten aus cams.h.
 * Dieses Objekt hat als Vater ein kogmo_rtdb_obj_a2_camera_t.
 */
typedef struct
{
 // absoluter Zeitpunkt der Kalibrierung soll in base.data_ts stehen!
 float quality; // "Qualitaetsmass der Kalibrierdaten", Wertebereich noch nicht eindeutig festgelegt, sicher ist aber: 0=Daten ungueltig
} kogmo_rtdb_subobj_a2_extcal_t;

typedef struct
{
  kogmo_rtdb_subobj_base_t base;
  kogmo_rtdb_subobj_c3_sixdof_t sixdof;
  kogmo_rtdb_subobj_a2_extcal_t extcal;
} kogmo_rtdb_obj_a2_extcal_t;





/*! \brief Ein Videobild
 * (aehnlich OpenCV)
 * Aendert sich mit jedem Zyklus.
 * Ein Kamerabild hat als Vater ein kogmo_rtdb_obj_a2_camera_t.
 */
typedef struct
{
  int width;            //!< Breite, Nullpunkt links   [bitte keine negativen Werte mehr verwendent, fruehere Festlegung: "negative Werte: Bild horizontal gespiegelt, Nullpunkt rechts")
                        // Die Breite sollte durch 4 teilbar sein, sonst kann man die KogMo-RTDB-AVIs evtl nicht mit einem anderen AVI-Player abspielen (macht aber nichts)
  int height;           //!< Hoehe, Nullpunkt oben     [bitte keine negativen Werte mehr verwendent, fruehere Festlegung: "negative Werte: Bild vertikal gespiegelt, Nullpunkt unten")
  int depth;            // Bits/Pixel, diese heissen A2_IMAGE_DEPTH_8U usw. und entsprechen opencv/cxtypes.h (s.u.)
  int channels;         // number of color channels, 1 for greyscale, 3 for rgb (nach opencv)
  int colormodell;      // 0=rgb interleaved, .. (*heul* schreibfehler! muesste "colormodell" heissen:-()
  int widthstep;        // Breite einer Zeile in Bytes einschliesslich Alignment (see opencv)
  unsigned char data[0];// nur, um die Adresse des ersten Pixels zu bekommen
                        // => size = sizeof(kogmo_rtdb_subobj_a2_image_t) + height*widthstep !!
} kogmo_rtdb_subobj_a2_image_t;

// Definierte Werte fuer depth (entsprechen den jeweiligen OpenCV/IPL-Definitionen, siehe opencv/cxtypes.h)
// KEINE EIGENEN MODI ohne Absprache mit A2 erfinden!
#define A2_IMAGE_DEPTH_1U           1  // 1 Bits/Pixel, unsigned char  (==IPL_DEPTH_1U)
#define A2_IMAGE_DEPTH_8U           8  // 8 Bits/Pixel, unsigned char  (==IPL_DEPTH_8U)
#define A2_IMAGE_DEPTH_16U         16  // 16 Bits/Pixel, unsigned word (==IPL_DEPTH_16U)
#define A2_IMAGE_DEPTH_32F         32  // 32 Bits/Pixel, float         (==IPL_DEPTH_32F)
#define A2_IMAGE_DEPTH_8S  0x80000008  // 8 Bits/Pixel, signed char    (==IPL_DEPTH_8S==IPL_DEPTH_SIGN|IPL_DEPTH_8U)
#define A2_IMAGE_DEPTH_16S 0x80000016  // 16 Bits/Pixel, signed char    (==IPL_DEPTH_16S==IPL_DEPTH_SIGN|IPL_DEPTH_16U)
#define A2_IMAGE_COLORMODEL_RGB    0   // Bei 1 Kanal: Graustufenbild, bei 3 Kanaelen: RGB
#define A2_IMAGE_COLORMODEL_YUV411 1   // YUV411
#define A2_IMAGE_COLORMODEL_YUV422 2   // YUV422
#define A2_IMAGE_COLORMODEL_YUV444 3   // YUV444

/*! \brief Vollstaendiges Objekt fuer ein Videobild
 */
typedef struct
{
  kogmo_rtdb_subobj_base_t base;
  kogmo_rtdb_subobj_a2_image_t image;
} kogmo_rtdb_obj_a2_image_t; 


/*! \brief Annotationen zu einem Videobild
 */
typedef struct
{
 uint16_t x1;
 uint16_t y1;
 uint16_t x2;
 uint16_t y2;
 uint8_t color;
 uint8_t width;
 uint16_t flags;
} annotline_t;
#define A2_IMAGEANNOT_MAXLINES 1024

typedef struct
{
  uint16_t numlines;    //!< Anzahl gueltiger Werte in line[]
  uint16_t flags;       //!< irgendwas
  annotline_t line[A2_IMAGEANNOT_MAXLINES];
} kogmo_rtdb_subobj_a2_imageannot_t;

/*! \brief Vollstaendiges Objekt fuer eine Videobild-Annotation
 * Das Annotationsobjekt sollte das betreffende Videobild als
 * als Parent-Objekt haben!
 */
typedef struct
{
  kogmo_rtdb_subobj_base_t base;
  kogmo_rtdb_subobj_a2_imageannot_t annot;
} kogmo_rtdb_obj_a2_imageannot_t; 















/*@}*/

#ifdef __cplusplus
}; /* namespace KogniMobil */
}; /* extern "C" */
#endif

#endif /* KOGMO_RTDB_OBJ_A2_H */
