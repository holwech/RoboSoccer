// #include "helpers.h"
// Created on: 16.11.2010
// Author    : Martin Schaefer (martin.schaefer_at_tum.de)
/* Description:
*/

#ifndef HELPERS_H_
#define HELPERS_H_


//Project Includes ============================================================
#include "global.h" //the debug flag is set in global.h so global.h must be included

//SYSTEM Includes ============================================================
#include <sstream>      //String-Streams
#include <string>       //string functions and string
#include <sys/time.h>   //time
#include <sys/types.h>  //types like uint32_t
#include <cassert>      //makes assert() possible
#include <stdio.h>
//#include <boost/lexical_cast.hpp>

//GLOBAL typedefs ============================================================
typedef float afType;
typedef double fType;

//GLOBAL Functions ============================================================
//template<typename T> inline void safeDelete(T*& p) {
//  delete p;
//  p = 0;
//} //delete 0 is always safe


//#pragma GCC diagnostic ignored "-Wconversion"
//template<typename T> inline std::string anyToString(const T& var) {
// std::ostringstream o;
//  o << var;
//  return o.str();
//}

//template<typename T> inline std::string anyToString(const T& var) {
//  return boost::lexical_cast<std::string>(var);
//}


//#pragma GCC diagnostic error "-Wconversion"

template<class T>
inline void mySwap(T &a, T &b) {
  const T dum(a);
  a = b;
  b = dum;
}

/*Milliseconds senden, dabei die obersten 12 bit nullen
 *1 bit reserve (1000 < 2^10) d.h. noch mal FFFFF Sekunden -> ca. 12Tage
 * (hält der Sensor niemals durch)
 */
//inline int sysTimeMs() {
//  timeval tv;
//  ::gettimeofday(&tv, 0); // aktuelle Zeit in struct tv schreiben
//  //printf("Time 2: %d\n",tv.tv_sec);
//  const int time = (tv.tv_sec & 0x000FFFFF) * 1000 + (int16_t) (tv.tv_usec / 1000);
//  assert(time>=0);
//  return time;
//}

inline int sysTimeMs(const timeval tv) {
  //printf("Time 2: %d\n",tv.tv_sec);
  const int time = (tv.tv_sec & 0x000FFFFF) * 1000 + (int16_t) (tv.tv_usec / 1000);
  assert(time>=0);
  return time;
}

inline long int sysTimeUs() {
  timeval tv;
  ::gettimeofday(&tv, 0); // aktuelle Zeit in struct tv schreiben
  //printf("Time 2: %d\n",tv.tv_sec);
  return tv.tv_usec;
}

//template<typename T> inline const T& min(const T& a, const T& b) {
//  if (b < a) return b;
//  else    return a;
//}
//
//template<typename T> inline const T& max(const T& a, const T& b){
//  if (a < b) return b;
//  else return a;
//}

//Debugging helpers ============================================================
#ifdef DEBUG //the debug flag is set in global.h so global.h must be included before helpers.h
  //Includes for helpers
  #include "unistd.h"  //sleep for debugging only //DEBUG
  #include <cstdarg>   //to use ... parameter

  //Debug helper functions
  template<class T>
  inline void assertInfo(T const& val,char const* pFormat, ...) {
    if(!(val)) {
      va_list args;
      va_start( args, pFormat );
      printf(pFormat, args );
      va_end( args );
      fflush(stdout);
      assert(false);
    }
  }

  template<class T>
  inline void assertRangeInfo(T const& pVal, int const lowtime, int const hightime) {
    assertInfo(pVal ,"Null Pointer in Range: %d-%d\n", lowtime, hightime);
    assertInfo((lowtime <= pVal->getT()),"Pointer Time: %d before Range: %d-%d\n", pVal->getT(), lowtime, hightime);
    assertInfo((pVal->getT()<= hightime),"Pointer Time: %d after  Range: %d-%d\n", pVal->getT(), lowtime, hightime);
  }

  template<class T>
  inline void debugIfPrint(T const& condition, char const* pFormat, ...) {
    if (condition) {
      va_list args;
      va_start(args, pFormat);
      printf(pFormat, args);
      va_end( args );
      fflush(stdout);
    }
  }

  inline void debugPrint(char const* pFormat, ...) {
    va_list args;
    va_start(args, pFormat);
    printf(pFormat, args);
    va_end( args );
    fflush(stdout);
  }

  /*#define assert(expression) \
    if(!(expression)) { \
      printf("Serious error that should be treated!!"); \
      fflush(stdout); \
    } else (void)0
   //the missing ; is left out on purpose
   */
#else
  #pragma GCC diagnostic ignored "-Wunused-parameter"
  inline void assertInfo(bool expression,char const* pFormat, ...){};
  template<class T>
  inline void assertRangeInfo(T const* const pVal, int const lowtime, int const hightime){};
  inline void debugIfPrint(bool condition, char const* pFormat, ...){};
  inline void debugPrint(char const* pFormat, ...){};
  //#pragma GCC diagnostic error "-Wunused-parameter"
#endif

//End Debugging helpers ============================================================

#endif /* HELPERS_H_ */
