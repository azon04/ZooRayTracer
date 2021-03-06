#include "Mesh.h"
#include "Material.h"
#include "JSONHelper.h"

bool Triangle::bounding_box(float t0, float t1, AABB& box) const
{
	Vec3 min = points[0];
	Vec3 max = points[0];
	for (int i = 1; i < 3; i++)
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

	box = AABB(min, max);
	return true;
}

void Triangle::writeToJSON(rapidjson::Value* jsonValue, rapidjson::Document* document)
{
	Hitable::writeToJSON(jsonValue, document);

	rapidjson::Value::Object& jsonObject = jsonValue->GetObject();
	jsonObject.AddMember("Class", "Triangle", document->GetAllocator());

	if (mat)
	{
		rapidjson::Value matValue;
		mat->writeToJSON(&matValue, document);
		jsonObject.AddMember("Material", matValue, document->GetAllocator());
	}

	{
		rapidjson::Value value;
		Vec3ToJSON(points[0], value, document);
		jsonObject.AddMember("Point0", value, document->GetAllocator());
	}

	{
		rapidjson::Value value;
		Vec3ToJSON(points[1], value, document);
		jsonObject.AddMember("Point1", value, document->GetAllocator());
	}

	{
		rapidjson::Value value;
		Vec3ToJSON(points[2], value, document);
		jsonObject.AddMember("Point2", value, document->GetAllocator());
	}
}

void Triangle::readJSON(rapidjson::Value* jsonValue)
{
	Hitable::readJSON(jsonValue);

	rapidjson::Value::Object jsonObject = jsonValue->GetObject();
	READ_JSON_OBJECT_MATERIAL(jsonObject, "Material", mat);
	READ_JSON_OBJECT_VEC3(jsonObject, "Point0", points[0]);
	READ_JSON_OBJECT_VEC3(jsonObject, "Point1", points[1]);
	READ_JSON_OBJECT_VEC3(jsonObject, "Point2", points[2]);
}

bool Triangle::hit(const Ray& r, float t_min, float t_max, HitRecord& rec) const
{
	Vec3 v1 = points[0] - points[1];
	Vec3 v2 = points[2] - points[1];
	Vec3 normal_plane = unit_vector(cross(v1, v2));

	// This will make triangle double sided
	float dot_ = dot(-normal_plane, r.direction());
	if (dot_ < 0.0) { dot_ *= -1.0f; normal_plane = normal_plane * -1.0f; }

	float distance_plane = dot(normal_plane, points[0]);
	float distance = dot(normal_plane, r.origin()) - distance_plane;
	float t = distance / dot_;

	if (t < t_min || t > t_max)
		return false;

	Vec3 pointAtT = r.point_at_parameter(t);
	Vec3 pointInTriangle = pointAtT - points[1];
	Vec3 v3 = points[2] - points[0];
	Vec3 cross1 = cross(v1, pointInTriangle);
	Vec3 cross2 = cross(pointInTriangle, v2);
	Vec3 cross3 = cross(v3, pointAtT - points[0]);

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

void Mesh::setAssetPath(const char* assetPath)
{
	std::stringstream stringStream;
	stringStream << assetPath;
	stringStream >> this->assetPath;
}

bool Mesh::hit(const Ray& r, float t_min, float t_max, HitRecord& rec) const
{
	if (triangle_bvh->hit(r, t_min, t_max, rec))
	{
		rec.mat_ptr = mat;
		return true;
	}
	return false;
}

void Mesh::writeToJSON(rapidjson::Value* jsonValue, rapidjson::Document* document)
{
	Hitable::writeToJSON(jsonValue, document);

	rapidjson::Value::Object& jsonObject = jsonValue->GetObject();
	jsonObject.AddMember("Class", "Mesh", document->GetAllocator());
	jsonObject.AddMember("MeshPath", rapidjson::StringRef(assetPath), document->GetAllocator());

	if (mat)
	{
		rapidjson::Value matValue;
		mat->writeToJSON(&matValue, document);
		jsonObject.AddMember("Material", matValue, document->GetAllocator());
	}
}

void Mesh::readJSON(rapidjson::Value* jsonValue)
{
	Hitable::readJSON(jsonValue);
	rapidjson::Value::Object jsonObject = jsonValue->GetObject();

	if (jsonObject.HasMember("MeshPath"))
	{
		strncpy(assetPath, jsonObject["MeshPath"].GetString(), jsonObject["MeshPath"].GetStringLength());
		// #TODO Load Mesh
	}

	READ_JSON_OBJECT_MATERIAL(jsonObject, "Material", mat);
}
