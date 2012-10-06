/*
 * OpenClUtils.hpp
 *
 *  Created on: 24.10.2011
 *      Author: q2web
 */

#ifndef OPENCLUTILS_HPP_
#define OPENCLUTILS_HPP_

//#define DATA_SIZE (4)
#define MAX_SOURCE_SIZE (10000)

#include <CL/cl.h>
#include <iostream>
namespace com {
namespace acme {
class OpenClUtils {
public:
	OpenClUtils();
	virtual ~OpenClUtils();

	static char * getKernelSource(std::string fileName);
	static void showKernelRunTime(cl_event timingEvent);

};
}
}
#endif /* OPENCLUTILS_HPP_ */
