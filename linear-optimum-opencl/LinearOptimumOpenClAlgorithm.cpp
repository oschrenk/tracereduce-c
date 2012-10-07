/*
 *******************************************************************************
 *  LinearOptimumOpenClAlgorithm.cpp
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
#include <stdexcept>
#include <algorithm>
#include "LinearOptimumOpenClAlgorithm.hpp"

namespace com {
namespace acme {

inline int
pow2roundup (int x)
{
    if (x < 0)
        return 0;
    --x;
    x |= x >> 1;
    x |= x >> 2;
    x |= x >> 4;
    x |= x >> 8;
    x |= x >> 16;
    return x+1;
}

int LinearOptimumOpenClAlgorithm::DEFAULT_SOURCE_ID = 0;

//long[] DEFAULT_LOCAL_WORKSIZE = new long[] {1};

std::string LinearOptimumOpenClAlgorithm::KERNEL_CROSSTRACK_HAVERSINE =
		"haversine2dPointLineDistance";
std::string LinearOptimumOpenClAlgorithm::KERNEL_MAXIMUM =
		"maximumWithPositionAndOffsetFloat";
std::string LinearOptimumOpenClAlgorithm::DIJKSTRA_INITIALIZE =
		"dijkstra_initialize";
std::string LinearOptimumOpenClAlgorithm::DIJKSTRA_SSSP1 = "dijkstra_sssp1";
std::string LinearOptimumOpenClAlgorithm::DIJKSTRA_SSSP2 = "dijkstra_sssp2";

LinearOptimumOpenClAlgorithm::LinearOptimumOpenClAlgorithm() {
	std::cout << "Const LinearOptimumOpenClAlgorithm" << std::endl;
	sssp1Kernel = NULL;
	sssp2Kernel = NULL;
	_dh = new DeviceHandler(CL_DEVICE_TYPE_CPU);
	DEFAULT_LOCAL_WORKSIZE = 1;
	std::cout << "Const LinearOptimumOpenClAlgorithm Ende" << std::endl;
}

LinearOptimumOpenClAlgorithm::~LinearOptimumOpenClAlgorithm() {
	std::cout << "Desst LinearOptimumOpenClAlgorithm" << std::endl;
	delete _dh;
}

std::vector<Point*> LinearOptimumOpenClAlgorithm::run(std::vector<Point*> trace,
		double epsilon) {
	this->trace = trace;
	this->epsilon = epsilon;

	// init arrays
	int length = trace.size();
	for (int i = 0; i < length; i++) {
		//Point* point = trace.at(i);
		longitudeXCoordinates.push_back(trace.at(i)->get(0));
		latitudeYCoordinates.push_back(trace.at(i)->get(1));
	}
	// get results
	//return trace;
	return run();
}

std::vector<Point*> LinearOptimumOpenClAlgorithm::run() {
	if (longitudeXCoordinates.size() != latitudeYCoordinates.size()) {
		throw std::invalid_argument("Source arrays must be same length");
	}
	cl_int ret;
	int length = longitudeXCoordinates.size();

	// OpenCl Preamble
//	cl_platform_id platformId = Platforms.getPlatforms().get(0);

//	cl_device_id deviceId =
//			Devices.getDevices(platformId, CL_DEVICE_TYPE_GPU).get(0);
//	cl_context context = Contexts.create(platformId, deviceId);
//
//	queue = CommandQueues.create(context, deviceId);
//	cl_program program = Programs.createFromSource(context, SOURCE);

	std::cout << " 1 " << std::endl;

	queue = clCreateCommandQueue(_dh->getContext(), _dh->getDeviceId(), NULL,
			&ret);
	// Create Kernels
	/* Create data parallel OpenCL kernel */
	distanceKernel = clCreateKernel(_dh->getProgram(),
			LinearOptimumOpenClAlgorithm::KERNEL_CROSSTRACK_HAVERSINE.c_str(),
			&ret);
	maximumKernel = clCreateKernel(_dh->getProgram(),
			LinearOptimumOpenClAlgorithm::KERNEL_MAXIMUM.c_str(), &ret);
	dijkstraInitializationKernel = clCreateKernel(_dh->getProgram(),
			LinearOptimumOpenClAlgorithm::DIJKSTRA_INITIALIZE.c_str(), &ret);
	sssp1Kernel = clCreateKernel(_dh->getProgram(),
			LinearOptimumOpenClAlgorithm::DIJKSTRA_SSSP1.c_str(), &ret);
	sssp2Kernel = clCreateKernel(_dh->getProgram(),
			LinearOptimumOpenClAlgorithm::DIJKSTRA_SSSP2.c_str(), &ret);

	std::cout << " 2 " << std::endl;
