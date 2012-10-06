/*
 * SplineInterpolationWithPointOmission.hpp
 *
 *  Created on: 03.10.2012
 *      Author: q2web
 */

#ifndef SPLINEINTERPOLATIONWITHPOINTOMISSION_HPP_
#define SPLINEINTERPOLATIONWITHPOINTOMISSION_HPP_
#include <iostream>

namespace com {
namespace acme {
class SplineInterpolationWithPointOmission {
public:
	SplineInterpolationWithPointOmission();
	virtual ~SplineInterpolationWithPointOmission();
	void compute(float* x, float* y, float* distances, bool* excludes,
			int length, int idToExclude);
};
}
} /* namespace com */
#endif /* SPLINEINTERPOLATIONWITHPOINTOMISSION_HPP_ */
