#include "Renderer.h"
#include "Material.h"

#include <iostream>

Vec3 de_nan(const Vec3& c)
{
	Vec3 temp = c;
	if (!(temp[0] == temp[0])) temp[0] = 0.0f;
	if (!(temp[1] == temp[1])) temp[1] = 0.0f;
	if (!(temp[2] == temp[2])) temp[2] = 0.0f;
	return temp;
}

void Renderer::render(Camera camera, Hitable* world, Hitable* light_shape)
{
	std::cout << "P3\n" << width << " " << height << "\n255\n";
	for (int j = height - 1; j >= 0; j--)
	{
		for (int i = 0; i < width; i++)
		{
			Vec3 col(0.0f, 0.0f, 0.0f);
			for (int s = 0; s < sample_count; s++)
			{
				float u = float(i + rand_float()) / float(width);
				float v = float(j + rand_float()) / float(height);
				Ray r = camera.get_ray(u, v);
				col += de_nan(color(r, world, light_shape, 0));
			}
			col /= float(sample_count);
			col = Vec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));
			//col = col / (col + vec3(1.0f));
			int ir = int(255.99 * fmin(1.0f, col.r()));
			int ig = int(255.99 * fmin(1.0f, col.g()));
			int ib = int(255.99 * fmin(1.0f, col.b()));

			std::cout << ir << " " << ig << " " << ib << "\n";
		}
	}
}

Vec3 Renderer::color(const Ray& r, Hitable* world, Hitable* light_shape, int depth)
{
	HitRecord rec;
	if (world->hit(r, 0.001f, FLT_MAX, rec))
	{
		ScatterRecord srec;
		Vec3 emitted = rec.mat_ptr->emitted(r, rec, rec.u, rec.v, rec.p);
		float pdf_val;
		if (depth < ray_depth && rec.mat_ptr->scatter(r, rec, srec))
		{
			if (srec.is_specular)
			{
				return srec.attenuation * color(srec.specular_ray, world, light_shape, depth + 1);
			}
			else if (light_shape)
			{
				HitablePDF p0(light_shape, rec.p);
				MixturePDF p(&p0, srec.pdf_ptr);
				Ray scattered = Ray(rec.p, p.generate(), r.time());
				pdf_val = p.value(scattered.direction());
				return emitted + srec.attenuation * rec.mat_ptr->scattering_pdf(r, rec, scattered) * color(scattered, world, light_shape, depth + 1) / pdf_val;
			}
			else
			{
				CosinePDF p(rec.normal);
				Ray scattered = Ray(rec.p, p.generate(), r.time());
				pdf_val = p.value(scattered.direction());
				return emitted + srec.attenuation * rec.mat_ptr->scattering_pdf(r, rec, scattered) * color(scattered, world, light_shape, depth + 1) / pdf_val;
			}
		}
		else
		{
			return emitted;
		}
	}
	else
	{
		return Vec3(0.0f);
	}
}