//	Pointer longitudeXCoordinatesPointer = Pointer.to(longitudeXCoordinates);
//	Pointer latitudeYCoordinatesPointer = Pointer.to(latitudeYCoordinates);

	float* longitudeXCoordinatesPointer = &longitudeXCoordinates[0];
	float* latitudeYCoordinatesPointer = &latitudeYCoordinates[0];

	 memObject = new cl_mem[3];
	/* Create Buffer Object */
	// x coordinates
	memObject[0] = clCreateBuffer(_dh->getContext(),
			CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
			length * sizeof(cl_float), NULL, &ret);
	// y coordinates
	memObject[1] = clCreateBuffer(_dh->getContext(),
			CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
			length * sizeof(cl_float), NULL, &ret);
	// distance
	std::cout << memObject[2] << std::endl;

	memObject[2] = clCreateBuffer(_dh->getContext(), CL_MEM_READ_WRITE,
			length * sizeof(cl_float), NULL, &ret);

	std::cout << memObject[2] << std::endl;

	/* Copy input data to the memory buffer */
	ret = clEnqueueWriteBuffer(queue, memObject[0], CL_TRUE, 0,
			length * sizeof(cl_float), longitudeXCoordinatesPointer, 0, NULL,
			NULL);
	ret = clEnqueueWriteBuffer(queue, memObject[1], CL_TRUE, 0,
			length * sizeof(cl_float), latitudeYCoordinatesPointer, 0, NULL,
			NULL);

	std::cout << " 3 " << std::endl;

	// Set default arguments for the kernels
	// distanceKernel
	ret = clSetKernelArg(distanceKernel, 0, sizeof(cl_mem),
			(void *) &memObject[0]);
	ret = clSetKernelArg(distanceKernel, 1, sizeof(cl_mem),
			(void *) &memObject[1]);
	ret = clSetKernelArg(distanceKernel, 2, sizeof(cl_mem),
			(void *) &memObject[2]);

	// maximumKernel
	ret = clSetKernelArg(maximumKernel, 0, sizeof(cl_mem),
			(void *) &memObject[2]);

	std::cout << " 3a " << std::endl;
	//	clSetKernelArg(maximumKernel, 0, Sizeof.cl_mem, Pointer.to(memObject[2]));

	// setup edges
	// 'lineStartIndex' and 'lineEndIndex' create virtual edge
	// calculate distance from intermediatePointIndex to this edge
	std::vector<int> edges;
	int edgeCount = 0;
	for (int lineStartIndex = 0; lineStartIndex < length - 2;
			lineStartIndex++) {
		std::cout << " 3aa " << std::endl;
		vertexArray.push_back(edgeCount);
		// add neighbor as edge
		edges.push_back(lineStartIndex + 1);
		edgeCount++;

		std::cout << " 3b " << std::endl;

		for (int lineEndIndex = lineStartIndex + 2; lineEndIndex < length;
				lineEndIndex++) {
			// for (int intermediatePointIndex = lineStartIndex + 1;
			// intermediatePointIndex < lineEndIndex;
			// intermediatePointIndex++) { //}

			std::cout << " 3c : " << lineEndIndex << std::endl;

			float fromX = longitudeXCoordinates[lineStartIndex];
			float fromY = latitudeYCoordinates[lineStartIndex];
			float toX = longitudeXCoordinates[lineEndIndex];
			float toY = latitudeYCoordinates[lineEndIndex];

			std::cout << " 3cc : " << lineEndIndex << std::endl;
			bool addEdge = isValidEdge(lineStartIndex, lineEndIndex, fromX,
					fromY, toX, toY);

			std::cout << " 4 " << std::endl;
			if (addEdge) {
				edges.push_back(lineEndIndex);
				edgeCount++;
			}
		}
	}

	// connect the prior to last edge with last
	vertexArray[length - 2] = edgeCount;
	edges.push_back(length - 1);

	//int[] edgeArray = Ints.toArray(edges);
	std::vector<int> edgeArray(edges);

	std::cout << " 5 " << std::endl;
