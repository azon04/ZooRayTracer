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
