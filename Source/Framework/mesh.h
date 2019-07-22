#ifndef _MESH_H_
#define _MESH_H_

#include "hitable.h"
#include "bvh.h"

class triangle : public hitable
{
public:
	triangle(const vec3& p1, const vec3& p2, const vec3& p3, material* _mat)
	{
		points[0] = p1;
		points[1] = p2;
		points[2] = p3;
		mat = _mat;
	}

	~triangle() {}

	// Hitable functions
	virtual bool hit(const ray& r, float  t_min, float t_max, hit_record& rec) const;

	virtual bool bounding_box(float t0, float t1, aabb& box) const
	{
		vec3 min = points[0];
		vec3 max = points[0];
		for (int i = 1; i < 2; i++)
		{
			for (int el = 0; el < 3; el++)
			{
				if (min[el] > points[i][el])
				{
					min[el] = points[i][el];
				}

				if (max[el] < points[i][el])
				{
					max[el] = points[i][el];
				}
			}
		}

		for (int i = 0; i < 3; i++)
		{
			if (min[i] == max[i])
			{
				min[i] -= 0.00001f;
				max[i] += 0.00001f;
			}
		}

		box = aabb(min, max);
		return true;
	}

	material* mat;
	vec3 points[3];
};

bool triangle::hit(const ray& r, float t_min, float t_max, hit_record& rec) const
{
	vec3 v1 = points[0] - points[1];
	vec3 v2 = points[2] - points[1];
	vec3 normal_plane = unit_vector(cross(v1, v2));

	// This will make triangle double sided
	float dot_ = dot(-normal_plane, r.direction());
	if (dot_ < 0.0) { dot_ *= -1.0f; normal_plane = normal_plane * -1.0f; }

	float distance_plane = dot(normal_plane, points[0]);
	float distance = dot(normal_plane, r.origin()) - distance_plane;
	float t = distance / dot_;
	
	if (t < t_min || t > t_max)
		return false;

	vec3 pointAtT = r.point_at_parameter(t);
	vec3 pointInTriangle = pointAtT - points[1];
	vec3 v3 = points[2] - points[0];
	vec3 cross1 = cross(v1, pointInTriangle);
	vec3 cross2 = cross(pointInTriangle, v2);
	vec3 cross3 = cross(v3, pointAtT - points[0]);

	if (!(dot(cross1, cross2) > 0.0f && dot(cross2, cross3) > 0.0f))
		return false;

	// #TODO the triangle class has no UV now, compute UV using barycentric coordinates
	rec.u = 0.0f;
	rec.v = 0.0f;

	rec.t = t;
	rec.p = r.point_at_parameter(t);
	rec.mat_ptr = mat;
	rec.normal = normal_plane;
	
	return true;
}

class mesh : public hitable
{
public:

	mesh(int triangleSize)
		: triangle_size(triangleSize),
		triangle_count(0),
		mat(nullptr)
	{
		triangles = new triangle*[triangleSize];
	}
	~mesh() {}

	void addTriangle(triangle* newTriangle)
	{
		triangles[triangle_count++] = newTriangle;
	}

	void setupMesh()
	{
		triangle_bvh = new bvh_node((hitable**)triangles, triangle_count, 0.0f, 1.0f);
	}

	virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const override
	{
		if (triangle_bvh->hit(r, t_min, t_max, rec))
		{
			rec.mat_ptr = mat;
			return true;
		}
		return false;
	}

	virtual bool bounding_box(float t0, float t1, aabb& box) const override
	{
		return triangle_bvh->bounding_box(t0, t1, box);
	}

	bvh_node* triangle_bvh;
	triangle** triangles;
	material* mat;
	int triangle_count;
	int triangle_size;
};

#endif
