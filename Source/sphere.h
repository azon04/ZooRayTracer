#ifndef _SPHERE_H_
#define _SPHERE_H_

#include "hitable.h"

class material;

class sphere : public hitable
{
public:
	sphere() {}
	sphere(vec3 _center, float _radius, material* mat) : center(_center), radius(_radius), mat_ptr(mat) {}

	virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const override;

	vec3 center;
	float radius;
	material* mat_ptr;
};

bool sphere::hit(const ray& r, float t_min, float t_max, hit_record& rec) const
{
	vec3 oc = r.origin() - center;
	float a = dot(r.direction(), r.direction());
	float b = 2 * dot(r.direction(), oc);
	float c = dot(oc, oc) - radius * radius;
	float discriminant = b * b - 4 * a*c;
	if (discriminant > 0)
	{
		float t = (-b - sqrt(discriminant)) / (2.0f*a);
		if (t < t_max && t > t_min)
		{
			rec.t = t;
			rec.p = r.point_at_parameter(t);
			rec.normal = (rec.p - center) / radius;
			rec.mat_ptr = mat_ptr;
			return true;
		}

		t = (-b + sqrt(discriminant)) / (2.0f*a);
		if (t < t_max && t > t_min)
		{
			rec.t = t;
			rec.p = r.point_at_parameter(t);
			rec.normal = (rec.p - center) / radius;
			rec.mat_ptr = mat_ptr;
			return true;
		}
	}

	return false;
}

#endif
