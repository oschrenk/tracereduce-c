/*
 *******************************************************************************
 *  CubicSplinesReferenceAlgorithm.cpp
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
#include <limits>
#include <algorithm>
#include "Arrays.hpp"

#include "CubicSplinesReferenceAlgorithm.hpp"

namespace com {
namespace acme {
CubicSplinesReferenceAlgorithm::CubicSplinesReferenceAlgorithm() {
	splineInterpolation = new SplineInterpolationWithPointOmission();
}

CubicSplinesReferenceAlgorithm::~CubicSplinesReferenceAlgorithm() {
	// TODO Auto-generated destructor stub
}

std::vector<Point*> CubicSplinesReferenceAlgorithm::run(
		std::vector<Point*> trace, double epsilon) {

	int length = trace.size();
	float* srcArrayX = new float[length];
	float* srcArrayY = new float[length];

	for (unsigned int i = 0; i < trace.size(); i++) {
		Point* point = trace.at(i);
		srcArrayX[i] = static_cast<float>(point->get(0));
		srcArrayY[i] = static_cast<float>(point->get(1));
	}

	float* distances;
	bool* excludes = Arrays::prefilled(length, false);
	int positionOfMinimum=0;
	do {
		distances = Arrays::prefilled(length - 2,
				std::numeric_limits<float>::max());
		for (int idToExclude = 1; idToExclude < length - 1; idToExclude++) {
			if (!excludes[idToExclude]) {
				splineInterpolation->compute(srcArrayX, srcArrayY, distances,
						excludes, length, idToExclude);
			}
		}

		// distances is length - 2 big
//TODO Arrays.positionOfMinimum??
		positionOfMinimum = Arrays.positionOfMinimum(distances);
		// TODO this check is feels weird, try to remove it
		if (positionOfMinimum >= 0 && distances[positionOfMinimum] <= epsilon) {
			// positions are counted from knot 2, as first and last knot are
			// always included
			excludes[positionOfMinimum + 1] = true;
		}
	} while (positionOfMinimum >= 0);

	int i = 0;
	for (std::vector<Point*>::iterator it = trace.begin(); it != trace.end();
			++it) {
		if (excludes[i]) {
			erase(trace, *it);
			//trace.erase(it);
		}
		i++;
	}
//	Iterator<Point> iterator = trace.iterator();
//	while (iterator.hasNext()) {
//		iterator.next();
//		if (excludes[i]) {
//			iterator.remove();
//		}
//		i++;
//	}

	return trace;
}

void CubicSplinesReferenceAlgorithm::erase(std::vector<Point*>& v,
		Point* point) {
	std::vector<Point*>::iterator it = remove(v.begin(), v.end(), point);
	v.erase(it, v.end());
}

}
} /* namespace com */
