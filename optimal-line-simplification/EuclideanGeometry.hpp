/*
 *******************************************************************************
 *  EuclideanGeometry.hpp
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
 * Created:     Sep 28, 2012
 *
 * Copyright (c) 2008 Q2WEB GmbH.
 * All rights reserved. 
 *
 *******************************************************************************
 */

#ifndef EUCLIDEANGEOMETRY_HPP_
#define EUCLIDEANGEOMETRY_HPP_

#include "Geometry.hpp"
#include "Point.hpp"

namespace com {
namespace acme {

class EuclideanGeometry: public Geometry {
public:
	EuclideanGeometry();
	virtual ~EuclideanGeometry();

	virtual double distance(Point* point, Point* lineStart, Point* lineEnd);
	virtual int compare(double a, double b);
private:
	static double length(Point* p);
	static Point* cross(Point* a, Point* b);
	static Point* distanceVector(Point* p, Point* a);
	static double square(double f);

	static double distance3d(Point* point, Point* lineStart, Point* lineEnd);
	static double distance2d(Point* point, Point* lineStart, Point* lineEnd);
};

}
} /* namespace opencl */
#endif /* EUCLIDEANGEOMETRY_HPP_ */
