#ifndef _RAY_H_
#define _RAY_H_

#include "vec3.h"

class ray
{
public:
	ray() {};
	ray(const vec3& _origin, const vec3& _direction) { A = _origin; B = _direction; }

	vec3 origin() const { return A; }
	vec3 direction() const { return B; }

	vec3& origin() { return A; }
	vec3& direction() { return B; }

	vec3 point_at_parameter(float t) const { return A + t * B; }

	vec3 A;
	vec3 B;
};

#endif