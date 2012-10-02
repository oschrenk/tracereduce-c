/*
 *******************************************************************************
 *  DouglasPeuckerReferenceAlgorithm.hpp
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
 * Author:     hadoop
 *
 * Created:     Sep 27, 2012
 *
 * Copyright (c) 2008 Q2WEB GmbH.
 * All rights reserved. 
 *
 *******************************************************************************
 */

#ifndef DOUGLASPEUCKERREFERENCEALGORITHM_HPP_
#define DOUGLASPEUCKERREFERENCEALGORITHM_HPP_

#include <iostream>
#include <list>
#include <vector>
#include "Geometry.hpp"

namespace com {
namespace acme {



class DouglasPeuckerReferenceAlgorithm {
public:
	DouglasPeuckerReferenceAlgorithm();
	DouglasPeuckerReferenceAlgorithm( Geometry* pGeometry);
	virtual ~DouglasPeuckerReferenceAlgorithm();

	std::vector<Point> run(const std::vector<Point> trace, const double epsilon);
	std::vector<Point> run(const std::vector<Point> trace);
private:
	/** The epsilon. */
	double epsilon;

	/** The geometry. */
	 Geometry* geometry;
};
}
} /* namespace com */
#endif /* DOUGLASPEUCKERREFERENCEALGORITHM_HPP_ */