#ifndef _AABB_H_
#define _AABB_H_

#include "ray.h"

inline float ffmin(float a, float b) { return a < b ? a : b; }
inline float ffmax(float a, float b) { return a > b ? a : b; }
inline void ffswap(float& a, float& b)
{
	b = a + b;
	a = b - a;
	b = b - a;
}

class aabb
{
public:
	aabb() {}
	aabb(const vec3& a, const vec3& b) : _min(a), _max(b) {}

	vec3 min() const { return _min; }
	vec3 max() const { return _max; }

	bool hit(const ray& r, float tmin, float tmax) const
	{
		for (int a = 0; a < 3; a++)
		{
			float invD = 1.0f / r.direction()[a];
			float t0 = (_min[a] - r.origin()[a]) * invD;
			float t1 = (_max[a] - r.origin()[a]) * invD;

			if (invD < 0.0f)
			{
				ffswap(t0, t1);
			}

			tmin = ffmax(t0, tmin);
			tmax = ffmin(t1, tmax);

			if (tmax <= tmin) { return false; }
		}
		return true;
	}

	vec3 _min;
	vec3 _max;
};

aabb surrounding_box(aabb box0, aabb box1)
{
	vec3 small(ffmin(box0.min().x(), box1.min().x()),
		ffmin(box0.min().y(), box1.min().y()),
		ffmin(box0.min().z(), box1.min().z()));
	vec3 big(ffmax(box0.max().x(), box1.max().x()),
		ffmax(box0.max().y(), box1.max().y()),
		ffmax(box0.max().z(), box1.max().z()));

	return aabb(small, big);
}

#endif
