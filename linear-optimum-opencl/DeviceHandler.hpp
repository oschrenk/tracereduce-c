/*
 * DeviceHandler.hpp
 *
 *  Created on: 24.10.2011
 *      Author: q2web
 */

#ifndef DEVICEHANDLER_HPP_
#define DEVICEHANDLER_HPP_

#if defined(__APPLE__) || defined(__MACOSX)
#include <OpenCL/cl.h>
#else
#include <CL/cl.h>
#endif

namespace com {
namespace acme {

class DeviceHandler {
public:
	DeviceHandler(cl_device_type deviceType);
	virtual ~DeviceHandler();
	cl_context getContext() const;
	cl_device_id getDeviceId() const;
	cl_program getProgram() const;
	void setContext(cl_context context);
	void setDeviceId(cl_device_id deviceId);
	void setProgram(cl_program program);
    cl_platform_id getPlatformId() const;
    void setPlatformId(cl_platform_id platformId);

private:
	cl_device_id deviceId; // compute device id
	cl_context context; // compute context
	cl_program program; // compute program
	cl_platform_id platformId; //opencl platform:id
};
}
}
#endif /* DEVICEHANDLER_HPP_ */
