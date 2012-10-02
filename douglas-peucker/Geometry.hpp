/*
 *******************************************************************************
 *  Geometry.hpp
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

#ifndef GEOMETRY_HPP_
#define GEOMETRY_HPP_

#include "Point.hpp"


namespace com {
namespace acme {

class Geometry {
public:
	~Geometry(){}
	virtual double distance(Point point, Point lineStart, Point lineEnd)=0;
	virtual int compare(double a, double b)=0;
};
}
} /* namespace com */
#endif /* GEOMETRY_HPP_ */
