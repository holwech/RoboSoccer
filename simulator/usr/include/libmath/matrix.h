// #include "matrix.h"
// Created on: 26.03.2009
// Author    : Martin Schaefer (martin.schaefer_at_tum.de)
/* Description:
 */

#ifndef MATRIX_H_
#define MATRIX_H_

#include "settings.h"

#include <cmath>     //math functions
#include <algorithm> //standard stl algos e.g. swap
#include <stdexcept> //exception handling
#include <cstring>   //for memcpy
#include <iostream>  //for matrix printing



const int cols = 3;
const int rows = 3;

/*!Matrix3d stellt 3 dimensionale fType Matrizen dar.
 * Die Organisation (intern) ist der sog. "column-major mode", d.h. im Speicher sind die Spalten
 * nacheinander angeordnet. Wegen (zumindest von mir so empfundenen) intuitiverer Handhabung
 * erfolgen Parameterangaben jedoch in der Form: erst die Zeile, dann die Spalte (also "row-major mode").
 * Übrigens: OpenGL z.B. erwartet Daten im "row-major mode"!
 *
 * TODO Denkbar wäre evtl. das eine Matrix3d ein Gleichungssystem der Form Ax = b lösen kann.
 * original author:  Jan Leupold
 */
template<typename T> class Vector3d;
typedef Vector3d<fType> Vector3df;

class Matrix3d {

  protected:
    // Matrix3d = m11 m12 m13
    //          m21 m22 m23
    //          m31 m32 m33
    // liegt so im Speicher: m11 m21 m31 m12 m22 m32 m13 m23 m33
    //                       1. Spalte,  2. Spalte,  3. Spalte
    fType mp_elements[rows * cols];

  public:

    /*
     *! Einfachste Arte der Konstruktion. Der optionale Parameter "def" gibt an,
     *  mit welchem Wert sämtliche Felder der Matrix3d gefüllt werden sollen.
     */
    explicit Matrix3d();

    explicit Matrix3d(const fType& def);

    /*
     *! Erzeugt eine Diagonalmatrix mit den 3 vorgegebenen Elementen
     */
    explicit Matrix3d(const fType& one,const fType& two,const fType& three);

    /*
     *! Erzeugt eine Diagonalmatrix mit den 3 vorgegebenen Elementen
     */
    explicit Matrix3d(const Vector3df& diag);

    /*
     * ! Spezial-Konstruktion aus 3 Vector3D.
     * Die Parameter "v1", "v2" und "v3" könnten z.B. ein Koordinatensystem bilden
     * operator() benützt, weil dadurch auch größere Matrizen beschrieben werden
     * können und hier die Schnelligkeit nicht sooo wichtig sein sollte
     */
    explicit Matrix3d(const Vector3df& v1, const Vector3df& v2, const Vector3df& v3);

    /*! Konstruktion aus einem 1-dimensionalem Werte-Array.
     *  Die Werte im Array müssen so vorliegen, dass erst alle Werte der ersten
     *  Spalte kommen, dann alle Werte der zweiten Spalte, ...
     *  also "column-major mode".
     *  Parameter "values" wird eh als Zeiger übergeben (C-Style-Array),
     *  d.h. Referenzparameter ist hier eh nicht notwendig
     */
    explicit Matrix3d(const fType values[]) {
      memcpy(mp_elements, values, rows * cols * sizeof(fType));
    }

    /*! Copy on Init Konstruktor bei gleichem Matrix3d-Werttyp */
    Matrix3d(const Matrix3d& other);

    //INLINE METHODS and OPERATORS ----------------------------------------------

    /*! Copy on Assignment Konstruktor bei gleichem Matrix3d-Werttyp */
    Matrix3d& operator=(const Matrix3d& other) {
      if (this != &other)
      {
        memcpy(mp_elements, other.mp_elements, rows * cols * sizeof(fType));
      }
      return *this;
    }

    int getRows() const {
      return rows;
    }

    int getCols() const {
      return cols;
    }

    fType getNorm() const {
      return sqrt(operator() (0,0) * operator() (0,0)
                  + operator() (1,1)* operator() (1,1)
                  + operator() (2,2) * operator() (2,2));
      }

    /*operator() (0,0) is first element
     * ! Zugriff (nur lesend) auf ein Element (Angabe von Reihe und Spalte,
     * wie Konstruktion aus 2-dimensionalem Array)
     * */
    const fType& operator()(int row, int col) const {
      return mp_elements[col * rows + row];
    }

    /*
     * ! Zugriff (lesend und schreibend) auf ein Element
     * (Angabe von Reihe und Spalte, wie Konstruktion aus 2-dimensionalem Array)
     */
    fType& operator()(int row, int col) {
      return mp_elements[col * rows + row];
    }

    /*! Zugriff (nur lesend) auf ein Element (Anordnung der Elemente: siehe Konstruktion aus 1-dimensionalem Array) */
    const fType& operator[](int index) const {
      return mp_elements[index];
    }

    /*! Zugriff (lesend und schreibend) auf ein Element (Anordnung der Elemente: siehe Konstruktion aus
     * 1-dimensionalem Array) */
    fType& operator[](int index) {
      return mp_elements[index];
    }

