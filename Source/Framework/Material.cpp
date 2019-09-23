#include "Material.h"

float schlick(float cosine, float ref_idx)
{
	float r0 = (1 - ref_idx) / (1 + ref_idx);
	r0 = r0 * r0;
	return r0 + (1 - r0) * pow((1 - cosine), 5);
}

bool Dielectric::scatter(const Ray& r_in, const HitRecord& rec, ScatterRecord& srec) const
{
	srec.is_specular = true;
	srec.pdf_ptr = 0;
	Vec3 outward_normal;
	Vec3 reflected = reflect(r_in.direction(), rec.normal);
	float ni_over_nt;
	float reflect_prob;
	float cosine;
	srec.attenuation = Vec3(1.0f, 1.0f, 1.0f);
	Vec3 refracted;
	if (dot(r_in.direction(), rec.normal) > 0)
	{
		outward_normal = -rec.normal;
		ni_over_nt = ref_idx;
		cosine = ref_idx * dot(r_in.direction(), rec.normal) / r_in.direction().length();
	}
	else
	{
		outward_normal = rec.normal;
		ni_over_nt = 1.0f / ref_idx;
		cosine = -dot(r_in.direction(), rec.normal) / r_in.direction().length();
	}

	if (refract(r_in.direction(), outward_normal, ni_over_nt, refracted))
	{
		reflect_prob = schlick(cosine, ref_idx);
	}
	else
	{
		reflect_prob = 1.0f;
	}

	if (rand() / float(RAND_MAX) < reflect_prob)
	{
		srec.specular_ray = Ray(rec.p, reflected, r_in.time());
	}
	else
	{
		srec.specular_ray = Ray(rec.p, refracted, r_in.time());
	}

	return true;	
}

void Dielectric::writeToJSON(rapidjson::Value* jsonValue, rapidjson::Document* document)
{
	Material::writeToJSON(jsonValue, document);

	rapidjson::Value::Object& jsonObject = jsonValue->GetObject();

	jsonObject.AddMember("Class", "Dielectric", document->GetAllocator());
	jsonObject.AddMember("RefIndex", ref_idx, document->GetAllocator());
}

void Dielectric::readJSON(rapidjson::Value* jsonValue)
{
	Material::readJSON(jsonValue);

	rapidjson::Value::Object jsonObject = jsonValue->GetObject();
	READ_JSON_OBJECT_FLOAT(jsonObject, "RefIndex", ref_idx);
}

void Material::writeToJSON(rapidjson::Value* jsonValue, rapidjson::Document* document)
{
	jsonValue->SetObject();
	rapidjson::Value::Object jsonObject = jsonValue->GetObject();
	jsonObject.AddMember("Type", "Material", document->GetAllocator());
}

void Material::readJSON(rapidjson::Value* jsonValue)
{

}

void Lambertian::writeToJSON(rapidjson::Value* jsonValue, rapidjson::Document* document)
{
	Material::writeToJSON(jsonValue, document);

	rapidjson::Value::Object& jsonObject = jsonValue->GetObject();
	jsonObject.AddMember("Class", "Lambertian", document->GetAllocator());

	rapidjson::Value textureValue;
	if (albedo)
	{
		albedo->writeToJSON(&textureValue, document);
	}
	jsonObject.AddMember("Albedo", textureValue, document->GetAllocator());
}

void Lambertian::readJSON(rapidjson::Value* jsonValue)
{
	Material::readJSON(jsonValue);
	rapidjson::Value::Object jsonObject = jsonValue->GetObject();
	READ_JSON_OBJECT_TEXTURE(jsonObject, "Albedo", albedo);
}

void Metal::writeToJSON(rapidjson::Value* jsonValue, rapidjson::Document* document)
{
	Material::writeToJSON(jsonValue, document);

	rapidjson::Value::Object& jsonObject = jsonValue->GetObject();

	jsonObject.AddMember("Class", "Metal", document->GetAllocator());
	jsonObject.AddMember("Fuzziness", fuzz, document->GetAllocator());
	
	rapidjson::Value albedoValue;
	Vec3ToJSON(albedo, albedoValue, document);
	jsonObject.AddMember("Albedo", albedoValue, document->GetAllocator());
}

void Metal::readJSON(rapidjson::Value* jsonValue)
{
	Material::readJSON(jsonValue);

	rapidjson::Value::Object jsonObject = jsonValue->GetObject();

	READ_JSON_OBJECT_FLOAT(jsonObject, "Fuzziness", fuzz);
	READ_JSON_OBJECT_VEC3(jsonObject, "Albedo", albedo);
}

void DiffuseLight::writeToJSON(rapidjson::Value* jsonValue, rapidjson::Document* document)
{
	Material::writeToJSON(jsonValue, document);

	rapidjson::Value::Object& jsonObject = jsonValue->GetObject();

	jsonObject.AddMember("Class", "DiffuseLight", document->GetAllocator());

	if(emit)
	{
		rapidjson::Value emitTextureValue;
		emit->writeToJSON(&emitTextureValue, document);
		jsonObject.AddMember("EmitterTexture", emitTextureValue, document->GetAllocator());
	}
}

void DiffuseLight::readJSON(rapidjson::Value* jsonValue)
{
	Material::readJSON(jsonValue);

	rapidjson::Value::Object jsonObject = jsonValue->GetObject();

	READ_JSON_OBJECT_TEXTURE(jsonObject, "EmitterTexture", emit);
}
