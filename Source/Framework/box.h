#ifndef _BOX_H_
#define _BOX_H_

#include "PlaneHitable.h"
#include "TransformHitable.h"
#include "HitableList.h"

class Box : public Hitable
{
public:
	Box() : list_ptr(nullptr) {}
	Box(const Vec3& p0, const Vec3& p1, Material *ptr);
	~Box();

	void setup();
	virtual bool hit(const Ray& r, float t_min, float t_max, HitRecord& rec) const;
	virtual bool bounding_box(float t0, float t1, AABB& _box) const
	{
		_box = AABB(pmin, pmax);
		return true;
	}
	
	virtual void writeToJSON(rapidjson::Value* jsonValue, rapidjson::Document* document);
	virtual void readJSON(rapidjson::Value* jsonValue);

	Vec3 pmin, pmax;
	Hitable *list_ptr;
	Material* mat;
};
#endif
