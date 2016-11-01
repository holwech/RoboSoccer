/*
 * vector2d.h
 *
 *  Created on: Nov 3, 2010
 *      Author: schreibe
 */

#ifndef VECTOR2D_H_
#define VECTOR2D_H_

#include "vector3d.h"

class Vector2d : public Vector3d<fType> {

public:
	explicit Vector2d(const double x, const double y);
	~ Vector2d ();

};

#endif /* VECTOR2D_H_ */
