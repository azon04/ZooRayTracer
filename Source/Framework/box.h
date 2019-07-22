#ifndef _BOX_H_
#define _BOX_H_

#include "plane.h"
#include "transform_hitable.h"
#include "hitablelist.h"

class box : public hitable
{
public:
	box() {}
	box(const vec3& p0, const vec3& p1, material *ptr);
	virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const;
	virtual bool bounding_box(float t0, float t1, aabb& _box) const
	{
		_box = aabb(pmin, pmax);
		return true;
	}
	vec3 pmin, pmax;
	hitable *list_ptr;
};

box::box(const vec3& p0, const vec3& p1, material *ptr)
{
	pmin = p0;
	pmax = p1;
	hitable **list = new hitable *[6];
	list[0] = new xy_rect(pmin.x(), pmax.x(), pmin.y(), pmax.y(), pmax.z(), ptr);
	list[1] = new flip_normals(new xy_rect(pmin.x(), pmax.x(), pmin.y(), pmax.y(), pmin.z(), ptr));
	list[2] = new xz_rect(pmin.x(), pmax.x(), pmin.z(), pmax.z(), pmax.y(), ptr);
	list[3] = new flip_normals(new xz_rect(pmin.x(), pmax.x(), pmin.z(), pmax.z(), pmin.y(), ptr));
	list[4] = new yz_rect(pmin.y(), pmax.y(), pmin.z(), pmax.z(), pmax.x(), ptr);
	list[5] = new flip_normals(new yz_rect(pmin.y(), pmax.y(), pmin.z(), pmax.z(), pmin.x(), ptr));

	list_ptr = new hitable_list(list, 6);
}

bool box::hit(const ray& r, float t_min, float t_max, hit_record& rec) const
{
	return list_ptr->hit(r, t_min, t_max, rec);
}

#endif
