// #include "error.h"
// Created on: 30.03.2009
// Author    : Martin Schaefer (martin.schaefer_at_tum.de)
/* Description:
 */

#ifndef ERROR_H_
#define ERROR_H_

// SYSTEM INCLUDES
// PROJECT INCLUDES
// LOCAL INCLUDES
#include <string>     //string functions and
#include <stdexcept>    //includes exception handling
#include <sstream>      //String-Streams

//System includes
#include "helpers.h"
/*Sort Class as:
 public:
 //LIFECYCLE
 //OPERATORS
 //OPERATIONS
 //ACCESS
 //INQUIRY
 protected:
 private:
 */

//Exceptions
class TimeError:public std::runtime_error{
  public:
    //explicit TimeError(int time):std::runtime_error(anyToString(time)), mTime(time){};
	explicit TimeError(int time):std::runtime_error("JETZT"), mTime(time){};
  protected:
    int mTime;
};

class NoElError:public std::runtime_error{
  public:
    explicit NoElError(const std::string& s):std::runtime_error(s){};
};

class RangeError:public std::runtime_error{
  public:
    explicit RangeError(const std::string& s):std::runtime_error(s){};
};

// INLINE METHODS
// EXTERNAL REFERENCES

#endif /* ERROR_H_ */
