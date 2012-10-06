/*
 *******************************************************************************
 *  SplineInterpolationWithPointOmission.cpp
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
#include <stdexcept>
#include <cmath>
#include "SplineInterpolationWithPointOmission.hpp"

namespace com {
namespace acme {
SplineInterpolationWithPointOmission::SplineInterpolationWithPointOmission() {
	// TODO Auto-generated constructor stub

}

SplineInterpolationWithPointOmission::~SplineInterpolationWithPointOmission() {
	// TODO Auto-generated destructor stub
}

void SplineInterpolationWithPointOmission::compute(float* x, float* y,
		float* distances, bool* excludes, int length, int idToExclude) {
	if (idToExclude <= 0) {
		throw std::invalid_argument("idToExclude must be > 0");
	}
	if (idToExclude >= length) {
		throw std::invalid_argument("idToExclude must be < length");
	}
	// @formatter:off

	// starts at 0
	// int id = get_global_id(0);

	// ******************************************
	// Exclusion
	// ******************************************
	// Looking at segment i, with control points x_i and x_{i-1},
	// the interpolation algorithm needs access to control points
	// x_{i-2}, x_{i-1}, x_{i}, x_{i+1} for i > 1, for i =1 it needs
	// access to x_{i-1}, x_{i}, x_{i+1}
	// so the outer left control point x_{left} always needs to be defined.
	// That means
	//
	// i) id = starting x_{left} (that means starting x_{id-1} or x_{id-2})
	// If x_{left} is to be excluded, the interpolation doesn't need be
	// calculated. To mark them the element i as excluded the distance is
	// set to Float.MAX, so that it gets ignored when calculating the
	// minimum interpolation error.
	// id = 0 can never be excluded

	// if (idToExclude == 2 && excludes[1]) {
	// distances[1] = Float.MAX_VALUE;
	// distances[2] = Float.MAX_VALUE;
	// return;
	// }
	//
	// if (idToExclude > 2 && excludes[1]
	// && (excludes[idToExclude - 1] || excludes[idToExclude - 2])) {
	// distances[idToExclude] = Float.MAX_VALUE;
	// distances[idToExclude - 1] = Float.MAX_VALUE;
	// distances[idToExclude - 2] = Float.MAX_VALUE;
	// return;
	// }

	// ii) excludeId == x_{id}
	// a) excludeId == x_{id} && excludes[id+1] == false
	// If x_{left} is NOT to be excluded, we need to look at x_{id}. We
	// iterate through the elements for j > i, until we find an element,
	// that isn't
	// excluded. Set j.
	// Interpolate with x_{id-2}, x_{id-1}, x_j, x_{j+1}
	// b) excludeId == x_{id} && excludes[id+1] == true
	// Find j according to ii,a. Iterate over the elements for k > j
	// until we find an element that isn't excluded. This is x_{k}.
	// (as always excludes[length] = false, this terminates
	// deterministically)
	// Interpolate with x_{id-2}, x_{id-1}, x_j, x_{k}

	// iii) excludes[n] = true for any n > excludeId + 2 < length - 1
	// That means that we don't find an exclusion during the spinup of the
	// kernel but find an
	// exclusion later. This of course means that case iii) is only to be
	// found when ii) happens
	// When during iteration over the element array in step i, excludes[i-1]
	// = true;
	// first set h = i-1, g = i-2
	//
	// a) exclude[g] = true && exclude[g-1] = false && exclude [g-2] = false
	// Symmetrical to case ii) we iterate backwards through the
	// array, with g = g -1, h = h-1 until you find an x[g] where
	// excludes[g] = false
	// Interpolate with x_{g-1}, x_{g}, x_i, x_{j}
	// a) exclude[g] = true && exclude[g-1] = true && exclude[n] = true for
	// any n < g-1
	// Find g according to iii,a. Symetrical to to ii, iterate backwards
	// through the
	// array, with fixed g and h = g -1 until you find and x[h] where
	// excludes[h] = false
	// (as always excludes[0] = false, this terminates deterministically)
	// Interpolate with x_{h}, x_{g}, x_i, x_{j}

	// ******************************************
	// Piecewise Cubic Spline Interpolation
	// ******************************************
	// f(x) = a + bx +cx^2 +dx^3
	// f'(x) = b + 2cx +3dx^2
	// f''(x) = 2c +6x

	// §1 Each curve segment passes through its control points, thus
	// f_i(x_i) = y_i, f_{i+1}(x_{i+1}) = y_{i+1}
	// I a_i + b_ix_i +c_ix^2_i +d_ix^3_i = y_i
	// II a_i + b_ix_{i+1} +C_1ix^2_{i+1} +d_ix^3_{i+1} = y_i

	// §2 Each curve segment has the same slope where they join, thus
	// f'_i(x_{i+1}) = f'_{i+1}}(x_{i+1})
	// enforcing C_1 continuity
	// III b_i + 2c_ix_{i+1} + 3d_ix^2_{i+1} - b_{i+1} - 2c_{i+}}x_{i+1} +
	// 3d_{i+1}}x^2_{i+1} = 0

	// §3 Each curve segment has the same curvature where they join, thus
	// f''_i(x_{i+1}) = f''_{i+1}}(x_{i+1})
	// enforcing C_2 continuity
	// IV 2c_i + 6d_ix_{i+1} - 2c_{i+1} - 6d_{i+1}x_{i+1} = 0

	// $4 At the left end of the curve we are missing the C_1 and C_2
	// equations

	// =>

	float x0 = 0, y0 = 0, xa, ya, xb, yb, xc = 0, yc = 0;
	float fPrimeLeft;
	float fPrimeRight;
	float fDoublePrimeLeft;
	float fDoublePrimeRight;
	// TODO remove d[0], 4 float per entry adds up

	float* d = new float[length]; // d[0] is left empty for better
	// readibility

	float* c = new float[length]; // c[0] is left empty for better
	// readibility

	float* b = new float[length]; // b[0] is left empty for better
	// readibility

	float* a = new float[length]; // a[0] is left empty for better
	// readibility

	// TODO remove excludes[0. excludes[n], these are only two floats
	// globally but still...
	int i = 1, j = 1, k = 2, g, h; // control point (start with 0)
	for (i = 1; j < (length - 1); i++) {
		g = i - 2;
		h = i - 1;
		// case iii)
		while (excludes[h] && h >= 1) {
			h = h - 1;
			g = g - 1;
		}
		// / case iii, b)
		while (h > 0 && excludes[g] && g > 1) {
			g = g - 1;
		}

		j = i;
		k = j + 1;
		// case ii) excludeId == xRight
		while (excludes[j] && k < (length - 1)) {
			j = j + 1;
			k = j + 1;
		}
		// because of case iii, we might have reached left border,
		// if check == false, left border is not reached , so we can set x0
		if (j > 1 && g > 0) {
			x0 = x[g];
			y0 = y[g];
		}
		xa = x[h];
		ya = y[h];
		xb = x[j];
		yb = y[j];
		// case ii,b)
		while (j < length - 1 && excludes[k] && k < length - 1) {
			k = k + 1;
		}
		// if evaluates to true than right border not reached => we can
		// access x[k], y[k]
		if (j < (length - 1)) {
			xc = x[k];
			yc = y[k];
		}
		fPrimeRight = 2 / ((xc - xb) / (yc - yb) + (xb - xa) / (yb - ya));

		// if check == true, left border is reached => change formula of
		// prime left
		fPrimeLeft =
				(j > 1 && h == 0) ?
						3 / 2 * (yb - ya) / (xb - xa) - fPrimeRight / 2 :
						2 / ((xb - xa) / (yb - ya) + (xa - x0) / (ya - y0));

		// formula of prime right changes if right border is reached
		if (j == length) {
			fPrimeRight = 3 / 2 * ((yb - ya) / (xb - xa)) - fPrimeLeft / 2;
		}

		fDoublePrimeLeft = -(2 * (fPrimeRight + 2 * fPrimeLeft)) / (xb - xa)
				+ (6 * (yb - ya) / (xb - xa)) / (xb - xa);
		fDoublePrimeRight = 2 * (2 * fPrimeRight + fPrimeLeft) / (xb - xa)
				- 6 * (yb - ya) / (xb - xa) / (xb - xa);

		// TODO check using i as index is appropiate
		d[i] = 1 / 6 * (fDoublePrimeRight - fDoublePrimeLeft) / (xb - xa);
		c[i] = 1 / 2 * (xb * fDoublePrimeLeft - xa * fDoublePrimeRight)
				/ (xb - xa);
		b[i] = ((yb - ya) - c[i] * (xb * xb - xa * xa)
				- d[i] * (xb * xb * xb - xa * xa * xa)) / (xb - xa);
		a[i] = ya - b[i] * xa - c[i] * xa * xa - d[i] * xa * xa * xa;

		// we don't need to iterate over excludes, skip over them
		// but subtract 1 because the loop will add one
		// j stops one segment before right edge, as right edge is always
		// included
		// we need to iterate one more time
		if (i != j) {
			i = j - 1;
			k = i + 1;
		}

		// @formatter:on
	}

	// compute the interpolation error
	// f(xx) = yy
	// yy = a[i] + b[i] * xx + c[i] + xx*xx + d[i] + xx*xx*xx;

	float yy = a[idToExclude] + b[idToExclude] * x[idToExclude]
			+ c[idToExclude] * x[idToExclude] * x[idToExclude]
			+ d[idToExclude] * x[idToExclude] * x[idToExclude] * x[idToExclude];
	float tmpValue = y[idToExclude] - yy;
	distances[idToExclude - 1] = std::abs(tmpValue);

}
}
} /* namespace com */
