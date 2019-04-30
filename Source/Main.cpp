#include <iostream>
#include "ray.h"
#include "hitablelist.h"
#include "sphere.h"
#include "float.h"
#include "camera.h"
#include "material.h"
#include <stdlib.h>
#include "rand_helper.h"

vec3 color(const ray& r, hitable_list* world, int depth)
{
	hit_record rec;
	if (world->hit(r, 0.001f, FLT_MAX, rec))
	{
		ray scattered;
		vec3 attenuation;
		if (depth < 40 && rec.mat_ptr->scatter(r, rec, attenuation, scattered))
		{
			return attenuation * color(scattered, world, depth + 1);
		}
		else
		{
			return vec3(0.0f, 0.0f, 0.0f);
		}
	}
	else
	{
		vec3 unit_direction = unit_vector(r.direction());
		float t = 0.5f * (unit_direction.y() + 1.0f);
		return (1.0f - t) * vec3(1.0f, 1.0f, 1.0f) + t * vec3(0.5f, 0.7f, 1.0f);
	}
}

hitable_list* random_scene()
{
	int n = 500;
	hitable** list = new hitable*[n + 1];
	list[0] = new sphere(vec3(0, -1000,0.0f), 1000, new lambertian(vec3(0.5f, 0.5f, 0.5f)));
	int i = 1;
	for (int a = -11; a < 11; a++)
	{
		for (int b = -11; b < 11; b++)
		{
			float choose_mat = rand_float();
			vec3 center(a + 0.9 * rand_float(), 0.2f, b + 9 * rand_float());
			if ((center - vec3(4.0f, 0.2f, 0)).length() > 0.9)
			{
				if (choose_mat < 0.8f) // diffuse
				{
					list[i++] = new sphere(center, 0.2f, new lambertian(vec3(rand_float() * rand_float(), rand_float() * rand_float(), rand_float() * rand_float())));
				}
				else if (choose_mat < 0.95f) // metal
				{
					list[i++] = new sphere(center, 0.2f, new metal(vec3(0.5f * (1 - rand_float()), 0.5f * (1 - rand_float()), 0.5f * (1 - rand_float())), 0.5f * rand_float()));
				}
				else // glass
				{
					list[i++] = new sphere(center, 0.2f, new dielectric(1.5f));
				}
			}
		}
	}

	list[i++] = new sphere(vec3(0.0f, 1.0f, 0.0f), 1.0f, new dielectric(1.5f));
	list[i++] = new sphere(vec3(-4.0f, 1.0f, 0.0f), 1.0f, new lambertian(vec3(0.4f, 0.2f, 0.1f)));
	list[i++] = new sphere(vec3(4.0f, 1.0f, 0.0f), 1.0f, new metal(vec3(0.7f, 0.6f, 0.5f), 0.0f));

	return new hitable_list(list, i);
}

int main()
{
	int nx = 1200;
	int ny = 800;
	int ns = 100;
	std::cout << "P3\n" << nx << " " << ny << "\n255\n";
	vec3 lower_left_corner(-2.0f, -1.0f, -1.0f);
	vec3 horizontal(4.0f, 0.0f, 0.0f);
	vec3 vertical(0.0f, 2.0f, 0.0f);
	vec3 origin(0.0f, 0.0f, 0.0f);

	hitable_list* world = random_scene();

	vec3 lookFrom(13.0f, 2.0f, 3.0f);
	vec3 lookAt(0.0f, 0.0f, 0.0f);
	float dist_to_focus = 10.0f;
	float aperture = 0.1f;

	camera cam(lookFrom, lookAt, vec3(0.0f, 1.0f, 0.0f), 20.0f, float(nx)/float(ny), aperture, dist_to_focus);

	for (int j = ny - 1; j >= 0; j--)
	{
		for (int i = 0; i < nx; i++)
		{
			vec3 col(0.0f, 0.0f, 0.0f);
			for (int s = 0; s < ns; s++)
			{
				float u = float(i + rand_float()) / float(nx);
				float v = float(j + rand_float()) / float(ny);
				ray r = cam.get_ray(u, v);
				col += color(r, world, 0);
			}
			col /= float(ns);
			col = vec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));
			int ir = int(255.99 * col.r());
			int ig = int(255.99 * col.g());
			int ib = int(255.99 * col.b());

			std::cout << ir << " " << ig << " " << ib << "\n";
		}
	}

	return 0;
}