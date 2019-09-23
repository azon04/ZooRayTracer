#ifndef _HITABLELIST_H_
#define  _HITABLELIST_H_

#include "Hitable.h"
#include "rand_helper.h"

class HitableList : public Hitable
{
public:
	HitableList() {}
	HitableList(Hitable **l, int n) { list = l; list_size = n; }

	virtual bool hit(const Ray& r, float t_min, float t_max, HitRecord& rec) const override;
	virtual bool bounding_box(float t0, float t1, AABB& box) const override;
	virtual float pdf_value(const Vec3& o, const Vec3& v) const;
	virtual Vec3 random(const Vec3& o) const;

	virtual void writeToJSON(rapidjson::Value* jsonValue, rapidjson::Document* document);
	virtual void readJSON(rapidjson::Value* jsonValue);

	Hitable** list;
	int list_size;
};

#endif
