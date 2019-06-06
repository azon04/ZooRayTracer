#ifndef _RAY_H_
#define _RAY_H_

#include "vec3.h"

class ray
{
public:
	ray() {};
	ray(const vec3& _origin, const vec3& _direction, float ti = 0.0f) { A = _origin; B = _direction; _time = ti; }

	vec3 origin() const { return A; }
	vec3 direction() const { return B; }
	float time() const { return _time; }

	vec3& origin() { return A; }
	vec3& direction() { return B; }
	float& time() { return _time; }

	vec3 point_at_parameter(float t) const { return A + t * B; }

	vec3 A;
	vec3 B;
	float _time;
};

#endif
