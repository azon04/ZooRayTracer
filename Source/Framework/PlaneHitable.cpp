#include "PlaneHitable.h"

bool XYRect::hit(const Ray& r, float t_min, float t_max, HitRecord& rec) const
{
	float t = (k - r.origin().z()) / r.direction().z();
	if (t < t_min || t > t_max)
		return false;
	float x = r.origin().x() + t * r.direction().x();
	float y = r.origin().y() + t * r.direction().y();
	if (x < x0 || x > x1 || y < y0 || y > y1)
		return false;
	rec.u = (x - x0) / (x1 - x0);
	rec.v = (y - y0) / (y1 - y0);
	rec.t = t;
	rec.p = r.point_at_parameter(t);
	rec.mat_ptr = mat;
	rec.normal = Vec3(0.0f, 0.0f, 1.0f);
	return true;
}

void XYRect::writeToJSON(rapidjson::Value* jsonValue, rapidjson::Document* document)
{
	Hitable::writeToJSON(jsonValue, document);

	rapidjson::Value::Object& jsonObject = jsonValue->GetObject();
	jsonObject.AddMember("Class", "XYRect", document->GetAllocator());
	jsonObject.AddMember("X0", x0, document->GetAllocator());
	jsonObject.AddMember("X1", x1, document->GetAllocator());
	jsonObject.AddMember("Y0", y0, document->GetAllocator());
	jsonObject.AddMember("Y1", y1, document->GetAllocator());
	jsonObject.AddMember("K", k, document->GetAllocator());

	if (mat)
	{
		rapidjson::Value matValue;
		mat->writeToJSON(&matValue, document);
		jsonObject.AddMember("Material", matValue, document->GetAllocator());
	}
}

void XYRect::readJSON(rapidjson::Value* jsonValue)
{
	Hitable::readJSON(jsonValue);

	rapidjson::Value::Object jsonObject = jsonValue->GetObject();
	READ_JSON_OBJECT_FLOAT(jsonObject, "Y0", y0);
	READ_JSON_OBJECT_FLOAT(jsonObject, "Y1", y1);
	READ_JSON_OBJECT_FLOAT(jsonObject, "X0", x0);
	READ_JSON_OBJECT_FLOAT(jsonObject, "X1", x1);
	READ_JSON_OBJECT_FLOAT(jsonObject, "K", k);
	READ_JSON_OBJECT_MATERIAL(jsonObject, "Material", mat);
}

bool XZRect::hit(const Ray& r, float t_min, float t_max, HitRecord& rec) const
{
	float t = (k - r.origin().y()) / r.direction().y();
	if (t < t_min || t > t_max)
		return false;
	float x = r.origin().x() + t * r.direction().x();
	float z = r.origin().z() + t * r.direction().z();
	if (x < x0 || x > x1 || z < z0 || z > z1)
		return false;
	rec.u = (x - x0) / (x1 - x0);
	rec.v = (z - z0) / (z1 - z0);
	rec.t = t;
	rec.p = r.point_at_parameter(t);
	rec.mat_ptr = mat;
	rec.normal = Vec3(0.0f, 1.0f, 0.0f);
	return true;
}

void XZRect::writeToJSON(rapidjson::Value* jsonValue, rapidjson::Document* document)
{
	Hitable::writeToJSON(jsonValue, document);

	rapidjson::Value::Object& jsonObject = jsonValue->GetObject();
	jsonObject.AddMember("Class", "XZRect", document->GetAllocator());
	jsonObject.AddMember("X0", x0, document->GetAllocator());
	jsonObject.AddMember("X1", x1, document->GetAllocator());
	jsonObject.AddMember("Z0", z0, document->GetAllocator());
	jsonObject.AddMember("Z1", z1, document->GetAllocator());
	jsonObject.AddMember("K", k, document->GetAllocator());

	if (mat)
	{
		rapidjson::Value matValue;
		mat->writeToJSON(&matValue, document);
		jsonObject.AddMember("Material", matValue, document->GetAllocator());
	}
}

void XZRect::readJSON(rapidjson::Value* jsonValue)
{
	Hitable::readJSON(jsonValue);

	rapidjson::Value::Object jsonObject = jsonValue->GetObject();
	READ_JSON_OBJECT_FLOAT(jsonObject, "X0", x0);
	READ_JSON_OBJECT_FLOAT(jsonObject, "X1", x1);
	READ_JSON_OBJECT_FLOAT(jsonObject, "Z0", z0);
	READ_JSON_OBJECT_FLOAT(jsonObject, "Z1", z1);
	READ_JSON_OBJECT_FLOAT(jsonObject, "K", k);
	READ_JSON_OBJECT_MATERIAL(jsonObject, "Material", mat);
}

bool YZRect::hit(const Ray& r, float t_min, float t_max, HitRecord& rec) const
{
	float t = (k - r.origin().x()) / r.direction().x();
	if (t < t_min || t > t_max)
		return false;
	float y = r.origin().y() + t * r.direction().y();
	float z = r.origin().z() + t * r.direction().z();
	if (y < y0 || y > y1 || z < z0 || z > z1)
		return false;
	rec.u = (y - y0) / (y1 - y0);
	rec.v = (z - z0) / (z1 - z0);
	rec.t = t;
	rec.p = r.point_at_parameter(t);
	rec.mat_ptr = mat;
	rec.normal = Vec3(1.0f, 0.0f, 0.0f);
	return true;
}

void YZRect::writeToJSON(rapidjson::Value* jsonValue, rapidjson::Document* document)
{
	Hitable::writeToJSON(jsonValue, document);

	rapidjson::Value::Object& jsonObject = jsonValue->GetObject();
	jsonObject.AddMember("Class", "YZRect", document->GetAllocator());
	jsonObject.AddMember("Y0", y0, document->GetAllocator());
	jsonObject.AddMember("Y1", y1, document->GetAllocator());
	jsonObject.AddMember("Z0", z0, document->GetAllocator());
	jsonObject.AddMember("Z1", z1, document->GetAllocator());
	jsonObject.AddMember("K", k, document->GetAllocator());

	if (mat)
	{
		rapidjson::Value matValue;
		mat->writeToJSON(&matValue, document);
		jsonObject.AddMember("Material", matValue, document->GetAllocator());
	}
}

void YZRect::readJSON(rapidjson::Value* jsonValue)
{
	Hitable::readJSON(jsonValue);

	rapidjson::Value::Object jsonObject = jsonValue->GetObject();
	READ_JSON_OBJECT_FLOAT(jsonObject, "Y0", y0);
	READ_JSON_OBJECT_FLOAT(jsonObject, "Y1", y1);
	READ_JSON_OBJECT_FLOAT(jsonObject, "Z0", z0);
	READ_JSON_OBJECT_FLOAT(jsonObject, "Z1", z1);
	READ_JSON_OBJECT_FLOAT(jsonObject, "K", k);
	READ_JSON_OBJECT_MATERIAL(jsonObject, "Material", mat);

}