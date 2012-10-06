/*
 *******************************************************************************
 *  Point.hpp
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

#ifndef POINT_HPP_
#define POINT_HPP_
#include <iostream>

namespace com {
namespace acme {

class Point {

public:
	Point();
	Point(double* pPoint);
	Point( int time,  double* point);
	Point(const Point& p);
	virtual ~Point();
	virtual int getTime();
	virtual int getDimensions();
	virtual double get( int dimension);
private:
	static int NO_TIME;
	int time;
	double* point;

};
}
} /* namespace com */
#endif /* POINT_HPP_ */