    /*! Addition: Dieser Matrix3d wird die andere "hinzuaddiert".
     * siehe auch die obige Addition. */
    Matrix3d& operator+=(const Matrix3d& other) {
      for (int elem = 0; elem < rows * cols; ++elem)
      {
        mp_elements[elem] += other[elem];
      }
      return *this;
    }

    /*! Subtraktion: Dieser Matrix3d wird die andere "wegsubtrahiert".
     * siehe auch die obige Subtraktion. */
    Matrix3d& operator-=(const Matrix3d& other) {
      for (int elem = 0; elem < rows * cols; ++elem)
      {
        mp_elements[elem] -= other[elem];
      }
      return *this;
    }

    /*! Multiplikation mit einem Skalar, von "rechts" */
    Matrix3d& operator*=(fType scalar) {
      for (int elem = 0; elem < rows * cols; ++elem)
      {
        mp_elements[elem] *= scalar;
      }
      return *this;
    }

    Matrix3d& clear(const fType& def = 0) {
      for (int elem = 0; elem < rows * cols; ++elem)
        mp_elements[elem] = def;
      return *this;
    }

    /*
     *  Erzeugt eine Identität
     */
    Matrix3d& Identity() {
      assert(cols == rows);
      clear();
      for (int diag = 0; diag < rows; ++diag)
        operator()(diag, diag) = 1;
      return *this;
    }

    /*! Gibt eine transponierte Kopie dieser Matrix3d zurück.
     * Weil der Größentyp der Rückgabe-Matrix3d anders ist,
     * kann die Operation nicht innerhalb dieser Matrix3d stattfinden, sondern muss eine
     * neue Matrix3d zurückgeben. Als Ausgleich ist die Methode "const".
     * Rückgabe über Referenz geht nicht, weil diese statisch angelegt werden müsste
     * Rückgabe "const" weil sonst a.Transpose() = b möglich wäre!
     */
    const Matrix3d Transpose() const {
      Matrix3d result;
      // stur alles kopieren ...
      for (int row = 0; row < rows; ++row)
      {
        for (int col = 0; col < cols; ++col)
        {
          result(col, row) = operator()(row, col);
          // aber halt row und col vertauscht
        }
      }
      return result;
    }

    /*! Transponiert diese Matrix3d. Das geht leider nur dann,
     * wenn die Matrix3d genau so viele Zeilen wie Spalten hat.
     * Hier geht die Rückgabe per Referenz, weil wir uns ja selber zurückgeben :)
     */
    Matrix3d& TransposeThis() {
      // Es können nur NxN Matrizen in sich selber transponiert werden
      assert(cols == rows);
      for (int row = 0; row < rows; ++row)
      {
        for (int col = row + 1; col < cols; ++col)
        {
          // nur eine "Hälfte" austauschen, sonst stimmt's nicht!
          std::swap(operator()(row, col), operator()(col, row));
        }
      }
      return *this;
    }

    /* Diese Matrix3d invertieren. Das geht leider nur dann,
     * wenn die Matrix3d genauso viele Zeilen wie Spalten hat.
     * Hier geht die Rückgabe per Referenz, weil wir uns ja selber zurückgeben :)
     */
    Matrix3d& InvertThis() throw (std::runtime_error);

    /* Die Diagonlae der Matrix als Float Vector zurückgeben
     *
     */
    Vector3df diag() const;
};

/**
 *! Ausgabe einer Matrix3d auf einem std::ostream.
 *  braucht kein "friend" sein, da mit öffentlicher Schnittstelle implementierbar
 */
inline std::ostream& operator<<(std::ostream& out, const Matrix3d& matrix) {
  for (int r = 0; r < rows; ++r)
  {
    //out << "Row " << r << ": ";
    for (int c = 0; c < cols; ++c)
    {
      out << matrix(r, c) << " ";
    }
    out << '\n';//std::endl;
  }
  return out;
}

///**
// *! Multiplikation einer Matrix3d mit einem Vector3D. Ergebnis ist wieder ein
// * Vector3D. Wenn die Matrix3d nicht 3 Zeilen und Spalten hat,
// * geht die Operation schief.
// * Es kann keine Referenz zurückgegeben werden,
// * da diese statisch angelegt werden müsste
// * global, damit jeder vom Operator profitieren kann, auch abgeleitete Klassen.
// */
//inline Vector3df operator*(const Matrix3d& matrix, const Vector3df& vec) {
//  assert(rows == 3);
//  assert(cols == 3);
//  fType x1 = matrix[0] * vec.getX() + matrix[3] * vec.getY() + matrix[6]
//      * vec.getZ();
//  fType x2 = matrix[1] * vec.getX() + matrix[4] * vec.getY() + matrix[7]
//      * vec.getZ();
//  fType x3 = matrix[2] * vec.getX() + matrix[5] * vec.getY() + matrix[8]
//      * vec.getZ();
//  return Vector3df(x1, x2, x3);
//}

