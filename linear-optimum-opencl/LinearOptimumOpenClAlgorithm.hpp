/*
 *******************************************************************************
 *  LinearOptimumOpenClAlgorithm.hpp
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
 * Created:     Oct 6, 2012
 *
 * Copyright (c) 2008 Q2WEB GmbH.
 * All rights reserved.
 *
 *******************************************************************************
 */

#ifndef LINEAROPTIMUMOPENCLALGORITHM_HPP_
#define LINEAROPTIMUMOPENCLALGORITHM_HPP_
#include <vector>
#include <iostream>

#if defined(__APPLE__) || defined(__MACOSX)
#include <OpenCL/cl.h>
#else
#include <CL/cl.h>
#endif

#include "Point.hpp"
#include "DeviceHandler.hpp"

namespace com {
namespace acme {
class LinearOptimumOpenClAlgorithm {
public:
	LinearOptimumOpenClAlgorithm();
	virtual ~LinearOptimumOpenClAlgorithm();

	std::vector<Point*> run(std::vector<Point*> trace, double epsilon);
	std::vector<Point*> run();

	bool isValidEdge(int leftOffset, int rightOffset, float fromX, float fromY,
			float toX, float toY);
	std::vector<Point*> getPath(std::vector<int> parentVertexArray);
	std::vector<Point*> runDijkstra(int vertexCount, int edgeCount);
	bool isEmpty(std::vector<int> ints);
private:

	static int DEFAULT_SOURCE_ID;

	size_t DEFAULT_LOCAL_WORKSIZE;

	static std::string SOURCE;
	static std::string KERNEL_CROSSTRACK_HAVERSINE;
	static std::string KERNEL_MAXIMUM;
	static std::string DIJKSTRA_INITIALIZE;
	static std::string DIJKSTRA_SSSP1;
	static std::string DIJKSTRA_SSSP2;

	cl_mem* memObject;
	cl_command_queue queue;
	cl_kernel distanceKernel;
	cl_kernel maximumKernel;
	cl_kernel dijkstraInitializationKernel;
	cl_kernel sssp1Kernel;
	cl_kernel sssp2Kernel;

	std::vector<float> longitudeXCoordinates;
	std::vector<float> latitudeYCoordinates;
	std::vector<int> vertexArray;

	double epsilon;

	std::vector<Point*> trace;

	DeviceHandler* _dh;
	timeval start;
	timeval end;

};
}
} /* namespace com */
#endif /* LINEAROPTIMUMOPENCLALGORITHM_HPP_ */
