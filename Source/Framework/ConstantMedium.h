#ifndef _CONSTANT_MEDIUM_H_
#define _CONSTANT_MEDIUM_H_

#include "Hitable.h"
#include "Material.h"
#include "PDF.h"

class Isotropic : public Material
{
public:
	Isotropic() : albedo(nullptr) 
	{
	}
	Isotropic(Texture* a) : albedo(a) 
	{
	}
	virtual bool scatter(const Ray& r_in, const HitRecord& rec, ScatterRecord& srec) const;

	virtual void writeToJSON(rapidjson::Value* jsonValue, rapidjson::Document* document);
	virtual void readJSON(rapidjson::Value* jsonValue);

	Texture* albedo;
};

class ConstantMedium : public Hitable
{
public:
	ConstantMedium() : boundary(nullptr), density(0.0f), phase_function(nullptr) {}
	ConstantMedium(Hitable* b, float d, Texture* a) : boundary(b), density(d)
	{
		phase_function = new Isotropic(a);
	}
	virtual bool hit(const Ray& r, float t_min, float t_max, HitRecord& rec) const;
	virtual bool bounding_box(float t0, float t1, AABB& box) const;

	virtual void writeToJSON(rapidjson::Value* jsonValue, rapidjson::Document* document);
	virtual void readJSON(rapidjson::Value* jsonValue);

	Hitable* boundary;
	float density;
	Material *phase_function;
};

#endif
