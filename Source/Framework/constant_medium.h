#ifndef _CONSTANT_MEDIUM_H_
#define _CONSTANT_MEDIUM_H_

#include "Hitable.h"
#include "Material.h"

class isotropic : public Material
{
public:
	isotropic(Texture* a) : albedo(a) {}
	virtual bool scatter(const Ray& r_in, const HitRecord& rec, Vec3& attenuation, Ray& scattered) const
	{
		scattered = Ray(rec.p, random_in_unit_sphere());
		attenuation = albedo->value(rec.u, rec.v, rec.p);
		return true;
	}
	Texture* albedo;
};

class constant_medium : public Hitable
{
public:
	constant_medium(Hitable* b, float d, Texture* a) : boundary(b), density(d)
	{
		phase_function = new isotropic(a);
	}
	virtual bool hit(const Ray& r, float t_min, float t_max, HitRecord& rec) const;
	virtual bool bounding_box(float t0, float t1, AABB& box) const
	{
		return boundary->bounding_box(t0, t1, box);
	}
	Hitable* boundary;
	float density;
	Material *phase_function;
};

bool constant_medium::hit(const Ray& r, float t_min, float t_max, HitRecord& rec) const
{
	HitRecord rec1, rec2;
	if (boundary->hit(r, -FLT_MAX, FLT_MAX, rec1))
	{
		if (boundary->hit(r, rec1.t + 0.0001f, FLT_MAX, rec2))
		{
			if (rec1.t < t_min)
				rec1.t = t_min;
			if (rec2.t > t_max)
				rec2.t = t_max;
			if (rec1.t >= rec2.t)
				return false;
			if (rec1.t < 0)
				rec1.t = 0;
			float distance_inside_boundary = (rec2.t - rec1.t) * r.direction().length();
			float hit_distance = -(1 / density) * log(rand_float());
			if (hit_distance < distance_inside_boundary)
			{
				rec.t = rec1.t + hit_distance / r.direction().length();
				rec.p = r.point_at_parameter(rec.t);
				rec.normal = Vec3(1.0f, 0.0f, 0.0f); // arbitrary
				rec.mat_ptr = phase_function;
				return true;
			}
		}
	}
	return false;
}

#endif
