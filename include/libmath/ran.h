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

#ifndef RAN_H_
#define RAN_H_

#include <stdint.h>

// class for the generation of equal-distributed random numbers
class Ran
{
public:

	Ran(uint64_t j = 4352321);
	~Ran();

	uint64_t int64(void)
	{
		mU = mU * 2862933555777941757LL + 7046029254386353087LL;
		mV ^= mV >> 17; mV ^= mV << 31; mV ^= mV >> 8;
		mW = 4294957665U*(mW & 0xffffffff) + (mW >> 32);
		uint64_t x = mU ^ (mU << 21); x ^= x >> 35; x ^= x << 4;
		return (x + mV) ^ mW;
	};

	double doub(void)
	{
		return 5.42101086242752217E-20 * this->int64();
	};

	int32_t int32(void)
	{
		return (int32_t)this->int64();
	};

private:

	uint64_t mU;
	uint64_t mV;
	uint64_t mW;

};



#endif /* RAN_H_ */
