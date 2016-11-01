// #include "global.h"
// Created on: 13.02.2009
// Author    : Martin Schaefer (martin.schaefer_at_tum.de)
/* Description:
*/

//#pragma GCC diagnostic ignored "-Wunused"

#ifndef SETTINGS_H_
#define SETTINGS_H_

//Global Includes ============================================================
#include <cassert>   //makes assert() possible


//GLOBAL typedefs ============================================================
typedef float  afType;
typedef double fType;


//template<class T>
//inline void mySwap(T &a, T &b) {const T dum(a); a=b; b=dum;}

/* Test function that includes a simple timeout saving the system
 * from starvation. Currently only for debug purposes
 */
//GLOBAL Classes ============================================================

/*#define assert(expression) \
  if(!(expression)) { \
    printf("Serious error that should be treated!!"); \
    fflush(stdout); \
  } else (void)0
  //the missing ; is left out on purpose
*/

#endif /* MYGLOBAL_H_ */
