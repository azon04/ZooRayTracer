#ifndef _BVH_H_
#define _BVH_H_

#include "Hitable.h"
#include "rand_helper.h"

#include <iostream>

class BVHNode : public Hitable
{
public:
	BVHNode() {}
	BVHNode(Hitable **l, int n, float time0, float time1);

	void construct();
	virtual bool hit(const Ray& r, float t_min, float t_max, HitRecord& rec) const override;
	virtual bool bounding_box(float t0, float t1, AABB& box) const override;
	
	virtual void writeToJSON(rapidjson::Value* jsonValue, rapidjson::Document* document);
	virtual void readJSON(rapidjson::Value* jsonValue);

	Hitable *left;
	Hitable *right;
	AABB box;

	Hitable** list;
	int listSize;
	float time0;
	float time1;
};
#endif
