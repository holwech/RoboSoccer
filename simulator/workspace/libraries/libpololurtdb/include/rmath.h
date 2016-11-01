// -*- C++ -*-
/***************************************************************************
 *
 *  Ein paar Templates für Mathefunktionen
 *
 *  (C) RCS
 *
 ***************************************************************************/

#ifndef INCLUDED_ROVI_RMATH_H
#define INCLUDED_ROVI_RMATH_H

#include <cmath>              // wg. D_PI und fmod
#include <cstdlib>
#include "global.h"

/*! \defgroup math Mathematische Funktionen und Klassen
 * Hier findet sich einiges Zusätzliches zu den Mathebibliotheken. Viel ist es gerade nicht,
 * ist jedoch praktisch wenn man es bereits hat. Die meisten Sachen hier sind als Templates ausgelegt,
 * würden also auch für Anwendertypen funktionieren, jedoch rate ich davon ab sie auf
 * andere Typen als die Eingebauten (char, short, int, long, long long, float, double, long double)
 * anzuwenden! Zum Beispiel ist mir nicht klar, was gerechnet wird, wenn z.B. einen Anwendertyp Vector3D
 * von Grad nach Radian umgerechnet wird ... (operator*(double) ist dort definiert).
 *
 * Erstaunlicherweise sind die Templates hier meistens schneller (!) als \c #define Makros, jedoch höchstens genauso schnell
 * wie die Funktionen der Standardbibliotheken. Ich beziehe mich dazu z.B. auf abs() (resp. fabs()) aus
 * der C++ Stdlib. Das ist allerdings ein schwieriges Thema. In der C++ Stdlib gibt es ein überladenes
 * abs() für double, float und long double, jedoch nicht für die Integertypen. Die Fließkommaversionen
 * arbeiten schnell und zuverlässig, schließlich sind sie mit Hilfe der C Stdlib Funktionen implementiert.
 * Deswegen habe ich mich dazu entschlossen die fehlenden (nach C++ Standard sollten in der Stdlib auch
 * Versionen für Integertypen vorhanden sein) abs() für Integertypen durch spezialisierte Templates
 * nachzubilden.
 * \author Jan Leupold
 * @{ */

//GLOBAL Constants ============================================================
static const double D_PI = M_PI;
const double DEG_TO_RAD = D_PI / 180.0;
const double RAD_TO_DEG = 180.0 / D_PI;

//! double-Werte kleiner als Eps werden als 0.0 behandelt (von einigen Funktionen).
/**
 *  e.g. some methods in the 3D part of this library rely on this value to test for "zero-ness".
 */
static const double Eps = 1e-12;


template<class T> inline T deg2rad(T x) {
    return x * DEG_TO_RAD;
} //!< Umrechnung von Grad nach Radian.

template<class T> inline T rad2deg(T x) {
    return x * RAD_TO_DEG;
} //!< Umrechnung von Radian nach Grad.

inline double toPosRad(double angle) {
    angle = fmod(angle, 2 * D_PI);
    if (angle < 0) angle += 2.0 * D_PI;
    return angle;
} //!< Liefert einen positiven Radian


inline double toPosRad(int angle) {
    return toPosRad(angle * DEG_TO_RAD);
} //!< Liefert einen positiven Radian

inline double toSymRad(double angle) {
    angle = fmod(angle, 2 * D_PI);
    if (angle > D_PI) angle -= 2.0 * D_PI;
    if (angle < -D_PI) angle += 2.0 * D_PI;
    return angle;
}//!< Liefert einen symmetrischen Radian

inline double toSymRad(int angle) {
    return toSymRad(angle * DEG_TO_RAD);
}//!< Liefert einen symmetrischen Radian

inline double toPosDeg(double angle) {
    return (toPosRad(angle) * RAD_TO_DEG);
}//!< Liefert positive Grad

inline double toSymDeg(double angle) {
    return (toSymRad(angle) * RAD_TO_DEG);
}//!< Liefert symmetrische Grad

//! sign() - Funktion
/*! Gibt +1 (als integer) für alle Werte größer gleich 0 (Achtung operator>=) zurück,
 * ansonsten -1.
 * ACHTUNG: diese Funktion sollte nur solange hier vorhanden sein, solange die C++ Stdlib
 * sie nicht zur Verfügung stellt! */
