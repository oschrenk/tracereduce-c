/*
 *******************************************************************************
 *  HaversineDistance.hpp
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
