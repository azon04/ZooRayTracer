#ifndef _MATERIAL_H_
#define _MATERIAL_H_

#include "Hitable.h"
#include "Texture.h"
#include "PDF.h"
#include "JSONParseable.h"

struct ScatterRecord
{
	Ray specular_ray;
	bool is_specular;
	Vec3 attenuation;
	PDF *pdf_ptr = nullptr;
};

class Material : public JSONParseable
{
public:
	virtual bool scatter(const Ray& r_in, const HitRecord& rec, ScatterRecord& srec) const { return false; };
	virtual float scattering_pdf(const Ray& r_in, const HitRecord& rec, Ray& scattered) const { return 1.0f; }
	virtual Vec3 emitted(const Ray& r_in, const HitRecord& rec, float u, float v, const Vec3& p) const { return Vec3(0.0f); }

	virtual void writeToJSON(rapidjson::Value* jsonValue, rapidjson::Document* document);
	virtual void readJSON(rapidjson::Value* jsonValue);
};

class Lambertian : public Material 
{
public:
	Lambertian()
	{
		pdf = new CosinePDF(Vec3());
	}

	Lambertian(Texture* a) : albedo(a) 
	{
		pdf = new CosinePDF(Vec3());
	}

	virtual float scattering_pdf(const Ray& r_in, const HitRecord& rec, Ray& scattered) const
	{
		float cosine = dot(rec.normal, unit_vector(scattered.direction()));
		if (cosine < 0.0f) cosine = 0.0f;
		return cosine / PI;
	}

	virtual bool scatter(const Ray& r_in, const HitRecord& rec, ScatterRecord& srec) const
	{
		srec.is_specular = false;
		srec.attenuation = albedo->value(rec.u, rec.v, rec.p);
		pdf->build_from_w(rec.normal);
		srec.pdf_ptr = pdf;
		return true;
	}

	virtual void writeToJSON(rapidjson::Value* jsonValue, rapidjson::Document* document);
	virtual void readJSON(rapidjson::Value* jsonValue);

	CosinePDF* pdf;
	Texture* albedo;
};

class Metal : public Material
{
public:
	Metal()
	{
		albedo = Vec3(0.0f);
		fuzz = 0.0f;
	}

	Metal(const Vec3& a, float f) : albedo(a) 
	{
		if (f < 1.0f)
		{
			fuzz = f;
		}
		else
		{
			fuzz = 1.0f;
		}
	}
	virtual bool scatter(const Ray& r_in, const HitRecord& hrec, ScatterRecord& srec) const
	{
		Vec3 reflected = reflect(unit_vector(r_in.direction()), hrec.normal);
		srec.specular_ray = Ray(hrec.p, reflected + fuzz * random_in_unit_sphere(), r_in.time());
		srec.attenuation = albedo;
		srec.is_specular = true;
		srec.pdf_ptr = nullptr;
		return true;
	}

	virtual void writeToJSON(rapidjson::Value* jsonValue, rapidjson::Document* document);
	virtual void readJSON(rapidjson::Value* jsonValue);

	Vec3 albedo;
	float fuzz;
};

class Dielectric : public Material
{
public:
	Dielectric() : ref_idx(1.0f) {}
	Dielectric(float ri) : ref_idx(ri) {}
	virtual bool scatter(const Ray& r_in, const HitRecord& rec, ScatterRecord& srec) const;

	virtual void writeToJSON(rapidjson::Value* jsonValue, rapidjson::Document* document);
	virtual void readJSON(rapidjson::Value* jsonValue);

	float ref_idx;
};

class DiffuseLight : public Material
{
public:
	DiffuseLight() : emit(nullptr) {}
	DiffuseLight(Texture* a) : emit(a) {}
	virtual bool scatter(const Ray& r_in, const HitRecord& rec, ScatterRecord& srec) const { return false; }
	virtual Vec3 emitted(const Ray& r_in, const HitRecord& rec, float u, float v, const Vec3& p) const
	{
		if (dot(rec.normal, r_in.direction()) < 0.0f)
			return emit->value(u, v, p);
		else
			return Vec3(0.0f, 0.0f, 0.0f);
	}

	virtual void writeToJSON(rapidjson::Value* jsonValue, rapidjson::Document* document);
	virtual void readJSON(rapidjson::Value* jsonValue);

	Texture* emit;
};

#endif
