#include <iostream>
#include <stdlib.h>

#include "ray.h"
#include "hitablelist.h"
#include "sphere.h"
#include "plane.h"
#include "transform_hitable.h"
#include "float.h"
#include "camera.h"
#include "material.h"
#include "rand_helper.h"
#include "bvh.h"
#include "box.h"
#include "pdf.h"
#include "constant_medium.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

vec3 de_nan(const vec3& c)
{
	vec3 temp = c;
	if (!(temp[0] == temp[0])) temp[0] = 0.0f;
	if (!(temp[1] == temp[1])) temp[1] = 0.0f;
	if (!(temp[2] == temp[2])) temp[2] = 0.0f;
	return temp;
}

vec3 color(const ray& r, hitable* world, hitable *light_shape, int depth)
{
	hit_record rec;
	if (world->hit(r, 0.001f, FLT_MAX, rec))
	{
		scatter_record srec;
		vec3 emitted = rec.mat_ptr->emitted(r, rec, rec.u, rec.v, rec.p);
		float pdf_val;
		if (depth < 50 && rec.mat_ptr->scatter(r, rec, srec))
		{
			if (srec.is_specular)
			{
				return srec.attenuation * color(srec.specular_ray, world, light_shape, depth + 1);
			}
			else
			{
				hitable_pdf p0(light_shape, rec.p);
				mixture_pdf p(&p0, srec.pdf_ptr);
				ray scattered = ray(rec.p, p.generate(), r.time());
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
		return vec3(0.0f);
	}
}

hitable* two_perlin_spheres()
{
	texture* pertext = new noise_texture(5.0f);
	hitable** list = new hitable*[2];

	list[0] = new sphere(vec3(0, -1000.0f, 0.0f), 1000.0f, new lambertian(pertext));
	list[1] = new sphere(vec3(0, 2.0f, 0.0f), 2.0f, new lambertian(pertext));

	return new hitable_list(list, 2);
}

hitable* two_spheres()
{
	int n = 5;
	hitable** list = new hitable*[n + 1];
	texture* checker = new checker_texture(new constant_texture(vec3(0.2f, 0.3f, 0.1f)), new constant_texture(vec3(0.9f)));
	
	list[0] = new sphere(vec3(0, -10.0f, 0.0f), 10.0f, new lambertian(checker));
	list[1] = new sphere(vec3(0, 10.0f, 0.0f), 10.0f, new lambertian(checker));

	return new hitable_list(list, 2);
}

hitable* texture_sphere()
{
	int nx, ny, nn;
	unsigned char *tex_data = stbi_load("Resources/earthmap.jpg", &nx, &ny, &nn, 0);
	material *mat = new lambertian(new image_texture(tex_data, nx, ny));

	return new sphere(vec3(0.0f, 0.0f, 0.0f), 2.0f, mat);
}

hitable* random_scene()
{
	int n = 500;
	hitable** list = new hitable*[n + 1];
	texture* checker = new checker_texture(new constant_texture(vec3(0.2f, 0.3f, 0.1f)), new constant_texture(vec3(0.9f)));
	list[0] = new sphere(vec3(0, -1000,0.0f), 1000, new lambertian(checker));

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
					list[i++] = new moving_sphere(center, center + vec3(0, 0.5f * rand_float(), 0), 0.0f, 1.0f, 0.2f, new lambertian(new constant_texture(vec3(rand_float() * rand_float(), rand_float() * rand_float(), rand_float() * rand_float()))));
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
	list[i++] = new sphere(vec3(-4.0f, 1.0f, 0.0f), 1.0f, new lambertian(new constant_texture(vec3(0.4f, 0.2f, 0.1f))));
	list[i++] = new sphere(vec3(4.0f, 1.0f, 0.0f), 1.0f, new metal(vec3(0.7f, 0.6f, 0.5f), 0.0f));

	texture* pertext = new noise_texture(5.0f);
	list[i++] = new sphere(vec3(2.0f, 0.5f, 2.0f), 0.5f, new lambertian(pertext));

	return new bvh_node(list, i, 0.0f, 1.0f);
}

hitable* simple_light()
{
	texture* pertext = new noise_texture(4);
	hitable **list = new hitable *[4];
	list[0] = new sphere(vec3(0.0f, -1000.0f, 0), 1000.0f, new lambertian(pertext));
	list[1] = new sphere(vec3(0.0f, 2.0f, 0.0f), 2.0f, new lambertian(pertext));
	list[2] = new sphere(vec3(0.0f, 7.0f, 0.0f), 2.0f, new diffuse_light(new constant_texture(vec3(4.0f))));
	list[3] = new xy_rect(3.0f, 5.0f, 1.0f, 3.0f, -2.0f, new diffuse_light(new constant_texture(vec3(4.0f))));
	return new hitable_list(list, 4);
}

hitable* cornell_box()
{
	hitable **list = new hitable *[8];
	int i = 0;
	material* red = new lambertian(new constant_texture(vec3(0.65f, 0.05f, 0.05f)));
	material* white = new lambertian(new constant_texture(vec3(0.73f, 0.73f, 0.73f)));
	material* green = new lambertian(new constant_texture(vec3(0.12f, 0.45f, 0.15f)));
	material* light = new diffuse_light(new constant_texture(vec3(15.0f, 15.0f, 15.0f)));

	list[i++] = new flip_normals(new yz_rect(0, 555.0f, 0.0f, 555.0f, 555.0f, green));
	list[i++] = new yz_rect(0, 555.0f, 0.0f, 555.0f, 0.0f, red);
	list[i++] = new flip_normals(new xz_rect(213.0f, 343.0f, 227.0f, 332.0f, 554.0f, light));
	list[i++] = new xz_rect(0.0f, 555.0f, 0.0f, 555.0f, 0.0f, white);
	list[i++] = new flip_normals(new xz_rect(0.0f, 555.0f, 0.0f, 555.0f, 555.0f, white));
	list[i++] = new flip_normals(new xy_rect(0.0f, 555.0f, 0.0f, 555.0f, 555.0f, white));
	material* aluminum = new metal(vec3(0.8f, 0.85f, 0.88f), 0.0f);
	list[i++] = new sphere(vec3(190.0f, 90.0f, 190.0f), 90, aluminum);
	//list[i++] = new translate( new rotate_y( new box(vec3(0.0f), vec3(165.0f, 165.0f, 165.0f), white), -18.0f), vec3(130.0f, 0.0f, 65.0f));
	list[i++] = new translate( new rotate_y( new box(vec3(0.0f), vec3(165.0f, 330.0f, 165.0f), white),  15.0f), vec3(265.0f, 0.0f, 295.0f));

	return new hitable_list(list, i);
}

hitable* cornell_smoke()
{
	hitable **list = new hitable *[8];
	int i = 0;
	material* red = new lambertian(new constant_texture(vec3(0.65f, 0.05f, 0.05f)));
	material* white = new lambertian(new constant_texture(vec3(0.73f, 0.73f, 0.73f)));
	material* green = new lambertian(new constant_texture(vec3(0.12f, 0.45f, 0.15f)));
	material* light = new diffuse_light(new constant_texture(vec3(7.0f, 7.0f, 7.0f)));

	list[i++] = new flip_normals(new yz_rect(0, 555.0f, 0.0f, 555.0f, 555.0f, green));
	list[i++] = new yz_rect(0, 555.0f, 0.0f, 555.0f, 0.0f, red);
	list[i++] = new xz_rect(113.0f, 443.0f, 127.0f, 432.0f, 554.0f, light);
	list[i++] = new xz_rect(0.0f, 555.0f, 0.0f, 555.0f, 0.0f, white);
	list[i++] = new flip_normals(new xz_rect(0.0f, 555.0f, 0.0f, 555.0f, 555.0f, white));
	list[i++] = new flip_normals(new xy_rect(0.0f, 555.0f, 0.0f, 555.0f, 555.0f, white));
	hitable* b1 = new translate(new rotate_y(new box(vec3(0.0f), vec3(165.0f, 165.0f, 165.0f), white), -18.0f), vec3(130.0f, 0.0f, 65.0f));
	hitable* b2 = new translate(new rotate_y(new box(vec3(0.0f), vec3(165.0f, 330.0f, 165.0f), white), 15.0f), vec3(265.0f, 0.0f, 295.0f));
	list[i++] = new constant_medium(b1, 0.01f, new constant_texture(vec3(1.0f)));
	list[i++] = new constant_medium(b2, 0.01f, new constant_texture(vec3(0.0f)));

	return new hitable_list(list, i);
}

hitable* final_chapter2()
{
	int nb = 20;
	hitable** list = new hitable*[30];
	hitable** boxlist = new hitable*[10000];
	hitable** boxlist2 = new hitable*[10000];
	material* white = new lambertian(new constant_texture(vec3(0.73f)));
	material* ground = new lambertian(new constant_texture(vec3(0.48f, 0.83f, 0.53f)));
	int b = 0;
	for (int i = 0; i < nb; i++)
	{
		for (int j = 0; j < nb; j++)
		{
			float w = 100.0f;
			float x0 = -1000 + i * w;
			float z0 = -1000 + j * w;
			float y0 = 0;
			float x1 = x0 + w;
			float y1 = 100 * (rand_float() + 0.001f);
			float z1 = z0 + w;
			boxlist[b++] = new box(vec3(x0, y0, z0), vec3(x1, y1, z1), ground);
		}
	}
	int l = 0;
	list[l++] = new bvh_node(boxlist, b, 0, 1);
	
	material* light = new diffuse_light(new constant_texture(vec3(7.0f)));
	list[l++] = new xz_rect(123.0f, 423.0f, 147.0f, 412.0f, 554.0f, light);
	
	vec3 center(400.0f, 400.0f, 200.0f);
	list[l++] = new moving_sphere(center, center + vec3(30.0f, 0.0f, 0.0f), 0, 1.0f, 50.0f, new lambertian(new constant_texture(vec3(0.77f, 0.3f, 0.1f))));
	list[l++] = new sphere(vec3(260.0f, 150.0f, 45.0f), 50.0f, new dielectric(1.5f));
	list[l++] = new sphere(vec3(0.0f, 150.0f, 145.0f), 50.0f, new metal(vec3(0.8f, 0.8f, 0.9f), 10.0f));

	hitable* boundary = new sphere(vec3(360.0f, 150.0f, 145.0f), 70.0f, new dielectric(1.5f));
	list[l++] = boundary;
	list[l++] = new constant_medium(boundary, 0.2f, new constant_texture(vec3(0.2f, 0.4f, 0.9f)));
	
	boundary = new sphere(vec3(0.0f), 5000, new dielectric(1.5f));
	list[l++] = new constant_medium(boundary, 0.0001f, new constant_texture(vec3(1.0f, 1.0f, 1.0f)));

	int nx, ny, nn;
	unsigned char* textData = stbi_load("Resources/earthmap.jpg", &nx, &ny, &nn, 0);
	material* emat = new lambertian(new image_texture(textData, nx, ny));
	list[l++] = new sphere(vec3(400.0f, 200.0f, 400.0f), 100.0f, emat);
	texture* pertext = new noise_texture(0.1f);
	list[l++] = new sphere(vec3(220.0f, 280.0f, 300.0f), 80.0f, new lambertian(pertext));
	
	int ns = 1000;
	
	for (int j = 0; j < ns; j++)
	{
		boxlist2[j] = new sphere(vec3(165.0f * rand_float(), 165 * rand_float(), 165 * rand_float()), 10.0f, white);
	}
	list[l++] = new translate(new rotate_y(new bvh_node(boxlist2, ns, 0.0f, 1.0f), 15.0f), vec3(-100.0f, 270.0f, 395.0f));

	return new hitable_list(list, l);
}

int main()
{
	int nx = 500;
	int ny = 500;
	int ns = 1000;
	std::cout << "P3\n" << nx << " " << ny << "\n255\n";

	//hitable* world = two_perlin_spheres();
	//hitable* world = simple_light();
	hitable* world = cornell_box();
	//hitable* world = cornell_smoke();
	//hitable* world = final_chapter2();
	//hitable* world = texture_sphere();
	//hitable* world = random_scene();

	hitable* light_shape = new xz_rect(213.0f, 343.0f, 227.0f, 332.0f, 554.0f, nullptr);
	hitable* glass_sphere = new sphere(vec3(190.0f, 90.0f, 190.0f), 90, nullptr);
	hitable* a[2];
	a[0] = light_shape;
	a[1] = glass_sphere;
	hitable_list hlist(a, 2);

	//vec3 lookFrom(13.0f, 1.0f, 3.0f);
	vec3 lookFrom(278.0f, 278.0f, -800.0f);
	//vec3 lookAt(0.0f, 0.0f, 0.0f);
	vec3 lookAt(278.0f, 278.0f, 0.0f);
	float dist_to_focus = 10.0f;
	float aperture = 0.0f;
	float vfov = 40.0f;

	camera cam(lookFrom, lookAt, vec3(0.0f, 1.0f, 0.0f), vfov, float(nx)/float(ny), aperture, dist_to_focus, 0.0f, 1.0f);

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
				col += de_nan(color(r, world, &hlist, 0));
			}
			col /= float(ns);
			col = vec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));
			//col = col / (col + vec3(1.0f));
			int ir = int(255.99 * col.r());
			int ig = int(255.99 * col.g());
			int ib = int(255.99 * col.b());

			std::cout << ir << " " << ig << " " << ib << "\n";
		}
	}

	return 0;
}