template<class T> inline int sign(const T x) {
    return (x >= 0) ? 1 : -1;
}

/**
 *  sign() function as needed by some algorithms of numerical recipies
 */
template<typename U>
inline int sign(int a, U b) {
    return (b >= 0.0) ? ::abs(a) : -::abs(a);
}
template<typename U>
inline long sign(long a, U b) {
    return (b >= 0.0) ? ::labs(a) : -::labs(a);
}
template<typename U>
inline long long int sign(long long int a, U b) {
    return (b >= 0.0) ? ::llabs(a) : -::llabs(a);
}
template<typename U>
inline double sign(double a, U b) {
    return (b >= 0.0) ? ::fabs(a) : -::fabs(a);
}
template<typename U>
inline float sign(float a, U b) {
    return (b >= 0.0) ? ::fabsf(a) : -::fabsf(a);
}
template<typename U>
inline long double sign(long double a, U b) {
    return (b >= 0.0) ? ::fabsl(a) : -::fabsl(a);
}

//! Default-Implementierung der abs Funktion
/*! Da es bekanntermaßen (fast) nichts schnelleres gibt als die Stdlib (ha ha ha), greifen
 * wir automatisch auf diese zu wenn möglich. Für die Integertypen gibt es Spezialisierungen
 * dieses Templates.
 * Für den Anwender ist die Sache damit einfach: abs wählt (momentan!) automatisch die schnellste
 * Variante aus und man kann nichts falsch machen. Ist doch toll, oder?
 */
template<class T> inline T abs(T x) {
    return std::abs(x);
}

/*! Spezialisierung für char */
template<> inline char abs<char> (char x) {
    return (x >= 0) ? x : -x;
}

/*! Spezialisierung für short int */
template<> inline short int abs<short int> (short int x) {
    return (x >= 0) ? x : -x;
}

/*! Spezialisierung für int */
template<> inline int abs<int> (int x) {
    return (x >= 0) ? x : -x;
}

/*! Spezialisierung für long int */
template<> inline long int abs<long int> (long int x) {
    return (x >= 0) ? x : -x;
}

/*! Spezialisierung für long long */
template<> inline long long abs<long long> (long long x) {
    return (x >= 0) ? x : -x;
}

//! sqr() liefert das Quadrat von x zurück
template<class T> inline T sqr(T x) {
    return x * x;
}

//! Rundet (Minus-korrekt!!) den gegebenen Wert.
/*! Das scheint's lustigerweise nicht in der libm zu haben ... oder bin ich zu doof um Man-Pages zu lesen?
 * Es gibt zwar eine Man-Page, die sagt mir aber: "Inkludiere math.h und freue Dich auf round(...)" nur
 * leider findet sich in math.h kein Sterbenswort über round geschweige denn in einer vernünftigen anderen
 * Headerdatei (außer in einer ominösen tgmath.h). Der Kompilierprozess bricht also ab, bzw. linkt erst gar nicht.
 * Sind das etwa Überbleibsel aus alten Zeiten ??? Kommt C damit besser zurecht?? na ja, auf jeden Fall
 * gibt es ein round.
 * Daß der Rückgabewert auf integer fest gesetzt ist, finde ich nicht sooo toll (hoffentlich gibt es damit
 * keine riesigen Probleme)! */
template<class T> inline int round(T x) {
    return (x >= 0) ? int(x + 0.5) : int(x - 0.5);
}

//! Testet ob ein Wert (sinnvollerweise ein Fließkommawert) \em ungefähr gleich 0 ist.
template<class T>
inline bool isZero(T x) {
    return abs(x) < Eps;
}

/**
 *  @brief Check a value for "almost equal to zero".
 *  @param x     The value to check
 *  @param limit The zero value to use
 */
template<class T, typename limit_t>
inline bool isZero(T x, const limit_t& limit) {
    return std::abs(x) < limit;
}

/**
 *  @brief returns the minimum value
 *  @param a b The values to compare
 */
template<typename T> inline T& min(const T& a, const T& b) {
    if (b < a) return b;
    else return a;
}

/**
 *  @brief returns the maximum value
 *  @param a b The values to compare
 */
template<typename T> inline T& max(const T& a, const T& b){
    if (a < b) return b;
    else return a;
}

/*! @} */

#endif /* not INCLUDED_ROVI_RMATH_H */
