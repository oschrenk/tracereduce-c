/*
 *******************************************************************************
 *  Point.cpp
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
 * Created:     Sep 26, 2012
 *
 * Copyright (c) 2008 Q2WEB GmbH.
 * All rights reserved. 
 *
 *******************************************************************************
 */
#include <iostream>
#include "Point.hpp"

namespace com {
namespace acme {

int Point::NO_TIME = -1;

Point::Point() {
	std::cout << "Const Point" << std::endl;
}

Point::Point(double* pPoint) {
	std::cout << "Const Point(double* pPoint)" << std::endl;
	this->point = pPoint;
}

Point::Point(int time, double* point) {
	this->time = time;
	this->point = point;
}

Point::~Point() {
	std::cout << "Dest Point" << std::endl;
}
/**
 * Gets the number of dimensions.
 *
 * @return the number of dimensions
 */
int Point::getDimensions() {
	//return sizeof(this->point);
	return 2;
}

/**
 * Gets the coordinate for the given dimension (starting with dimension
 * <code>0</code>)
 *
 * @param dimension
 *            the dimension
 * @return the double
 */
double Point::get(int dimension) {
	return this->point[dimension];
}

int Point::getTime() {
	return this->time;
}

}
} /* namespace com */
