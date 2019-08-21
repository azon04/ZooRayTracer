#ifndef _RAY_H_
#define _RAY_H_

#include "Vec3.h"

class Ray
{
public:
	Ray() {};
	Ray(const Vec3& _origin, const Vec3& _direction, float ti = 0.0f) { A = _origin; B = _direction; _time = ti; }

	Vec3 origin() const { return A; }
	Vec3 direction() const { return B; }
	float time() const { return _time; }

	Vec3& origin() { return A; }
	Vec3& direction() { return B; }
	float& time() { return _time; }

	Vec3 point_at_parameter(float t) const { return A + t * B; }

	Vec3 A;
	Vec3 B;
	float _time;
};

#endif