//	LOGGER.trace("VertexArray {}", Arrays.toString(vertexArray));
//	LOGGER.trace("EdgeArray {}", Arrays.toString(edgeArray));

	// set last vertex array to 0,
	// it would be a cyclic but as the last vertex is always the target
	// there is no harm
	vertexArray[length - 1] = 0;

	// clean up after distance kernel
	clReleaseMemObject(memObject[0]);
	clReleaseMemObject(memObject[1]);
	clReleaseMemObject(memObject[2]);
	clReleaseKernel(distanceKernel);
	clReleaseKernel(maximumKernel);

	std::cout << " 6 " << std::endl;
	// *********************************
	// RUN DIJKSTRA
	// *********************************

	int vertexCount = vertexArray.size();

//	Pointer vertexArrayPointer = Pointer.to(vertexArray);
//	Pointer edgeArrayPointer = Pointer.to(edgeArray);

	int* vertexArrayPointer = &vertexArray[0];
	int* edgeArrayPointer = &edgeArray[0];

	std::cout << " 7 " << std::endl;
	memObject = new cl_mem[6];
	// __global uint *vertexArray

	memObject[0] = clCreateBuffer(_dh->getContext(),
			CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
			vertexArray.size() * sizeof(cl_uint), NULL, &ret);
	// __global uint *edgeArray
	memObject[1] = clCreateBuffer(_dh->getContext(),
			CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
			edgeCount * sizeof(cl_uint), NULL, &ret);
	// __global uint *maskArray
	memObject[2] = clCreateBuffer(_dh->getContext(),
			CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
			vertexCount * sizeof(cl_uint), NULL, &ret);
	// __global float *costArray,
	memObject[3] = clCreateBuffer(_dh->getContext(), CL_MEM_READ_ONLY,
			vertexCount * sizeof(cl_uint), NULL, &ret);
	// __global float *updatingCostArray,
	memObject[4] = clCreateBuffer(_dh->getContext(), CL_MEM_READ_ONLY,
			vertexCount * sizeof(cl_uint), NULL, &ret);
	// __global uint *parentVertexArray,
	memObject[5] = clCreateBuffer(_dh->getContext(), CL_MEM_READ_ONLY,
			vertexCount * sizeof(cl_uint), NULL, &ret);

	ret = clEnqueueWriteBuffer(queue, memObject[0], CL_TRUE, 0,
			length * sizeof(cl_float), vertexArrayPointer, 0, NULL, NULL);
	ret = clEnqueueWriteBuffer(queue, memObject[1], CL_TRUE, 0,
			length * sizeof(cl_float), edgeArrayPointer, 0, NULL, NULL);

	std::cout << " 7a " << std::endl;
	std::vector<Point*> simplifiedTrace = runDijkstra(vertexArray.size(),
			edgeArray.size());
	std::cout << " 7b " << std::endl;

	// Release kernel, program, and memory objects
	clReleaseMemObject(memObject[0]);
	clReleaseMemObject(memObject[1]);
	clReleaseMemObject(memObject[2]);
	clReleaseMemObject(memObject[3]);
	clReleaseMemObject(memObject[4]);
	clReleaseMemObject(memObject[5]);

	clReleaseKernel(dijkstraInitializationKernel);
	clReleaseKernel(sssp1Kernel);
	clReleaseKernel(sssp2Kernel);
	//clReleaseProgram(program);
	clReleaseCommandQueue(queue);
	//clReleaseContext(context);

	std::cout << " 8 " << std::endl;

	return simplifiedTrace;
}

