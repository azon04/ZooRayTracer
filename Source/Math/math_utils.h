#ifndef _MATH_UTILS_H_
#define _MATH_UTILS_H_

#include "math.h"
#include "rand_helper.h"

#define PI       3.14159265358979323846f

vec3 random_cosine_direction()
{
	float r1 = rand_float();
	float r2 = rand_float();
	float z = sqrt(1 - r2);
	float phi = 2 * PI * r1;
	float x = cos(phi) * 2 * sqrt(r2);
	float y = sin(phi) * 2 * sqrt(r2);
	return vec3(x, y, z);
}

#endif // !_MATH_UTILS_H_