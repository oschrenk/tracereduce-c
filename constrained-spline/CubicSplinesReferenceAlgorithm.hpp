/*
 * CubicSplinesReferenceAlgorithm.hpp
 *
 *  Created on: 03.10.2012
 *      Author: q2web
 */

#ifndef CUBICSPLINESREFERENCEALGORITHM_HPP_
#define CUBICSPLINESREFERENCEALGORITHM_HPP_

#include <iostream>
#include <vector>
#include "SplineInterpolationWithPointOmission.hpp"
#include "Point.hpp"
namespace com {
namespace acme {
class CubicSplinesReferenceAlgorithm {
public:
	CubicSplinesReferenceAlgorithm();
	virtual ~CubicSplinesReferenceAlgorithm();
	 std::vector<Point*> run(std::vector<Point*> trace,  double epsilon);
	 void erase(std::vector<Point*>& v, Point* point);
private:
	SplineInterpolationWithPointOmission* splineInterpolation;
};
}
} /* namespace com */
#endif /* CUBICSPLINESREFERENCEALGORITHM_HPP_ */
