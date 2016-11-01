// #include "global.h"
// Created on: 13.02.2009
// Author    : Martin Schaefer (martin.schaefer_at_tum.de)
/* Description:
*/

#ifndef MYGLOBAL_H_
#define MYGLOBAL_H_

using namespace std;

//SYSTEM Includes ============================================================
#include "stdint.h"  //Standard printf
#include <iostream>  //IO-Streams
#include <sstream>   //String-Streams
#include <sys/time.h>
#include <cmath>     //math functions
#include <algorithm> //standard stl algos e.g. swap
#include <exception> //includes exception handling
#include <stdexcept> //includes exception handling
#include <cassert>   //makes assert() possible
#include "string.h"  //string functions and
#include "error.h"
#include "glob.h"

//PROJEKT Includes ============================================================
#include "rmath.h"

//GLOBAL Defines ============================================================
//#define DEBUG

#ifdef DEBUG
  #include "unistd.h"  //sleep for debugging only //DEBUG
  #define DEBUG_PRINT(format, args...) printf(format, ##args);fflush(stdout);
  //#define PRINT_DEBUG(format, args...) printf("\033[31m"format"\033[0m", ##args);
   #define DEBUG_IF(condition) if(condition)
#else
  #define DEBUG_PRINT(format, args...)
  #define DEBUG_IF (condition)
#endif

//GLOBAL Functions ============================================================
template<typename T> inline void safeDelete(T*& p) {delete p; p = 0;}

/*Sytemzeit in Milliseconds senden, dabei die obersten 12 bit nullen
 *2 bit reserve (1000 < 2^10) d.h. noch 3 mal FFFFF Sekunden -> ca. 3*12Tage
 */
inline uint32_t sysTimeMs(){
  timeval tv;
  ::gettimeofday(&tv, 0); // aktuelle Zeit in struct tv schreiben
  //printf("Time 2: %d\n",tv.tv_sec);
  return (tv.tv_sec & 0x000FFFFF) * 1000 + (uint32_t)(tv.tv_usec / 1000);
}



#endif /* MYGLOBAL_H_ */
