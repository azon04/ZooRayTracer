#ifndef _AABB_H_
#define _AABB_H_

#include "Ray.h"

inline float ffmin(float a, float b) { return a < b ? a : b; }
inline float ffmax(float a, float b) { return a > b ? a : b; }
inline void ffswap(float& a, float& b)
{
	b = a + b;
	a = b - a;
	b = b - a;
}

class AABB
{
public:
	AABB() {}
	AABB(const Vec3& a, const Vec3& b) : _min(a), _max(b) {}

	Vec3 min() const { return _min; }
	Vec3 max() const { return _max; }

	bool hit(const Ray& r, float tmin, float tmax) const
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

	Vec3 _min;
	Vec3 _max;
};

AABB surrounding_box(AABB box0, AABB box1);

#endif
