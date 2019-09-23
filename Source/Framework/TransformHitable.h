#ifndef _TRANSFORM_HIT_H_
#define _TRANSFORM_HIT_H_

#include "Hitable.h"
#include "Vec3.h"

class FlipNormals : public Hitable
{
public:
	FlipNormals() {}
	FlipNormals(Hitable *p) : ptr(p) {}
	virtual bool hit(const Ray& r, float t_min, float t_max, HitRecord& rec) const
	{
		if (ptr->hit(r, t_min, t_max, rec))
		{
			rec.normal = -rec.normal;
			return true;
		}
		else
			return false;
	}
	virtual bool bounding_box(float t0, float t1, AABB& box) const 
	{
		return ptr->bounding_box(t0, t1, box);
	}

	virtual void writeToJSON(rapidjson::Value* jsonValue, rapidjson::Document* document);
	virtual void readJSON(rapidjson::Value* jsonValue);

	Hitable *ptr;
};

class TranslateHitable : public Hitable
{
public:
	TranslateHitable() {}
	TranslateHitable(Hitable* p, const Vec3& displacement) : ptr(p), offset(displacement) {}
	virtual bool hit(const Ray& r, float t_min, float t_max, HitRecord& rec) const;
	virtual bool bounding_box(float t0, float t1, AABB& box) const;

	virtual void writeToJSON(rapidjson::Value* jsonValue, rapidjson::Document* document);
	virtual void readJSON(rapidjson::Value* jsonValue);

	Hitable *ptr;
	Vec3 offset;
};

class RotateY : public Hitable
{
public:
	RotateY() {}
	RotateY(Hitable* p, float angle);
	virtual bool hit(const Ray& r, float t_min, float t_max, HitRecord& rec) const;
	virtual bool bounding_box(float t0, float t1, AABB& box) const
	{
		box = bbox;
		return hasBox;
	}

	virtual void writeToJSON(rapidjson::Value* jsonValue, rapidjson::Document* document);
	virtual void readJSON(rapidjson::Value* jsonValue);

	Hitable* ptr;
	float sin_theta;
	float cos_theta;
	bool hasBox;
	AABB bbox;

	// Cached angle value
	float angle;
};

#endif
