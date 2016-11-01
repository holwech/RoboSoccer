// #include "array.h"
// Created on: 09.02.2009
// Author    : Martin Schaefer (martin.schaefer_at_tum.de)
/* Description:
 * Generic index-safe array with stored length
 * start pointer and end pointer*
 *
*/

#ifndef ARRAY_H_
#define ARRAY_H_

//#include "stdlib.h"
//#include "../includes.h"
#include "error.h"
//#include "vector3d.h"

template<typename T>
class Array{
  public:
    Array(int n);

    template<typename U>
    Array(const Array<U>& old);
    Array(int n, const T& init);
    ~Array();
    int size() const {return mSize;}
    void fillArray(const T& val){
      for (int i=0; i<mSize; ++i) mpArray[i] = val;
    };

    T const* const& begin() const { return mpBegin;} //returns a pointer to the start of the array
    T const* const& end() const { return mpEnd;}                     //returns a poniter BEHIND the end of the array
    T const* const& getReadPointer() const{return mpBegin;};

    T &operator[](int i)
    {
      if(i> mSize-1) throw RangeError("Index ist außerhalb des Bereichs");
      return mpArray[i];
    }

    const T &operator[](int i) const {
      if(i> mSize-1) throw RangeError("Index ist außerhalb des Bereichs");
      return mpArray[i];
    }

  protected:
    T* const mpArray;
    const T* mpBegin;
    const T* const mpEnd;
    const int mSize;
  private:
    Array &operator=(const Array &);  // not provided
};

template<typename T>
Array<T>::Array(int n):
 mpArray(new T[n]),mpBegin(mpArray),
 mpEnd(mpArray+n),mSize(n){
}

template<typename T>
Array<T>::Array(int n,const T& init):
 mpArray(new T[n]),mpBegin(mpArray),
 mpEnd(mpArray+n),mSize(n){
 fillArray(init);
}

template<typename T>
template<typename U>
Array<T>::Array(const Array<U>& old): mpArray(new T[old.size()]), mpBegin(mpArray),
mpEnd(mpArray+old.size()),mSize(old.size()){
  for (int i = 0;i< mSize;++i){
    mpArray[i] = old[i];
  }

}

template<typename T>
Array<T>::~Array(){
  delete[] mpArray;
}




#endif /* ARRAY_H_ */
