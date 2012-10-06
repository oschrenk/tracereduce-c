/*
 *******************************************************************************
 *  DouglasPeuckerReferenceAlgorithm.cpp
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
#include <math.h>
#include <iostream>
#include <list>
#include "DouglasPeuckerReferenceAlgorithm.hpp"

namespace com {
namespace acme {
DouglasPeuckerReferenceAlgorithm::DouglasPeuckerReferenceAlgorithm() {
	std::cout << "Const DouglasPeuckerReferenceAlgorithm" << std::endl;

}

DouglasPeuckerReferenceAlgorithm::DouglasPeuckerReferenceAlgorithm(
		Distance* distance) {
	std::cout
			<< "Const DouglasPeuckerReferenceAlgorithm(const Distance distance)"
			<< std::endl;
	this->distance = distance;
}

DouglasPeuckerReferenceAlgorithm::~DouglasPeuckerReferenceAlgorithm() {
	std::cout << "Dest DouglasPeuckerReferenceAlgorithm" << std::endl;
}

std::vector<Point*> DouglasPeuckerReferenceAlgorithm::run(
		const std::vector<Point*> trace, const double pEpsilon) {
	std::cout << "DouglasPeuckerReferenceAlgorithm::run(trace,pEpsilon)"
			<< std::endl;
	epsilon = pEpsilon;
	return run(trace);
}

std::vector<Point*> DouglasPeuckerReferenceAlgorithm::run(
		const std::vector<Point*> trace) {
//	std::cout << "DouglasPeuckerReferenceAlgorithm::run(trace) trace size : " <<trace.size()<< std::endl;
	const int traceLength = trace.size();

	// determine the node at which the trace will be separated
	double maximumDistance = 0;
	int maxIndex = -1;

	for (int i = 1; i < traceLength - 1; i++) {
		double tmpDistance = distance->distance(*trace.at(i), *trace.at(0),
				*trace.at(traceLength - 1));
		if (tmpDistance > maximumDistance) {
			maximumDistance = tmpDistance;
			maxIndex = i;
		}
	}

	std::vector<Point*> resultList;

	// if no separation needed, just return start and end
	if (maxIndex == -1 || (maximumDistance <= epsilon)) {
		//resultList = std::vector<Point>;
		resultList.push_back(trace.at(0));
		resultList.push_back(trace.at(traceLength - 1));
	} else {
		// otherwise continue:
		// minimize lower part
		std::vector<Point*> lowerTrace;
		for (int i = 0; i <= maxIndex; i++) {
			lowerTrace.push_back(trace.at(i));
		}
		std::vector<Point*> lowerTraceResults = run(lowerTrace);
		// remove last element, cause this is equal to the first element in
		// the upper part result list
		lowerTraceResults.erase(
				lowerTraceResults.begin() + lowerTraceResults.size() - 1);

		// minimize upper part
		std::vector<Point*> upperTrace;
		for (int i = maxIndex; i < traceLength; i++) {
			upperTrace.push_back(trace.at(i));
		}
		std::vector<Point*> upperTraceResults = run(upperTrace);

		// assemble the result
		std::vector<Point*>::iterator it;
		it = resultList.begin();
		resultList.insert(it, lowerTraceResults.begin(),
				lowerTraceResults.end());
		it = resultList.begin();
		resultList.insert(it + resultList.size(), upperTraceResults.begin(),
				upperTraceResults.end());
		//resultList.addAll(lowerTraceResults);
		//resultList.addAll(upperTraceResults);
	}
	return resultList;
}

}
} /* namespace com */
