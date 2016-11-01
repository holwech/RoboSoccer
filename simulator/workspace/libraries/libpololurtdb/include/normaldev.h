/*
 *   This library realizes a particle filter for localization which is
 *   able to learn the accessibility of spaces in the given map to
 *   enhance the accuracy of the positioning.
 *   Copyright (C) 2010  Julian Straub (Julian.Straub@mytum.de)
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef NORMALDEV_H_
#define NORMALDEV_H_

#include <stdint.h>
#include <cmath>
#include "ran.h"

class Ran;

// normal deviation class -> gives access to normal distributed random numbers
class NormalDev : Ran
{
public:
	NormalDev(double mu, double sig, uint64_t i);
	~NormalDev();

	double dev(void) {
		double u,v,x,y,q;
		do {
			u = doub();
			v = 1.7156*(doub()-0.5);
			x = u - 0.449871;
			y = fabs(v) + 0.386595;
			q = x*x + y*(0.19600*y-0.25472*x);
		} while (q > 0.27597
			&& (q > 0.27846 || (v*v) > -4.*log(u)*(u*u)));
		return mMu + mSig*v/u;
	}

private:
	double mMu,mSig;
	double mStoredval;
};


#endif /* NORMALDEV_H_ */
