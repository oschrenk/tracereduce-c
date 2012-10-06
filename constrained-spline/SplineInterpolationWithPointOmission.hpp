/*
 *******************************************************************************
 *  SplineInterpolationWithPointOmission.hpp
 *
 *******************************************************************************
 *
 * Copyright:   Q2WEB GmbH
 *              quality to the web
 *
 *              Tel  : +49 (0) 211 / 159694-00	Kronprinzenstr. 82-84
 *              Fax  : +49 (0) 211 / 159694-09	40217 Duesseldorf
 *              eMail: info@q2web.de	http://www.q2web.de
 *
 *
 * Author:     Oliver Schrenk <oliver.schrenk@q2web.de>
 *
 * Copyright (c) 2008 Q2WEB GmbH.
 * All rights reserved.
 *
 *******************************************************************************
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
