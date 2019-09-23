#ifndef _PLANE_H_
#define _PLANE_H_

#include "Hitable.h"
#include "Material.h"

class XYRect : public Hitable
{
public:
	XYRect() {}
	XYRect(float _x0, float _x1, float _y0, float _y1, float _k, Material* _mat) : x0(_x0), x1(_x1), y0(_y0), y1(_y1), k(_k), mat(_mat) {}
	virtual bool hit(const Ray& r, float t_min, float t_max, HitRecord& rec) const;
	virtual bool bounding_box(float t0, float t1, AABB& box) const
	{
		box = AABB(Vec3(x0, y0, k - 0.00001f), Vec3(x1, y1, k + 0.00001f));
		return true;
	}

	virtual void writeToJSON(rapidjson::Value* jsonValue, rapidjson::Document* document);
	virtual void readJSON(rapidjson::Value* jsonValue);

	Material* mat;
	float x0, x1, y0, y1, k;
};

class XZRect : public Hitable
{
public:
	XZRect() {}
	XZRect(float _x0, float _x1, float _z0, float _z1, float _k, Material* _mat) : x0(_x0), x1(_x1), z0(_z0), z1(_z1), k(_k), mat(_mat)  {}
	virtual bool hit(const Ray& r, float t_min, float t_max, HitRecord& rec) const;
	virtual bool bounding_box(float t0, float t1, AABB& box) const
	{
		box = AABB(Vec3(x0, k - 0.00001f, z0), Vec3(x1, k + 0.00001f, z1));
		return true;
	}

	virtual float pdf_value(const Vec3& o, const Vec3& v) const
	{
		HitRecord rec;
		if (this->hit(Ray(o, v), 0.0001f, FLT_MAX, rec))
		{
			float area = (x1 - x0) * (z1 - z0);
			float distance_squared = rec.t * rec.t * v.squared_length();
			float cosine = fabs(dot(v, rec.normal) / v.length());
			return distance_squared / (cosine * area);
		}
		else
			return 0.0f;
	}

	virtual Vec3 random(const Vec3& o) const
	{
		Vec3 random_point = Vec3(x0 + rand_float() * (x1 - x0), k, z0 + rand_float() * (z1 - z0));
		return random_point - o;
	}

	virtual void writeToJSON(rapidjson::Value* jsonValue, rapidjson::Document* document);
	virtual void readJSON(rapidjson::Value* jsonValue);

	Material* mat;
	float x0, x1, z0, z1, k;
};

class YZRect : public Hitable
{
public:
	YZRect() {}
	YZRect(float _y0, float _y1, float _z0, float _z1, float _k, Material* _mat) : y0(_y0), y1(_y1), z0(_z0), z1(_z1), k(_k), mat(_mat) {}
	virtual bool hit(const Ray& r, float t_min, float t_max, HitRecord& rec) const;
	virtual bool bounding_box(float t0, float t1, AABB& box) const
	{
		box = AABB(Vec3(k - 0.00001f, y0, z0), Vec3(k + 0.00001f, y1, z1));
		return true;
	}

	virtual void writeToJSON(rapidjson::Value* jsonValue, rapidjson::Document* document);
	virtual void readJSON(rapidjson::Value* jsonValue);

	Material* mat;
	float y0, y1, z0, z1, k;
};

#endif
