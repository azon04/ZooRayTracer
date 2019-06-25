#ifndef _MATERIAL_H_
#define _MATERIAL_H_

#include "hitable.h"
#include "texture.h"
#include "pdf.h"

vec3 random_in_unit_sphere()
{
	vec3 p;
	do
	{
		p = 2.0f * vec3(rand_float(), rand_float(), rand_float()) - vec3(1.0f, 1.0f, 1.0f);
	} while (p.squared_length() >= 1.0f);

	return p;
}

float schlick(float cosine, float ref_idx)
{
	float r0 = (1 - ref_idx) / (1 + ref_idx);
	r0 = r0 * r0;
	return r0 + (1 - r0) * pow((1 - cosine), 5);
}

struct scatter_record
{
	ray specular_ray;
	bool is_specular;
	vec3 attenuation;
	pdf *pdf_ptr = nullptr;
};

class material
{
public:
	virtual bool scatter(const ray& r_in, const hit_record& rec, scatter_record& srec) const { return false; };
	virtual float scattering_pdf(const ray& r_in, const hit_record& rec, ray& scattered) const { return 1.0f; }
	virtual vec3 emitted(const ray& r_in, const hit_record& rec, float u, float v, const vec3& p) const { return vec3(0.0f); }
};

class lambertian : public material 
{
public:
	lambertian(texture* a) : albedo(a) 
	{
		pdf = new cosine_pdf(vec3());
	}

	virtual float scattering_pdf(const ray& r_in, const hit_record& rec, ray& scattered) const
	{
		float cosine = dot(rec.normal, unit_vector(scattered.direction()));
		if (cosine < 0.0f) cosine = 0.0f;
		return cosine / PI;
	}

	virtual bool scatter(const ray& r_in, const hit_record& rec, scatter_record& srec) const
	{
		srec.is_specular = false;
		srec.attenuation = albedo->value(rec.u, rec.v, rec.p);
		pdf->build_from_w(rec.normal);
		srec.pdf_ptr = pdf;
		return true;
	}
	cosine_pdf* pdf;
	texture* albedo;
};

class metal : public material
{
public:
	metal(const vec3& a, float f) : albedo(a) 
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
	virtual bool scatter(const ray& r_in, const hit_record& hrec, scatter_record& srec) const
	{
		vec3 reflected = reflect(unit_vector(r_in.direction()), hrec.normal);
		srec.specular_ray = ray(hrec.p, reflected + fuzz * random_in_unit_sphere(), r_in.time());
		srec.attenuation = albedo;
		srec.is_specular = true;
		srec.pdf_ptr = nullptr;
		return true;
	}

	vec3 albedo;
	float fuzz;
};

class dielectric : public material
{
public:
	dielectric(float ri) : ref_idx(ri) {}
	virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const
	{
		vec3 outward_normal;
		vec3 reflected = reflect(r_in.direction(), rec.normal);
		float ni_over_nt;
		float reflect_prob;
		float cosine;
		attenuation = vec3(1.0f, 1.0f, 1.0f);
		vec3 refracted;
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
			scattered = ray(rec.p, reflected, r_in.time());
		}
		else
		{
			scattered = ray(rec.p, refracted, r_in.time());
		}

		return true;
	}

	float ref_idx;
};

class diffuse_light : public material
{
public:
	diffuse_light(texture* a) : emit(a) {}
	virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const { return false; }
	virtual vec3 emitted(const ray& r_in, const hit_record& rec, float u, float v, const vec3& p) const
	{
		if (dot(rec.normal, r_in.direction()) < 0.0f)
			return emit->value(u, v, p);
		else
			return vec3(0.0f, 0.0f, 0.0f);
	}
	texture* emit;
};

#endif
