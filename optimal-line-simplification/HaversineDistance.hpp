/*
 * HaversineDistance.hpp
 *
 *  Created on: 03.10.2012
 *      Author: q2web
 */

#ifndef HAVERSINEDISTANCE_HPP_
#define HAVERSINEDISTANCE_HPP_

#include "Distance.hpp"
#include "Point.hpp"

namespace com {
namespace acme {
class HaversineDistance: public Distance {
public:
	HaversineDistance();
	virtual ~HaversineDistance();


	double distance(Point& from, Point& to);
	double distance(Point& point, Point& lineStart,
			Point& lineEnd);
	static double distance2d(Point& point, Point& lineStart,
			Point& lineEnd);
	static double orthodromeBearing(Point& from, Point& to);


	static double orthodromeBearing(double lon1, double lat1,
			double lon2, double lat2);
	static double haversineDistance(Point& from, Point& to);
	static double haversineDistance(double longitudeFrom, double latitudeFrom,
			double longitudeTo, double latitudeTo);
private:
	static double toRadians(double angdeg);
	static double toDegrees(double angrad);

	static int LATITUDE;
	static int LONGITUDE;
	/** The Constant EARTH_VOLUMETRIC_MEAN_RADIUS. */
	static int EARTH_VOLUMETRIC_MEAN_RADIUS;
};
}
} /* namespace com */
#endif /* HAVERSINEDISTANCE_HPP_ */
