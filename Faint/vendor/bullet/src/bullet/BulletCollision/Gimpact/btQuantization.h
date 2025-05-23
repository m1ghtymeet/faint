#ifndef BT_GIMPACT_QUANTIZATION_H_INCLUDED
#define BT_GIMPACT_QUANTIZATION_H_INCLUDED

/*! \file btQuantization.h
*\author Francisco Leon Najera

*/
/*
This source file is part of GIMPACT Library.

For the latest info, see http://gimpact.sourceforge.net/

Copyright (c) 2007 Francisco Leon Najera. C.C. 80087371.
email: projectileman@yahoo.com


This software is provided 'as-is', without any express or implied warranty.
In no event will the authors be held liable for any damages arising from the use of this software.
Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it freely,
subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
*/

#include "LinearMath/btTransform.h"

SIMD_FORCE_INLINE void bt_calc_quantization_parameters(
	btVec3& outMinBound,
	btVec3& outMaxBound,
	btVec3& bvhQuantization,
	const btVec3& srcMinBound, const btVec3& srcMaxBound,
	btScalar quantizationMargin)
{
	//enlarge the AABB to avoid division by zero when initializing the quantization values
	btVec3 clampValue(quantizationMargin, quantizationMargin, quantizationMargin);
	outMinBound = srcMinBound - clampValue;
	outMaxBound = srcMaxBound + clampValue;
	btVec3 aabbSize = outMaxBound - outMinBound;
	bvhQuantization = btVec3(btScalar(65535.0),
								btScalar(65535.0),
								btScalar(65535.0)) /
					  aabbSize;
}

SIMD_FORCE_INLINE void bt_quantize_clamp(
	unsigned short* out,
	const btVec3& point,
	const btVec3& min_bound,
	const btVec3& max_bound,
	const btVec3& bvhQuantization)
{
	btVec3 clampedPoint(point);
	clampedPoint.setMax(min_bound);
	clampedPoint.setMin(max_bound);

	btVec3 v = (clampedPoint - min_bound) * bvhQuantization;
	out[0] = (unsigned short)(v.getX() + 0.5f);
	out[1] = (unsigned short)(v.getY() + 0.5f);
	out[2] = (unsigned short)(v.getZ() + 0.5f);
}

SIMD_FORCE_INLINE btVec3 bt_unquantize(
	const unsigned short* vecIn,
	const btVec3& offset,
	const btVec3& bvhQuantization)
{
	btVec3 vecOut;
	vecOut.setValue(
		(btScalar)(vecIn[0]) / (bvhQuantization.getX()),
		(btScalar)(vecIn[1]) / (bvhQuantization.getY()),
		(btScalar)(vecIn[2]) / (bvhQuantization.getZ()));
	vecOut += offset;
	return vecOut;
}

#endif  // BT_GIMPACT_QUANTIZATION_H_INCLUDED
