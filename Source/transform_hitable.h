#ifndef _TRANSFORM_HIT_H_
#define _TRANSFORM_HIT_H_

#include "hitable.h"
#include "vec3.h"

class flip_normals : public hitable
{
public:
	flip_normals(hitable *p) : ptr(p) {}
	virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const
	{
		if (ptr->hit(r, t_min, t_max, rec))
		{
			rec.normal = -rec.normal;
			return true;
		}
		else
			return false;
	}
	virtual bool bounding_box(float t0, float t1, aabb& box) const 
	{
		return ptr->bounding_box(t0, t1, box);
	}
	hitable *ptr;
};

class translate : public hitable
{
public:
	translate(hitable* p, const vec3& displacement) : ptr(p), offset(displacement) {}
	virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const;
	virtual bool bounding_box(float t0, float t1, aabb& box) const;

	hitable *ptr;
	vec3 offset;
};

bool translate::hit(const ray& r, float t_min, float t_max, hit_record& rec) const
{
	ray moved_r(r.origin() - offset, r.direction(), r.time());
	if (ptr->hit(moved_r, t_min, t_max, rec))
	{
		rec.p += offset;
		return true;
	}
	else
	{
		return false;
	}
}

bool translate::bounding_box(float t0, float t1, aabb& box) const
{
	if (ptr->bounding_box(t0, t1, box))
	{
		box = aabb(box.min() + offset, box.max() + offset);
		return true;
	}
	else
	{
		return false;
	}
}

class rotate_y : public hitable
{
public:
	rotate_y(hitable* p, float angle);
	virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const;
	virtual bool bounding_box(float t0, float t1, aabb& box) const
	{
		box = bbox;
		return hasBox;
	}
	hitable* ptr;
	float sin_theta;
	float cos_theta;
	bool hasBox;
	aabb bbox;
};

rotate_y::rotate_y(hitable* p, float angle)
{
	float radians = (PI / 180.0f) * angle;
	sin_theta = sin(radians);
	cos_theta = cos(radians);
	ptr = p;

	hasBox = ptr->bounding_box(0, 1, bbox);
	vec3 min(FLT_MAX);
	vec3 max(-FLT_MAX);
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			for (int k = 0; k < 2; k++)
			{
				float x = i * bbox.max().x() + (1 - i)*bbox.min().x();
				float y = j * bbox.max().y() + (1 - j)*bbox.min().y();
				float z = k * bbox.max().z() + (1 - k)*bbox.min().z();
				float newx = cos_theta * x + sin_theta * z;
				float newz = -sin_theta * x + cos_theta * z;
				vec3 tester(newx, y, newz);
				for (int c = 0; c < 3; c++)
				{
					if (tester[c] > max[c])
						max[c] = tester[c];
					if (tester[c] < min[c])
						min[c] = tester[c];
				}
			}
		}
	}
	bbox = aabb(min, max);
}

bool rotate_y::hit(const ray& r, float t_min, float t_max, hit_record& rec) const
{
	vec3 origin = r.origin();
	vec3 direction = r.direction();
	origin.x() = cos_theta * r.origin().x() - sin_theta * r.origin().z();
	origin.z() = sin_theta * r.origin().x() + cos_theta * r.origin().z();
	direction.x() = cos_theta * r.direction().x() - sin_theta * r.direction().z();
	direction.z() = sin_theta * r.direction().x() + cos_theta * r.direction().z();
	ray rotated_t(origin, direction, r.time());
	if (ptr->hit(rotated_t, t_min, t_max, rec))
	{
		vec3 p = rec.p;
		vec3 normal = rec.normal;
		p.x() = cos_theta * rec.p.x() + sin_theta * rec.p.z();
		p.z() = -sin_theta * rec.p.x() + cos_theta * rec.p.z();
		normal.x() = cos_theta * rec.normal.x() + sin_theta * rec.normal.z();
		normal.z() = -sin_theta * rec.normal.x() + cos_theta * rec.normal.z();
		rec.p = p;
		rec.normal = normal;
		return true;
	}
	else
	{
		return false;
	}
}

#endif
