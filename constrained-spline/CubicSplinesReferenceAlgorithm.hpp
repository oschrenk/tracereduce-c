/*
 *******************************************************************************
 *  CubicSplinesReferenceAlgorithm.hpp
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
