/*
 * HaversineDistance.cpp
 *
 *  Created on: 03.10.2012
 *      Author: q2web
 */
#include <cmath>
#include <iostream>
#include <stdexcept>
#include "HaversineDistance.hpp"

namespace com {
namespace acme {

int HaversineDistance::LATITUDE = 1;
int HaversineDistance::LONGITUDE = 0;
/** The Constant EARTH_VOLUMETRIC_MEAN_RADIUS. */
int HaversineDistance::EARTH_VOLUMETRIC_MEAN_RADIUS = 6371000;

HaversineDistance::HaversineDistance() {
	std::cout << "Const HaversineDistance" << std::endl;

}

HaversineDistance::~HaversineDistance() {
	std::cout << "Dest HaversineDistance" << std::endl;
}

/*
 * @see de.q2web.gis.core.api.Distance#distance(de.q2web.gis.trajectory
 * .core.api.Point, de.q2web.gis.core.api.Point)
 */
/**
 * Lng-Lat[-Height] in spherical coordinates.
 *
 * @param from
 *            the from
 * @param to
 *            the to
 * @return the double
 */
double HaversineDistance::distance(Point& from, Point& to) {
	throw std::invalid_argument("Not yet implemented");
}

/*
 * (non-Javadoc)
 *
 * @see de.q2web.gis.core.api.Distance#distance(de.q2web.gis.trajectory
 * .core.api.Point, de.q2web.gis.core.api.Point,
 * de.q2web.gis.core.api.Point)
 */
double HaversineDistance::distance(Point& point, Point& lineStart,
		Point& lineEnd) {
	return std::abs(distance2d(point, lineStart, lineEnd));
}

double HaversineDistance::distance2d(Point& point, Point& lineStart,
		Point& lineEnd) {

	double r = EARTH_VOLUMETRIC_MEAN_RADIUS;
	double b12 = orthodromeBearing(lineStart, lineEnd);
	double b13 = orthodromeBearing(lineStart, point);
	double d13 = haversineDistance(lineStart, point);

	// @formatter:off
	double dt = //
			std::asin( //
					std::sin(d13 / r) //
					* std::sin(b13 - b12) //
							) * r; //
	// @formatter:on

	return dt;
}

double HaversineDistance::orthodromeBearing(Point& from, Point& to) {
	// read longitude as x
	// read latitude as y
	return orthodromeBearing(from.get(0), from.get(1), to.get(0), to.get(1));
}
double HaversineDistance::toRadians(double angdeg) {
	return angdeg / 180.0 * M_PI;

}

double HaversineDistance::toDegrees(double angrad) {
	return angrad * 180.0 / M_PI;
}

double HaversineDistance::orthodromeBearing(double lon1, double lat1,
		double lon2, double lat2) {
	lat1 = HaversineDistance::toRadians(lat1);
	lat2 = HaversineDistance::toRadians(lat2);

	double deltaLongitude = HaversineDistance::toRadians(lon2 - lon1);

	double y = std::sin(deltaLongitude) * std::cos(lat2);
	double x = std::cos(lat1) * std::sin(lat2)
			- std::sin(lat1) * std::cos(lat2) * std::cos(deltaLongitude);

	return std::atan2(y, x);
}

double HaversineDistance::haversineDistance(Point& from, Point& to) {
	// read x longitude
	// read y as latitude

	return haversineDistance(from.get(LONGITUDE), from.get(LATITUDE),
			to.get(LONGITUDE), to.get(LATITUDE));
}

double HaversineDistance::haversineDistance(double longitudeFrom,
		double latitudeFrom, double longitudeTo, double latitudeTo) {
	double deltaLatitude = toRadians(latitudeFrom - latitudeTo);
	double deltaLongitude = toRadians((longitudeFrom - longitudeTo));

	double sinusHalfDeltaLatitude = std::sin(deltaLatitude / 2);
	double sinusHalfDeltaLongitude = std::sin(deltaLongitude / 2);

	double a = sinusHalfDeltaLatitude * sinusHalfDeltaLatitude
			+ std::cos(toRadians(latitudeFrom))
					* std::cos(toRadians(latitudeTo)) * sinusHalfDeltaLongitude
					* sinusHalfDeltaLongitude;
	double c = 2 * std::atan2(std::sqrt(a), std::sqrt(1 - a));

	return EARTH_VOLUMETRIC_MEAN_RADIUS * c;
}

}
} /* namespace com */
