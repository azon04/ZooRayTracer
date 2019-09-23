#include "ConstantMedium.h"

bool Isotropic::scatter(const Ray& r_in, const HitRecord& rec, ScatterRecord& srec) const
{
	srec.is_specular = true;
	srec.specular_ray = Ray(rec.p, random_in_unit_sphere());
	srec.attenuation = albedo->value(rec.u, rec.v, rec.p);
	return true;
}

void Isotropic::writeToJSON(rapidjson::Value* jsonValue, rapidjson::Document* document)
{
	Material::writeToJSON(jsonValue, document);

	rapidjson::Value::Object& jsonObject = jsonValue->GetObject();

	jsonObject.AddMember("Class", "Isotropic", document->GetAllocator());

	rapidjson::Value textureValue;
	if (albedo)
	{
		albedo->writeToJSON(&textureValue, document);
	}
	jsonObject.AddMember("Albedo", textureValue, document->GetAllocator());
}

void Isotropic::readJSON(rapidjson::Value* jsonValue)
{
	Material::readJSON(jsonValue);

	rapidjson::Value::Object jsonObject = jsonValue->GetObject();

	READ_JSON_OBJECT_TEXTURE(jsonObject, "Albedo", albedo);
}

bool ConstantMedium::bounding_box(float t0, float t1, AABB& box) const
{
	return boundary->bounding_box(t0, t1, box);
}

void ConstantMedium::writeToJSON(rapidjson::Value* jsonValue, rapidjson::Document* document)
{
	Hitable::writeToJSON(jsonValue, document);

	rapidjson::Value::Object& jsonObject = jsonValue->GetObject();

	jsonObject.AddMember("Class", "ConstantMedium", document->GetAllocator());
	jsonObject.AddMember("Density", density, document->GetAllocator());

	if (boundary)
	{
		rapidjson::Value boundaryValue;
		boundary->writeToJSON(&boundaryValue, document);
		jsonObject.AddMember("Boundary", boundaryValue, document->GetAllocator());
	}

	if (phase_function)
	{
		rapidjson::Value phaseFunctionValue;
		phase_function->writeToJSON(&phaseFunctionValue, document);
		jsonObject.AddMember("PhaseFunction", phaseFunctionValue, document->GetAllocator());
	}
}

void ConstantMedium::readJSON(rapidjson::Value* jsonValue)
{
	Hitable::readJSON(jsonValue);

	rapidjson::Value::Object jsonObject = jsonValue->GetObject();
	READ_JSON_OBJECT_FLOAT(jsonObject, "Density", density);
	READ_JSON_OBJECT_HITABLE(jsonObject, "Boundary", boundary);
	READ_JSON_OBJECT_MATERIAL(jsonObject, "PhaseFunction", phase_function);
}

bool ConstantMedium::hit(const Ray& r, float t_min, float t_max, HitRecord& rec) const
{
	HitRecord rec1, rec2;
	if (boundary->hit(r, -FLT_MAX, FLT_MAX, rec1))
	{
		if (boundary->hit(r, rec1.t + 0.0001f, FLT_MAX, rec2))
		{
			if (rec1.t < t_min)
				rec1.t = t_min;
			if (rec2.t > t_max)
				rec2.t = t_max;
			if (rec1.t >= rec2.t)
				return false;
			if (rec1.t < 0)
				rec1.t = 0;
			float distance_inside_boundary = (rec2.t - rec1.t) * r.direction().length();
			float hit_distance = -(1 / density) * log(rand_float());
			if (hit_distance < distance_inside_boundary)
			{
				rec.t = rec1.t + hit_distance / r.direction().length();
				rec.p = r.point_at_parameter(rec.t);
				rec.normal = Vec3(1.0f, 0.0f, 0.0f); // arbitrary
				rec.mat_ptr = phase_function;
				return true;
			}
		}
	}
	return false;
}

