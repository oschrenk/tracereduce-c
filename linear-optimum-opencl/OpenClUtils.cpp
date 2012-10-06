/*
 * OpenClUtils.cpp
 *
 *  Created on: 24.10.2011
 *      Author: q2web
 */

#include "OpenClUtils.hpp"
#include <sstream>
#include <fstream>
#include <iterator>

#include <cstdlib>
#include <cstdio>

namespace com {
namespace acme {
OpenClUtils::OpenClUtils() {
	// TODO Auto-generated constructor stub

}

OpenClUtils::~OpenClUtils() {
	// TODO Auto-generated destructor stub
}

void OpenClUtils::showKernelRunTime(cl_event timingEvent) {
	int err;
	cl_ulong evStartTime = (cl_ulong) 0;
	cl_ulong evEndTime = (cl_ulong) 0;

	size_t returnBytes;
	err = clGetEventProfilingInfo(timingEvent, CL_PROFILING_COMMAND_QUEUED,
			sizeof(cl_ulong), &evStartTime, &returnBytes);

	err = clGetEventProfilingInfo(timingEvent, CL_PROFILING_COMMAND_END,
			sizeof(cl_ulong), &evEndTime, &returnBytes);

	double run_time = (double) (evEndTime - evStartTime);
	//printf("\n profile data %f secs\n", run_time * 1.0e-9);
	printf("\n profile data %f msecs\n", run_time * 1.0e-9);
	//printf("\n profile data %f ns\n", run_time * 1.0e-3);
}

char * OpenClUtils::getKernelSource(std::string fileName) {
	FILE *fp;
//	const char fileName[] = "./dataParallel.cl";
	size_t source_size;
	char *source_str;

	/* Load kernel source file */

	fp = fopen(fileName.c_str(), "r");
	if (!fp) {
		fprintf(stderr, "Failed to load kernel.\n");
		exit(1);
	}
	source_str = (char *) malloc(MAX_SOURCE_SIZE);
	source_size = fread(source_str, 1, MAX_SOURCE_SIZE, fp);

	fclose(fp);
	return source_str;

//	std::ifstream file(fileName.c_str());
//	if (!file.is_open()) {
//		std::cout << "Could not open file : " << fileName << std::endl;
//	}
//
//	std::string code(std::istreambuf_iterator<char>(file),
//			(std::istreambuf_iterator<char>()));
//	file.close();

//	std::cout << " CODE : " << code << std::endl;
}
}
}
