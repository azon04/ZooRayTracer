#ifndef _SPHERE_H_
#define _SPHERE_H_

#include "hitable.h"
#include "math.h"

class material;

#define PI       3.14159265358979323846f

void get_sphere_uv(const vec3& p, float& u, float& v)
{
	float phi = atan2(p.z(), p.x());
	float thetha = asin(p.y());
	u =  1 - ( phi + PI) / (2 * PI);
	v = (thetha + PI / 2.0f) / PI;
}

class sphere : public hitable
{
public:
	sphere() {}
	sphere(vec3 _center, float _radius, material* mat) : center(_center), radius(_radius), mat_ptr(mat) {}

	virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const override;
	virtual bool bounding_box(float t0, float t1, aabb& box) const override;

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
			get_sphere_uv(rec.normal, rec.u, rec.v);
			return true;
		}

		t = (-b + sqrt(discriminant)) / (2.0f*a);
		if (t < t_max && t > t_min)
		{
			rec.t = t;
			rec.p = r.point_at_parameter(t);
			rec.normal = (rec.p - center) / radius;
			rec.mat_ptr = mat_ptr;
			get_sphere_uv(rec.normal, rec.u, rec.v);
			return true;
		}
	}

	return false;
}

bool sphere::bounding_box(float t0, float t1, aabb& box) const
{
	box = aabb(center - vec3(radius), center + vec3(radius));
	return true;
}

class moving_sphere : public hitable
{
public:
	moving_sphere() {}
	moving_sphere(vec3 cen0, vec3 cen1, float t0, float t1, float r, material* m)
		: center0(cen0), center1(cen1), time0(t0), time1(t1), radius(r), mat_ptr(m)
	{}

	virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const;
	virtual bool bounding_box(float t0, float t1, aabb& box) const;

	vec3 center(float time) const;

	vec3 center0, center1;
	float time0, time1;
	float radius;
	material* mat_ptr;
};

bool moving_sphere::hit(const ray& r, float t_min, float t_max, hit_record& rec) const
{
	vec3 centerAtTime = center(r.time());
	vec3 oc = r.origin() - centerAtTime;
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
			rec.normal = (rec.p - centerAtTime) / radius;
			rec.mat_ptr = mat_ptr;
			get_sphere_uv(rec.normal, rec.u, rec.v);
			return true;
		}

		t = (-b + sqrt(discriminant)) / (2.0f*a);
		if (t < t_max && t > t_min)
		{
			rec.t = t;
			rec.p = r.point_at_parameter(t);
			rec.normal = (rec.p - centerAtTime) / radius;
			rec.mat_ptr = mat_ptr;
			get_sphere_uv(rec.normal, rec.u, rec.v);
			return true;
		}
	}

	return false;
}

bool moving_sphere::bounding_box(float t0, float t1, aabb& box) const
{
	aabb box0(center(t0) - vec3(radius), center(t0) + vec3(radius));
	aabb box1(center(t1) - vec3(radius), center(t1) + vec3(radius));
	box = surrounding_box(box0, box1);
	return true;
}

vec3 moving_sphere::center(float time) const
{
	return center0 + ((time - time0) / (time1 - time0)) * (center1 - center0);
}

#endif