/**
 *! Multiplikation mit einer anderen Matrix3d. Die Matrix3d "first" ist die "linke".
 * Das heisst, bei R = first * other (R = Rückgabematrix).
 * Der Typ der übergebenen Matrix3d kann sich von der "first" Matrix3d unterscheiden,
 * die zurückgegebene Matrix3d hat jedoch den gleichen Typ wie die "first" Matrix3d.
 * Die beiden Typen müssen sich lediglich multiplizieren lassen können.
 * Weil sich der Größentyp ändert, muss eine Kopie zurückgeben werden,
 * dafür aber "const" Methode
 * "const" Rückgabewert, damit nicht (a*b) = c geschrieben werden kann
 * Ist nicht als globale Funktion definiert,
 * weil sich kaum die Gelegenheit bieten wird,
 * dass bei einer Matrix3d-Multiplikation einer der beiden Multiplikanden
 * implizit generiert wird, oder?
 * global, damit jeder von diesem Operator profitieren kann,
 * auch abgeleitete Klassen.
 */
inline const Matrix3d operator*(const Matrix3d& first, const Matrix3d& second) {
  Matrix3d result;
  for (int row_this = 0; row_this < rows; ++row_this) // alle Zeilen dieser Matrix3d durchlaufen
  {
    for (int col_other = 0; col_other < rows; ++col_other) // nicht wundern, dass col_other mit rows verglichen wird
    {
      fType temp = 0;
      for (int col_this = 0; col_this < cols; ++col_this)
        temp += first(row_this, col_this) * second(col_this, col_other);
      result(row_this, col_other) = temp;
    }
  }
  return result;
}

/**
 *! Multiplikation einer Matrix3d mit einem Skalar.
 * Die Matrix3d "matrix" ist die "linke".
 * Das heisst, bei R = matrix * scalar (R = Rückgabematrix).
 * Der Typ des Skalars kann sich nicht von "matrix" unterscheiden, da sonst zu viele Operatoren
 * vorhanden sind :)
 * Existiert wahlweise auch als operator*= als Methode,
 * weil sich weder der Größen- noch der Datentyp verändert.
 * "const" Rückgabewert, damit nicht (A*b) = C geschrieben werden kann.
 */
inline const Matrix3d operator*(Matrix3d matrix, const fType& scalar) {
  for (int elem = 0; elem < rows * cols; ++elem)
    matrix[elem] *= scalar;
  return matrix;
}

/*
 *! siehe operator* (Matrix3d, Skalar), hier jedoch mit vertauschten Argumenten
 * */
inline const Matrix3d operator*(const fType& scalar, const Matrix3d& matrix) {
  return operator*(matrix, scalar);
}

/**
 *! Division einer Matrix3d mit einem Skalar.
 * Die Matrix3d "matrix" ist die "linke".
 * Das heisst, bei R = matrix / scalar (R = Rückgabematrix).
 * Der Typ des Skalars kann sich nicht von "matrix" unterscheiden,
 * da sonst zu viele Operatoren
 * vorhanden sind :)
 * "const" Rückgabewert, damit nicht (A/b) = C geschrieben werden kann.
 */
inline const Matrix3d operator/(Matrix3d matrix, const fType& scalar) {
  assert(scalar);
  for (int elem = 0; elem < rows * cols; ++elem)
    matrix[elem] /= scalar;
  return matrix;
}

/**
 *! Vergleichsoperator für zwei Matrizen vom gleichen Größentyp.
 *  Die Werttypen darf unterschiedlich sein.
 */
inline bool operator==(const Matrix3d& left, const Matrix3d& right) {
  for (int elem = 0; elem < rows * cols; ++elem)
    if (left[elem] != right[elem]) return false;
  return true;
}

inline bool operator!=(const Matrix3d& left, const Matrix3d& right) {
  return !operator==(left, right);
}

/*
 *! Addition zweier Matrizen: Rückgabewert = Linke Matrix3d plus die rechte Matrix3d.
 * Das heisst, R = left + right (R = Rückgabematrix).
 * Die Werttypen der Matrizen können unterschiedlich sein,
 * die Größentypen jedoch nicht. Die zurückgegebene Matrix3d hat den
 * gleichen Typ wie "left". Die Werttypen müssen sich addieren lassen können!
 */
inline const Matrix3d operator+(const Matrix3d& left, const Matrix3d& right) {
  Matrix3d result(left); // Kopie von "left" (= erster Summand)
  for (int elem = 0; elem < rows * cols; ++elem)
    result[elem] += right[elem];
  return result;
}

/*
 * ! Subtraktion zweier Matrizen: Rückgabewert = Linke Matrix3d minus die rechte Matrix3d.
 * Das heisst, bei R = left - right (R = Rückgabematrix). Der Typ der Rückgabematrix
 * ist gleich dem Typ von "left". Die Werttypen der beiden Matrizen können sich unterscheiden,
 * sie müssen sich lediglich voneinander subtrahieren lassen.
 */
inline const Matrix3d operator-(const Matrix3d& left, const Matrix3d& right) {
  Matrix3d result(left); // Kopie von "left"
  for (int elem = 0; elem < rows * cols; ++elem)
    result[elem] -= right[elem];
  return result;
}




#endif /* MATRIX_H_ */
