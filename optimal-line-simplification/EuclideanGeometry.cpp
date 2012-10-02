/*
 *******************************************************************************
 *  EuclideanGeometry.cpp
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
#include <cmath>
#include <iostream>
#include <stdexcept>
#include "EuclideanGeometry.hpp"

namespace com {
namespace acme {

EuclideanGeometry::EuclideanGeometry() {
	std::cout << "Const EuclideanGeometry" << std::endl;

}

EuclideanGeometry::~EuclideanGeometry() {
	std::cout << "Dest EuclideanGeometry" << std::endl;
}

double EuclideanGeometry::distance(Point* point, Point* lineStart,
		Point* lineEnd) {
//	std::cout << "EuclideanGeometry::distance dimensions: "<<lineStart->getDimensions() << std::endl;

	int dimensions = lineStart->getDimensions();
	if (dimensions == 2) {
		return distance2d(point, lineStart, lineEnd);
	}

	if (dimensions == 3) {
		return distance3d(point, lineStart, lineEnd);
	}

	throw std::invalid_argument("Invalid dimensions");

	//throw new IllegalArgumentException("Invalid dimensions");
}

/**
 * is again given by projecting r onto v, giving
 *
 * denom = (sqrt((x_2-x_1)^2+(y_2-y_1)^2))
 * distance=(|(x_2-x_1)(y_1-y_0)-(x_1-x_0)(y_2-y_1)|)/
 *
 *
 * @return
 */
double EuclideanGeometry::distance2d(Point* point, Point* lineStart,
		Point* lineEnd) {
//	std::cout << "EuclideanGeometry::distance2d" << std::endl;
	double x_0 = point->get(0);
	double y_0 = point->get(1);
	double x_1 = lineStart->get(0);
	double y_1 = lineStart->get(1);
	double x_2 = lineEnd->get(0);
	double y_2 = lineEnd->get(1);

	double nom = std::abs(
			(x_2 - x_1) * (y_1 - y_0) - (x_1 - x_0) * (y_2 - y_1));
	double denom = std::sqrt(
			(x_2 - x_1) * (x_2 - x_1) + (y_2 - y_1) * (y_2 - y_1));

	return nom / denom;
}

/**
 * Distance3d.
 *
 * @param point
 *            the point
 * @param lineStart
 *            the line start
 * @param lineEnd
 *            the line end
 * @return the float
 */
double EuclideanGeometry::distance3d(Point* point, Point* lineStart,
		Point* lineEnd) {
//	std::cout << "EuclideanGeometry::distance3d" << std::endl;
	Point* lineVector = distanceVector(lineEnd, lineStart);
	return length(cross(distanceVector(point, lineStart), lineVector))
			/ length(lineVector);
}

/*
 * @see de.q2web.gis.trajectory.core.api.Geometry#compare(double, double)
 */

int EuclideanGeometry::compare(double a, double b) {
	if (a < b) {
		return -1; // Neither val is NaN, thisVal is smaller
	}
	if (a > b) {
		return 1; // Neither val is NaN, thisVal is larger
	}
//	std::cout << "EuclideanGeometry::compare" << std::endl;
	// Cannot use doubleToRawLongBits because of possibility of NaNs.
//		 long thisBits = Double.doubleToLongBits(a);
//		 long anotherBits = Double.doubleToLongBits(b);

	long thisBits = 1;
	long anotherBits = 2;
	return (thisBits == anotherBits ? 0 : // Values are equal
			(thisBits < anotherBits ? -1 : // (-0.0, 0.0) or (!NaN, NaN)
					1)); // (0.0, -0.0) or (NaN, !NaN)
}

/**
 * Square.
 *
 * @param f
 *            the f
 * @return the float
 */

double EuclideanGeometry::square(double f) {
	return f * f;
}

/**
 * Calculates p-a.
 *
 * @param p
 *            the p
 * @param a
 *            the a
 * @return the point
 */
Point* EuclideanGeometry::distanceVector(Point* p, Point* a) {
	int dimensions = p->getDimensions();
	double* c = new double[dimensions];
	for (int i = 0; i < dimensions; i++) {
		c[i] = p->get(i) - a->get(i);
	}
	return new Point(c);

}

/**
 * Cross.
 *
 * @param a
 *            the a
 * @param b
 *            the b
 * @return the point
 */
Point* EuclideanGeometry::cross(Point* a, Point* b) {
	double c[] = { a->get(1) * b->get(2) - a->get(2) * b->get(1), a->get(2)
			* b->get(0) - a->get(0) * b->get(2), a->get(0) * b->get(1)
			- a->get(1) * b->get(0) };
//	double* c;
	return new Point(c);
}

/**
 * Length.
 *
 * @param p
 *            the p
 * @return the float
 */
double EuclideanGeometry::length(Point* p) {
	int dimensions = p->getDimensions();
	double sumSquared = 0;
	for (int i = 0; i < dimensions; i++) {
		sumSquared = sumSquared + square(p->get(i));
	}
	return std::sqrt(sumSquared);
}
}
} /* namespace opencl */
