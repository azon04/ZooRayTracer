#ifndef _BOX_H_
#define _BOX_H_

#include "PlaneHitable.h"
#include "TransformHitable.h"
#include "HitableList.h"

class Box : public Hitable
{
public:
	Box() {}
	Box(const Vec3& p0, const Vec3& p1, Material *ptr);
	virtual bool hit(const Ray& r, float t_min, float t_max, HitRecord& rec) const;
	virtual bool bounding_box(float t0, float t1, AABB& _box) const
	{
		_box = AABB(pmin, pmax);
		return true;
	}
	Vec3 pmin, pmax;
	Hitable *list_ptr;
};

Box::Box(const Vec3& p0, const Vec3& p1, Material *ptr)
{
	pmin = p0;
	pmax = p1;
	Hitable **list = new Hitable *[6];
	list[0] = new XYRect(pmin.x(), pmax.x(), pmin.y(), pmax.y(), pmax.z(), ptr);
	list[1] = new FlipNormals(new XYRect(pmin.x(), pmax.x(), pmin.y(), pmax.y(), pmin.z(), ptr));
	list[2] = new XZRect(pmin.x(), pmax.x(), pmin.z(), pmax.z(), pmax.y(), ptr);
	list[3] = new FlipNormals(new XZRect(pmin.x(), pmax.x(), pmin.z(), pmax.z(), pmin.y(), ptr));
	list[4] = new YZRect(pmin.y(), pmax.y(), pmin.z(), pmax.z(), pmax.x(), ptr);
	list[5] = new FlipNormals(new YZRect(pmin.y(), pmax.y(), pmin.z(), pmax.z(), pmin.x(), ptr));

	list_ptr = new HitableList(list, 6);
}

bool Box::hit(const Ray& r, float t_min, float t_max, HitRecord& rec) const
{
	return list_ptr->hit(r, t_min, t_max, rec);
}

#endif
