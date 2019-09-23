#ifndef _SPHERE_H_
#define _SPHERE_H_

#include "Hitable.h"
#include "Material.h"
#include "math.h"
#include "math_utils.h"

class Material;

void get_sphere_uv(const Vec3& p, float& u, float& v);
Vec3 random_to_sphere(float radius, float distance_squared);

class Sphere : public Hitable
{
public:
	Sphere() {}
	Sphere(Vec3 _center, float _radius, Material* mat) : center(_center), radius(_radius), mat_ptr(mat) {}

	virtual bool hit(const Ray& r, float t_min, float t_max, HitRecord& rec) const override;
	virtual bool bounding_box(float t0, float t1, AABB& box) const override;
	virtual float pdf_value(const Vec3& o, const Vec3& v) const;
	virtual Vec3 random(const Vec3& o) const;

	virtual void writeToJSON(rapidjson::Value* jsonValue, rapidjson::Document* document);
	virtual void readJSON(rapidjson::Value* jsonValue);

	Vec3 center;
	float radius;
	Material* mat_ptr;
};

class MovingSphere : public Hitable
{
public:
	MovingSphere() {}
	MovingSphere(Vec3 cen0, Vec3 cen1, float t0, float t1, float r, Material* m)
		: center0(cen0), center1(cen1), time0(t0), time1(t1), radius(r), mat_ptr(m)
	{}

	virtual bool hit(const Ray& r, float t_min, float t_max, HitRecord& rec) const;
	virtual bool bounding_box(float t0, float t1, AABB& box) const;

	virtual void writeToJSON(rapidjson::Value* jsonValue, rapidjson::Document* document);
	virtual void readJSON(rapidjson::Value* jsonValue);

	Vec3 center(float time) const;

	Vec3 center0, center1;
	float time0, time1;
	float radius;
	Material* mat_ptr;
};

#endif
