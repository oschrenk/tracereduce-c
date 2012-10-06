/*
 * Arrays.cpp
 *
 *  Created on: 03.10.2012
 *      Author: q2web
 */

#include "Arrays.hpp"

namespace com {
namespace acme {

int Arrays::NOT_FOUND = -1;

Arrays::Arrays() {
	// TODO Auto-generated constructor stub

}

Arrays::~Arrays() {
	// TODO Auto-generated destructor stub
}

bool* Arrays::prefilled(int length, bool value) {
	bool* a = new bool[length];
	for (int i = 0; i < length; i++) {
		a[i] = value;
	}
	//java.util.Arrays.fill(a, value);
	return a;
}

/**
 * Prefilled.
 *
 * @param length
 *            the length
 * @param value
 *            the value
 * @return the float[]
 */
float* Arrays::prefilled(int length, float value) {
	float* a = new float[length];
	for (int i = 0; i < length; i++) {
		a[i] = value;
	}
	//java.util.Arrays.fill(a, value);
	return a;
}

}
} /* namespace com */
