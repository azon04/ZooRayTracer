#ifndef _HITABLE_H_
#define _HITABLE_H_

#include "Ray.h"
#include "AABB.h"
#include "JSONParseable.h"

class Material;

struct HitRecord
{
	float t;
	Vec3 p;
	Vec3 normal;
	float u, v;
	Material* mat_ptr;
};

class Hitable : public JSONParseable
{
public:
	virtual bool hit(const Ray& r, float  t_min, float t_max, HitRecord& rec) const = 0;
	virtual bool bounding_box(float t0, float t1, AABB& box) const = 0;
	virtual float pdf_value(const Vec3& o, const Vec3& v) const { return 0.0f; }
	virtual Vec3 random(const Vec3& o) const { return Vec3(1.0f, 0.0f, 0.0f); }

	virtual void writeToJSON(rapidjson::Value* jsonValue, rapidjson::Document* document)
	{
		jsonValue->SetObject();
		rapidjson::Value::Object jsonObject = jsonValue->GetObject();
		jsonObject.AddMember("Type", "Hitable", document->GetAllocator());
	}
};

#endif