bool LinearOptimumOpenClAlgorithm::isValidEdge(int leftOffset, int rightOffset,
		float fromX, float fromY, float toX, float toY) {

	// 1. compute distances
	cl_uint ret;
	int a[] = { leftOffset };
	ret = clSetKernelArg(distanceKernel, 3, sizeof(cl_uint), (void *) a);

	int b[] = { fromX };
	ret = clSetKernelArg(distanceKernel, 4, sizeof(cl_float), (void *) b);

	int c[] = { fromY };
	ret = clSetKernelArg(distanceKernel, 5, sizeof(cl_float), (void *) c);

	int d[] = { toX };
	ret = clSetKernelArg(distanceKernel, 6, sizeof(cl_float), (void *) d);

	int e[] = { toY };
	ret = clSetKernelArg(distanceKernel, 7, sizeof(cl_float), (void *) e);

	int length = rightOffset - leftOffset + 1;
	size_t global_item_size = length;

	std::cout << " leftOffset : " << leftOffset << std::endl;
	std::cout << " rightOffset : " << rightOffset << std::endl;
	std::cout << " length : " << length << std::endl;

	size_t local_item_size = DEFAULT_LOCAL_WORKSIZE;
	clEnqueueNDRangeKernel(queue, distanceKernel, 1, NULL, &global_item_size,
			&local_item_size, 0, NULL, NULL);
	clEnqueueBarrier(queue);

	// 2. get maximum distance
	int h[] = { leftOffset };
	ret = clSetKernelArg(maximumKernel, 1, sizeof(cl_uint), (void *) h);

	int i[] = { rightOffset };
	ret = clSetKernelArg(maximumKernel, 2, sizeof(cl_uint), (void *) i);

	long globalWorkSize = pow2roundup(length);

	for (int pass = 0; globalWorkSize > 1; pass++) {

		globalWorkSize = globalWorkSize>>1;

		std::cout << " pass : " << pass << " : - : " << globalWorkSize
				<< std::endl;

		global_item_size = globalWorkSize;
		int j[] = { globalWorkSize };

		std::cout << " pass2 : " << pass << " : - : " << globalWorkSize
				<< std::endl;

		ret = clSetKernelArg(maximumKernel, 3, sizeof(cl_uint), (void *) j);

		clEnqueueNDRangeKernel(queue, maximumKernel, 1, NULL, &global_item_size,
				&local_item_size, 0, NULL, NULL);

		clEnqueueBarrier(queue);
		/* Finalization */
		ret = clFlush(queue);
//		ret = clFinish(queue);
	}

	std::cout << " 4 "	<< std::endl;

	/* Transfer result to host */
	float *values = (float *) malloc( 2*sizeof(float) );
	std::cout << memObject[2] << std::endl;
	ret = clEnqueueReadBuffer(queue, memObject[2], CL_TRUE,
			0, sizeof(float) * 2, values, 0,
			NULL, NULL);

	std::cout << " 4a "	<< std::endl;

	// make sure all read operations are done before rerunning
	clEnqueueBarrier(queue);

	float maximumDistance = values[0];
	std::cout << " maximumDistance : " << maximumDistance << " : - : "
			<< globalWorkSize << std::endl;
	// connect edge?
	return (maximumDistance <= epsilon);
}

