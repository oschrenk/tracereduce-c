/*
 *******************************************************************************
 *  Main.cpp
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
#include "DouglasPeuckerReferenceAlgorithm.hpp"
#include "EuclideanGeometry.hpp"

using namespace com;

int main() {

	std::vector<com::acme::Point> testVec;
	double startArray[] = { 1, 3 };
	com::acme::Point* start = new com::acme::Point(startArray);
	double bArray[] = { 2, 4 };
	com::acme::Point* b = new com::acme::Point(bArray);
	double cArray[] = { 3, 2 };
	com::acme::Point* c = new com::acme::Point(cArray);
	double dArray[] = { 4, 4 };
	com::acme::Point* d = new com::acme::Point(dArray);
	double eArray[] = { 5, 2 };
	com::acme::Point* e = new com::acme::Point(eArray);
	double fArray[] = { 6, 4 };
	com::acme::Point* f = new com::acme::Point(fArray);
	double endArray[] = { 7, 3 };
	com::acme::Point* end = new com::acme::Point(endArray);

	testVec.push_back(*start);
	testVec.push_back(*b);
	testVec.push_back(*c);
	testVec.push_back(*d);
	testVec.push_back(*e);
	testVec.push_back(*f);
	testVec.push_back(*end);

	com::acme::EuclideanGeometry* eg = new com::acme::EuclideanGeometry();
	com::acme::DouglasPeuckerReferenceAlgorithm* dprAlgo =
			new com::acme::DouglasPeuckerReferenceAlgorithm(eg);
	std::vector<com::acme::Point> result = dprAlgo->run(testVec, 1);

	std::cout << " Result Vector Size : " << result.size() << std::endl;
	for (std::vector<com::acme::Point>::iterator it = result.begin();
			it != result.end(); ++it) {
		std::cout << it->get(0) << " : " << it->get(1) << std::endl;
	}

	delete start;
	delete b;
	delete c;
	delete d;
	delete e;
	delete f;
	delete end;

	delete dprAlgo;
	delete eg;
//	std::cout << "Hallo Oli" << std::endl;

	return 0;
}

