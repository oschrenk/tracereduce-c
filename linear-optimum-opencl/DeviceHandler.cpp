/*
 * DeviceHandler.cpp
 *
 *  Created on: 24.10.2011
 *      Author: q2web
 */

#include "DeviceHandler.hpp"
#include "OpenClUtils.hpp"
#include <iostream>
#include <cstdlib>

namespace com {
namespace acme {

DeviceHandler::DeviceHandler(cl_device_type deviceType) {
	int err;
	// compute device id
	// Connect to a compute device
	// Deklaration Plattform
	clGetPlatformIDs(1, &platformId, NULL); //int gpu = 1;
	err = clGetDeviceIDs(platformId, deviceType, 1, &deviceId, NULL);
	if (err != CL_SUCCESS) {
		std::cout << "Error: Failed to create a device group!\n";
		exit(1);
	}
	// Create a compute context
	//
	context = clCreateContext(0, 1, &deviceId, NULL, NULL, &err);
	if (!context) {
		std::cout << "Error: Failed to create a compute context!\n";
		exit(1);
	}
	const char *code = OpenClUtils::getKernelSource("linearOptimum.cl");
	program = clCreateProgramWithSource(context, 1, (const char **) &code, NULL,
			&err);
	if (!program) {
		std::cout << "Error: Failed to create compute program!\n";
		exit(1);
	}
	// Build the program executable
	//
	err = clBuildProgram(program, 0, NULL, NULL, NULL, NULL);
	if (err != CL_SUCCESS) {
		size_t len;
		char buffer[4096];
		std::cout << "Error: Failed to build program executable!\n";
		clGetProgramBuildInfo(program, deviceId, CL_PROGRAM_BUILD_LOG,
				sizeof(buffer), buffer, &len);
		std::cout << buffer;
		exit(1);
	}
}

DeviceHandler::~DeviceHandler() {
	clReleaseProgram(program);
	clReleaseContext(context);
}

cl_context DeviceHandler::getContext() const {
	return context;
}

cl_device_id DeviceHandler::getDeviceId() const {
	return deviceId;
}

cl_program DeviceHandler::getProgram() const {
	return program;
}

void DeviceHandler::setContext(cl_context context) {
	this->context = context;
}

void DeviceHandler::setDeviceId(cl_device_id deviceId) {
	this->deviceId = deviceId;
}

void DeviceHandler::setProgram(cl_program program) {
	this->program = program;
}

cl_platform_id DeviceHandler::getPlatformId() const {
	return platformId;
}

void DeviceHandler::setPlatformId(cl_platform_id platformId) {
	this->platformId = platformId;
}
}
}

