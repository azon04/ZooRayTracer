#ifndef _HITABLE_H_
#define _HITABLE_H_

#include "ray.h"
#include "aabb.h"

class material;

struct hit_record
{
	float t;
	vec3 p;
	vec3 normal;
	float u, v;
	material* mat_ptr;
};

class hitable
{
public:
	virtual bool hit(const ray& r, float  t_min, float t_max, hit_record& rec) const = 0;
	virtual bool bounding_box(float t0, float t1, aabb& box) const = 0;
	virtual float pdf_value(const vec3& o, const vec3& v) const { return 0.0f; }
	virtual vec3 random(const vec3& o) const { return vec3(1.0f, 0.0f, 0.0f); }
};

#endif
