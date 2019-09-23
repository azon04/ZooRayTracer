#ifndef _MESH_H_
#define _MESH_H_

#include "Hitable.h"
#include "bvh.h"

class Triangle : public Hitable
{
public:
	Triangle() : mat(nullptr) {}
	Triangle(const Vec3& p1, const Vec3& p2, const Vec3& p3, Material* _mat)
	{
		points[0] = p1;
		points[1] = p2;
		points[2] = p3;
		mat = _mat;
	}

	~Triangle() {}

	// Hitable functions
	virtual bool hit(const Ray& r, float  t_min, float t_max, HitRecord& rec) const;
	virtual bool bounding_box(float t0, float t1, AABB& box) const;

	virtual void writeToJSON(rapidjson::Value* jsonValue, rapidjson::Document* document);
	virtual void readJSON(rapidjson::Value* jsonValue);

	Material* mat;
	Vec3 points[3];
};

class Mesh : public Hitable
{
public:

	Mesh() {}
	Mesh(int triangleSize)
		: triangle_size(triangleSize),
		triangle_count(0),
		mat(nullptr)
	{
		triangles = new Triangle*[triangleSize];
	}
	~Mesh() {}

	void setAssetPath(const char* assetPath);

	void addTriangle(Triangle* newTriangle)
	{
		triangles[triangle_count++] = newTriangle;
	}

	void setupMesh()
	{
		triangle_bvh = new BVHNode((Hitable**)triangles, triangle_count, 0.0f, 1.0f);
	}

	virtual bool hit(const Ray& r, float t_min, float t_max, HitRecord& rec) const override;

	virtual bool bounding_box(float t0, float t1, AABB& box) const override
	{
		return triangle_bvh->bounding_box(t0, t1, box);
	}

	virtual void writeToJSON(rapidjson::Value* jsonValue, rapidjson::Document* document);
	virtual void readJSON(rapidjson::Value* jsonValue);

	char assetPath[50];
	BVHNode* triangle_bvh;
	Triangle** triangles;
	Material* mat;
	int triangle_count;
	int triangle_size;
};

#endif