std::vector<Point*> LinearOptimumOpenClAlgorithm::runDijkstra(int vertexCount,
		int edgeCount) {

	// Initialization
	//
	// __global int *maskArray,
	cl_uint ret;
	ret = clSetKernelArg(dijkstraInitializationKernel, 0, sizeof(cl_mem),
			memObject[2]);
	ret = clSetKernelArg(dijkstraInitializationKernel, 1, sizeof(cl_mem),
			memObject[3]);
	ret = clSetKernelArg(dijkstraInitializationKernel, 2, sizeof(cl_mem),
			memObject[4]);
	ret = clSetKernelArg(dijkstraInitializationKernel, 3, sizeof(cl_mem),
			memObject[5]);
	int a[1] = { DEFAULT_SOURCE_ID };
	ret = clSetKernelArg(dijkstraInitializationKernel, 4, sizeof(cl_mem), a);

	//long globalWorkSize[1] = { vertexCount };
	size_t global_item_size = vertexCount;
	size_t local_item_size = DEFAULT_LOCAL_WORKSIZE;
	std::cout << " 9 " << std::endl;
	clEnqueueNDRangeKernel(queue, dijkstraInitializationKernel, 1, NULL,
			&global_item_size, &local_item_size, 0, NULL, NULL);

	std::vector<int> maskArray(vertexCount);
	int* maskArrayPointer = &maskArray[0];
	std::cout << " 10 " << std::endl;
	do {
		// Enqueue Kernel SSSP 1
		//
		// __global int *vertexArray,
		ret = clSetKernelArg(sssp1Kernel, 0, sizeof(cl_mem), memObject[0]);
//		clSetKernelArg(sssp1Kernel, 0, Sizeof.cl_mem, Pointer.to(memObject[0]));
		// __global float *edgeArray,
		ret = clSetKernelArg(sssp1Kernel, 1, sizeof(cl_mem), memObject[1]);
//		clSetKernelArg(sssp1Kernel, 1, Sizeof.cl_mem, Pointer.to(memObject[1]));
		// __global int *maskArray,
		ret = clSetKernelArg(sssp1Kernel, 2, sizeof(cl_mem), memObject[2]);
//		clSetKernelArg(sssp1Kernel, 2, Sizeof.cl_mem, Pointer.to(memObject[2]));
		// __global float *costArray,
		ret = clSetKernelArg(sssp1Kernel, 3, sizeof(cl_mem), memObject[3]);
//		clSetKernelArg(sssp1Kernel, 3, Sizeof.cl_mem, Pointer.to(memObject[3]));
		// __global float *updatingCostArray,
		ret = clSetKernelArg(sssp1Kernel, 4, sizeof(cl_mem), memObject[4]);
//		clSetKernelArg(sssp1Kernel, 4, Sizeof.cl_mem, Pointer.to(memObject[4]));
		// __global uint *parentVertexArray,
		ret = clSetKernelArg(sssp1Kernel, 5, sizeof(cl_mem), memObject[5]);
//		clSetKernelArg(sssp1Kernel, 5, Sizeof.cl_mem, Pointer.to(memObject[5]));
		// int vertexCount
		int a[1] = { vertexCount };
		ret = clSetKernelArg(sssp1Kernel, 6, sizeof(cl_uint), a);
//		clSetKernelArg(sssp1Kernel, 6, Sizeof.cl_uint, Pointer.to(new int[] {
//				vertexCount }));
		// int edgeCount
		int b[1] = { edgeCount };
		ret = clSetKernelArg(sssp1Kernel, 7, sizeof(cl_uint), b);
//		clSetKernelArg(sssp1Kernel, 7, Sizeof.cl_uint, Pointer.to(new int[] {
//				edgeCount }));
		//
		clEnqueueNDRangeKernel(queue, sssp1Kernel, 1, NULL, &global_item_size,
				&local_item_size, 0, NULL, NULL);

		// Enqueue Kernel SSSP 2
		// __global int *maskArray,
		ret = clSetKernelArg(sssp2Kernel, 0, sizeof(cl_mem), memObject[2]);
		//clSetKernelArg(sssp2Kernel, 0, Sizeof.cl_mem, Pointer.to(memObject[2]));
		// __global float *costArray,
		ret = clSetKernelArg(sssp2Kernel, 1, sizeof(cl_mem), memObject[3]);
		//	clSetKernelArg(sssp2Kernel, 1, Sizeof.cl_mem, Pointer.to(memObject[3]));
		// __global float *updatingCostArray
		ret = clSetKernelArg(sssp2Kernel, 2, sizeof(cl_mem), memObject[4]);
		//clSetKernelArg(sssp2Kernel, 2, Sizeof.cl_mem, Pointer.to(memObject[4]));

		clEnqueueNDRangeKernel(queue, sssp2Kernel, 1, NULL, &global_item_size,
				&local_item_size, 0, NULL, NULL);

		// read mask array
		clEnqueueReadBuffer(queue, memObject[2], CL_TRUE, 0,
				sizeof(cl_uint) * vertexCount, &maskArrayPointer[0], 0, NULL,
				NULL);
	} while (!isEmpty(maskArray));

	// read parent vertex array
	std::cout << " 11 " << std::endl;
	std::vector<int> parentVertexArray(vertexCount);

	int* parentVertexArrayPointer = &parentVertexArray[0];

	clEnqueueReadBuffer(queue, memObject[5], CL_TRUE, 0,
			sizeof(cl_uint) * vertexCount, parentVertexArrayPointer, 0, NULL,
			NULL);
	std::cout << " 12 " << std::endl;
	return getPath(parentVertexArray);
}

std::vector<Point*> LinearOptimumOpenClAlgorithm::getPath(
		std::vector<int> parentVertexArray) {
	std::vector<Point*> path;
	std::cout << " 13 parentVertexArray : " << parentVertexArray.size()
			<< " : trace. : " << trace.size() << std::endl;

	int currentParent = parentVertexArray.size() - 1;
	for (std::vector<int>::iterator it = parentVertexArray.begin();
			it != parentVertexArray.end(); ++it) {
		std::cout << "parentVertexArray : " << *it << std::endl;
	}
	// add target
	path.push_back(trace.at(currentParent));
	std::cout << " 14 " << std::endl;
	// add chain
	while ((currentParent = parentVertexArray.at(currentParent)) != 0) {
		std::cout << " 14 currentParent : " << currentParent << std::endl;
		path.push_back(trace.at(currentParent));
	}

	std::cout << " 15 " << std::endl;
	// add source
	path.push_back(trace.at(0));

	std::cout << " 16 " << std::endl;

	//Collections.reverse(path);
	std::reverse(path.begin(), path.end());
	std::cout << " 17 " << std::endl;
	return path;
}

bool LinearOptimumOpenClAlgorithm::isEmpty(std::vector<int> ints) {
	for (std::vector<int>::iterator it = ints.begin(); it != ints.end(); ++it) {
		if (*it != 0) {
			return false;
		}
	}
	return true;
}

}
} /* namespace com */
