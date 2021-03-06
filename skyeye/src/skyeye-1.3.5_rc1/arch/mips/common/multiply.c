/* Copyright (C)
* 2011 - skyeye team
* This program is free software; you can redistribute it and/or
* modify it under the terms of the GNU General Public License
* as published by the Free Software Foundation; either version 2
* of the License, or (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*
*/

/**
* @file multiply.c
* @brief define mips 64bits multply operations
* @author skyeye team
* @version
* @date 2011-05-05
*/

#include "emul.h"
// Perform long multiplication of x by y.

extern MIPS_State * mstate;

/**
* @brief Unsigned 64 bits multiply operation
*
* @param u one multiplier
* @param v another multiplier
*/
void 
multiply_UInt64(UInt64 u, UInt64 v)
{
    	UInt64 u0 = bits(u, 31,  0);
    	UInt64 u1 = bits(u, 63, 32);

    	UInt64 v0 = bits(v, 31,  0);
    	UInt64 v1 = bits(v, 63, 32);

    	UInt64 w0 = 0;
    	UInt64 w1 = 0;
    	UInt64 w2;
    	UInt64 w3;

    	UInt64 t, k;

    	k = 0;

    	// i == 0, j == 0
    	t  = u0 * v0 + w0 + k;
    	w0 = bits(t, 31,  0);
    	k  = bits(t, 63, 32);

    	// i == 1, j == 0
    	t  = u1 * v0 + w1 + k;
    	w1 = bits(t, 31,  0);
    	k  = bits(t, 63, 32);

    	w2 = k;
    	k  = 0;

    	// i == 0, j == 1
    	t  = u0 * v1 + w1 + k;
    	w1 = bits(t, 31,  0);
    	k  = bits(t, 63, 32);

    	// j == 1, i == 1
    	t  = u1 * v1 + w2 + k;
    	w2 = bits(t, 31,  0);
    	k  = bits(t, 63, 32);

    	w3 = k;

    	// Glue the bits back into full words.
    	mstate->lo = w0 | (w1 << 32);
    	mstate->hi = w2 | (w3 << 32);
}

/**
* @brief 64 bits multiply operation
*
* @param u one multiplier
* @param v another multiplier
*/
void 
multiply_Int64(Int64 u, Int64 v)
{
    	// Compute the sign of the result;
    	int neg;
    	if (u < 0) {
		if (v < 0) {
	    		neg =0;
	    		v = -v;
		} else {
	    		neg = 1;
		}
		u = -u;
    	} else {
		if (v < 0) {
	    	neg = 1;
	    	v = -v;
		} else {
	   		 neg =0;
		}
    	}
    	multiply_UInt64((UInt64)u, (UInt64)v);
    	if (neg) {
		mstate->lo = ~mstate->lo;
		mstate->hi = ~mstate->hi;
		if (++mstate->lo == 0) {
	    		++mstate->hi;
		}
    	}
}
