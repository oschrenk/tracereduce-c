/*
 * Arrays.hpp
 *
 *  Created on: 03.10.2012
 *      Author: q2web
 */

#ifndef ARRAYS_HPP_
#define ARRAYS_HPP_
#include <iostream>

namespace com {
namespace acme {
class Arrays {
public:
	Arrays();
	virtual ~Arrays();

	static bool* prefilled(int length, bool value);
	static float* prefilled(int length, float value);
private:
	static int NOT_FOUND;
};
}
} /* namespace com */
#endif /* ARRAYS_HPP_ */
