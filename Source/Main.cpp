#include <iostream>
#include <stdlib.h>

#include "Ray.h"
#include "HitableList.h"
#include "Sphere.h"
#include "PlaneHitable.h"
#include "TransformHitable.h"
#include "float.h"
#include "Camera.h"
#include "Material.h"
#include "rand_helper.h"
#include "BVH.h"
#include "Box.h"
#include "PDF.h"
#include "ConstantMedium.h"
#include "Mesh.h"
#include "Renderer.h"

#include "stb_image.h"

#include "FileIO/MeshReader.h"
#include "FileIO/JSONParser.h"

WorldInfo two_perlin_spheres()
{
	WorldInfo worldInfo;

	//vec3 lookFrom(13.0f, 1.0f, 3.0f);
	Vec3 lookFrom(278.0f, 278.0f, -800.0f);
	//vec3 lookAt(0.0f, 0.0f, 0.0f);
	Vec3 lookAt(278.0f, 278.0f, 0.0f);
	float dist_to_focus = 10.0f;
	float aperture = 0.0f;
	float vfov = 40.0f;

	worldInfo.camera = Camera(lookFrom, lookAt, Vec3(0.0f, 1.0f, 0.0f), vfov, float(500) / float(500), aperture, dist_to_focus, 0.0f, 1.0f);
	worldInfo.renderSettings.sampleCount = 250;
	worldInfo.renderSettings.width = 500;
	worldInfo.renderSettings.height = 500;
	worldInfo.renderSettings.rayDepth = 25;

	Texture* pertext = new NoiseTexture(5.0f);
	Hitable** list = new Hitable*[2];

	list[0] = new Sphere(Vec3(0, -1000.0f, 0.0f), 1000.0f, new Lambertian(pertext));
	list[1] = new Sphere(Vec3(0, 2.0f, 0.0f), 2.0f, new Lambertian(pertext));

	worldInfo.world = new HitableList(list, 2);
	worldInfo.lightList = nullptr;

	return worldInfo;
}

WorldInfo two_spheres()
{
	WorldInfo worldInfo;

	//vec3 lookFrom(13.0f, 1.0f, 3.0f);
	Vec3 lookFrom(278.0f, 278.0f, -800.0f);
	//vec3 lookAt(0.0f, 0.0f, 0.0f);
	Vec3 lookAt(278.0f, 278.0f, 0.0f);
	float dist_to_focus = 10.0f;
	float aperture = 0.0f;
	float vfov = 40.0f;

	worldInfo.camera = Camera(lookFrom, lookAt, Vec3(0.0f, 1.0f, 0.0f), vfov, float(500) / float(500), aperture, dist_to_focus, 0.0f, 1.0f);
	worldInfo.renderSettings.sampleCount = 250;
	worldInfo.renderSettings.width = 500;
	worldInfo.renderSettings.height = 500;
	worldInfo.renderSettings.rayDepth = 25;

	int n = 5;
	Hitable** list = new Hitable*[n + 1];
	Texture* checker = new CheckerTexture(new ConstantColorTexture(Vec3(0.2f, 0.3f, 0.1f)), new ConstantColorTexture(Vec3(0.9f)));
	
	list[0] = new Sphere(Vec3(0, -10.0f, 0.0f), 10.0f, new Lambertian(checker));
	list[1] = new Sphere(Vec3(0, 10.0f, 0.0f), 10.0f, new Lambertian(checker));

	worldInfo.world = new HitableList(list, 2);
	worldInfo.lightList = nullptr;

	return worldInfo;
}

WorldInfo texture_sphere()
{
	WorldInfo worldInfo;

	//vec3 lookFrom(13.0f, 1.0f, 3.0f);
	Vec3 lookFrom(278.0f, 278.0f, -800.0f);
	//vec3 lookAt(0.0f, 0.0f, 0.0f);
	Vec3 lookAt(278.0f, 278.0f, 0.0f);
	float dist_to_focus = 10.0f;
	float aperture = 0.0f;
	float vfov = 40.0f;

	worldInfo.camera = Camera(lookFrom, lookAt, Vec3(0.0f, 1.0f, 0.0f), vfov, float(500) / float(500), aperture, dist_to_focus, 0.0f, 1.0f);
	worldInfo.renderSettings.sampleCount = 250;
	worldInfo.renderSettings.width = 500;
	worldInfo.renderSettings.height = 500;
	worldInfo.renderSettings.rayDepth = 25;

	int nx, ny, nn;
	unsigned char *tex_data = stbi_load("Resources/earthmap.jpg", &nx, &ny, &nn, 0);
	Material *mat = new Lambertian(new ImageTexture(tex_data, nx, ny, "Resources/earthmap.jpg"));

	worldInfo.world = new Sphere(Vec3(0.0f, 0.0f, 0.0f), 2.0f, mat);
	worldInfo.lightList = nullptr;

	return worldInfo;
}

WorldInfo random_scene()
{
	WorldInfo worldInfo;

	Vec3 lookFrom(13.0f, 2.0f, 3.0f);
	Vec3 lookAt(0.0f, 0.0f, 0.0f);
	float dist_to_focus = 10.0f;
	float aperture = 0.1f;
	float vfov = 20.0f;

	worldInfo.camera = Camera(lookFrom, lookAt, Vec3(0.0f, 1.0f, 0.0f), vfov, float(500) / float(500), aperture, dist_to_focus, 0.0f, 1.0f);
	worldInfo.renderSettings.sampleCount = 10;
	worldInfo.renderSettings.width = 1200;
	worldInfo.renderSettings.height = 800;
	worldInfo.renderSettings.rayDepth = 25;

	int n = 500;
	Hitable** list = new Hitable*[n + 1];
	Texture* checker = new CheckerTexture(new ConstantColorTexture(Vec3(0.2f, 0.3f, 0.1f)), new ConstantColorTexture(Vec3(0.9f)));
	list[0] = new Sphere(Vec3(0, -1000,0.0f), 1000, new Lambertian(checker));

	int i = 1;
	for (int a = -11; a < 11; a++)
	{
		for (int b = -11; b < 11; b++)
		{
			float choose_mat = rand_float();
			Vec3 center(a + 0.9 * rand_float(), 0.2f, b + 9 * rand_float());
			if ((center - Vec3(4.0f, 0.2f, 0)).length() > 0.9)
			{
				if (choose_mat < 0.8f) // diffuse
				{
					list[i++] = new MovingSphere(center, center + Vec3(0, 0.5f * rand_float(), 0), 0.0f, 1.0f, 0.2f, new Lambertian(new ConstantColorTexture(Vec3(rand_float() * rand_float(), rand_float() * rand_float(), rand_float() * rand_float()))));
				}
				else if (choose_mat < 0.95f) // metal
				{
					list[i++] = new Sphere(center, 0.2f, new Metal(Vec3(0.5f * (1 - rand_float()), 0.5f * (1 - rand_float()), 0.5f * (1 - rand_float())), 0.5f * rand_float()));
				}
				else // glass
				{
					list[i++] = new Sphere(center, 0.2f, new Dielectric(1.5f));
				}
			}
		}
	}

	list[i++] = new Sphere(Vec3(0.0f, 1.0f, 0.0f), 1.0f, new Dielectric(1.5f));
	list[i++] = new Sphere(Vec3(-4.0f, 1.0f, 0.0f), 1.0f, new Lambertian(new ConstantColorTexture(Vec3(0.4f, 0.2f, 0.1f))));
	list[i++] = new Sphere(Vec3(4.0f, 1.0f, 0.0f), 1.0f, new Metal(Vec3(0.7f, 0.6f, 0.5f), 0.0f));

	Texture* pertext = new NoiseTexture(5.0f);
	list[i++] = new Sphere(Vec3(2.0f, 0.5f, 2.0f), 0.5f, new Lambertian(pertext));

	worldInfo.world = new BVHNode(list, i, 0.0f, 1.0f);
	worldInfo.lightList = nullptr;

	return worldInfo;
}

WorldInfo simple_light()
{
	WorldInfo worldInfo;

	//vec3 lookFrom(13.0f, 1.0f, 3.0f);
	Vec3 lookFrom(278.0f, 278.0f, -800.0f);
	//vec3 lookAt(0.0f, 0.0f, 0.0f);
	Vec3 lookAt(278.0f, 278.0f, 0.0f);
	float dist_to_focus = 10.0f;
	float aperture = 0.0f;
	float vfov = 40.0f;

	worldInfo.camera = Camera(lookFrom, lookAt, Vec3(0.0f, 1.0f, 0.0f), vfov, float(500) / float(500), aperture, dist_to_focus, 0.0f, 1.0f);
	worldInfo.renderSettings.sampleCount = 250;
	worldInfo.renderSettings.width = 500;
	worldInfo.renderSettings.height = 500;
	worldInfo.renderSettings.rayDepth = 25;

	Texture* pertext = new NoiseTexture(4);
	Hitable **list = new Hitable *[4];
	list[0] = new Sphere(Vec3(0.0f, -1000.0f, 0), 1000.0f, new Lambertian(pertext));
	list[1] = new Sphere(Vec3(0.0f, 2.0f, 0.0f), 2.0f, new Lambertian(pertext));
	list[2] = new Sphere(Vec3(0.0f, 7.0f, 0.0f), 2.0f, new DiffuseLight(new ConstantColorTexture(Vec3(4.0f))));
	list[3] = new XYRect(3.0f, 5.0f, 1.0f, 3.0f, -2.0f, new DiffuseLight(new ConstantColorTexture(Vec3(4.0f))));
	worldInfo.world = new HitableList(list, 4);
	worldInfo.lightList = nullptr;

	return worldInfo;
}

WorldInfo cornell_box()
{
	WorldInfo worldInfo;

	//vec3 lookFrom(13.0f, 1.0f, 3.0f);
	Vec3 lookFrom(278.0f, 278.0f, -800.0f);
	//vec3 lookAt(0.0f, 0.0f, 0.0f);
	Vec3 lookAt(278.0f, 278.0f, 0.0f);
	float dist_to_focus = 10.0f;
	float aperture = 0.0f;
	float vfov = 40.0f;

	worldInfo.camera = Camera(lookFrom, lookAt, Vec3(0.0f, 1.0f, 0.0f), vfov, float(500) / float(500), aperture, dist_to_focus, 0.0f, 1.0f);
	worldInfo.renderSettings.sampleCount = 250;
	worldInfo.renderSettings.width = 500;
	worldInfo.renderSettings.height = 500;
	worldInfo.renderSettings.rayDepth = 25;

	Hitable **list = new Hitable *[8];
	int i = 0;
	Material* red = new Lambertian(new ConstantColorTexture(Vec3(0.65f, 0.05f, 0.05f)));
	Material* white = new Lambertian(new ConstantColorTexture(Vec3(0.73f, 0.73f, 0.73f)));
	Material* green = new Lambertian(new ConstantColorTexture(Vec3(0.12f, 0.45f, 0.15f)));
	Material* light = new DiffuseLight(new ConstantColorTexture(Vec3(15.0f, 15.0f, 15.0f)));

  	list[i++] = new FlipNormals(new YZRect(0, 555.0f, 0.0f, 555.0f, 555.0f, green));
  	list[i++] = new YZRect(0, 555.0f, 0.0f, 555.0f, 0.0f, red);
 	Hitable* light_plane = new FlipNormals(new XZRect(213.0f, 343.0f, 227.0f, 332.0f, 554.0f, light));
 	list[i++] = light_plane;
   	list[i++] = new XZRect(0.0f, 555.0f, 0.0f, 555.0f, 0.0f, white);
   	list[i++] = new FlipNormals(new XZRect(0.0f, 555.0f, 0.0f, 555.0f, 555.0f, white));
  	list[i++] = new FlipNormals(new XYRect(0.0f, 555.0f, 0.0f, 555.0f, 555.0f, white));
	//list[i++] = new triangle(vec3(0.0f, 0.0f, 555.0f), vec3(0.0f, 555.0f, 755.0f), vec3(555.0f, 555.0f, 555.0f), white);
	Material* aluminum = new Metal(Vec3(0.8f, 0.85f, 0.88f), 0.0f);
	Material* glass = new Dielectric(1.5f);
	MeshReader reader;
	Mesh* _mesh = reader.parse("Input/bunny_200.obj", 750.0f);
	_mesh->mat = white;
	Hitable* _meshHitable = new TranslateHitable(new RotateY(_mesh, 180.0f), Vec3(275.0f, 0.0f, 290.0f));
	list[i++] = _meshHitable;
	list[i++] = new Sphere(Vec3(190.0f, 90.0f, 190.0f), 90, glass);
	//list[i++] = new translate( new rotate_y( new box(vec3(0.0f), vec3(165.0f, 165.0f, 165.0f), white), -18.0f), vec3(130.0f, 0.0f, 65.0f));
	//list[i++] = new translate( new rotate_y( new box(vec3(0.0f), vec3(165.0f, 330.0f, 165.0f), white),  15.0f), vec3(265.0f, 0.0f, 295.0f));

	Hitable** a = new Hitable*[2];
	a[0] = new XZRect(213.0f, 343.0f, 227.0f, 332.0f, 554.0f, nullptr);
	a[1] = new Sphere(Vec3(190.0f, 90.0f, 190.0f), 90, nullptr);

	worldInfo.world = new HitableList(list, i);
	worldInfo.lightList = new HitableList(a, 2);

	return worldInfo;
}

WorldInfo cornell_smoke()
{
	WorldInfo worldInfo;

	Vec3 lookFrom(278.0f, 278.0f, -800.0f);
	Vec3 lookAt(278.0f, 278.0f, 0.0f);
	float dist_to_focus = 10.0f;
	float aperture = 0.0f;
	float vfov = 40.0f;

	worldInfo.camera = Camera(lookFrom, lookAt, Vec3(0.0f, 1.0f, 0.0f), vfov, float(500) / float(500), aperture, dist_to_focus, 0.0f, 1.0f);
	worldInfo.renderSettings.sampleCount = 250;
	worldInfo.renderSettings.width = 500;
	worldInfo.renderSettings.height = 500;
	worldInfo.renderSettings.rayDepth = 25;

	Hitable **list = new Hitable *[8];
	int i = 0;
	Material* red = new Lambertian(new ConstantColorTexture(Vec3(0.65f, 0.05f, 0.05f)));
	Material* white = new Lambertian(new ConstantColorTexture(Vec3(0.73f, 0.73f, 0.73f)));
	Material* green = new Lambertian(new ConstantColorTexture(Vec3(0.12f, 0.45f, 0.15f)));
	Material* light = new DiffuseLight(new ConstantColorTexture(Vec3(7.0f, 7.0f, 7.0f)));

	list[i++] = new FlipNormals(new YZRect(0, 555.0f, 0.0f, 555.0f, 555.0f, green));
	list[i++] = new YZRect(0, 555.0f, 0.0f, 555.0f, 0.0f, red);
	list[i++] = new XZRect(113.0f, 443.0f, 127.0f, 432.0f, 554.0f, light);
	list[i++] = new XZRect(0.0f, 555.0f, 0.0f, 555.0f, 0.0f, white);
	list[i++] = new FlipNormals(new XZRect(0.0f, 555.0f, 0.0f, 555.0f, 555.0f, white));
	list[i++] = new FlipNormals(new XYRect(0.0f, 555.0f, 0.0f, 555.0f, 555.0f, white));
	Hitable* b1 = new TranslateHitable(new RotateY(new Box(Vec3(0.0f), Vec3(165.0f, 165.0f, 165.0f), white), -18.0f), Vec3(130.0f, 0.0f, 65.0f));
	Hitable* b2 = new TranslateHitable(new RotateY(new Box(Vec3(0.0f), Vec3(165.0f, 330.0f, 165.0f), white), 15.0f), Vec3(265.0f, 0.0f, 295.0f));
	list[i++] = new ConstantMedium(b1, 0.01f, new ConstantColorTexture(Vec3(1.0f)));
	list[i++] = new ConstantMedium(b2, 0.01f, new ConstantColorTexture(Vec3(0.0f)));

	worldInfo.world = new HitableList(list, i);
	worldInfo.lightList = nullptr;

	return worldInfo;
}

WorldInfo final_chapter2()
{
	WorldInfo worldInfo;

	Vec3 lookFrom(200.0f, 278.0f, -600.0f);
	Vec3 lookAt(278.0f, 278.0f, 0.0f);
	float dist_to_focus = 10.0f;
	float aperture = 0.0f;
	float vfov = 40.0f;

	worldInfo.camera = Camera(lookFrom, lookAt, Vec3(0.0f, 1.0f, 0.0f), vfov, float(500) / float(500), aperture, dist_to_focus, 0.0f, 1.0f);
	worldInfo.renderSettings.sampleCount = 250;
	worldInfo.renderSettings.width = 500;
	worldInfo.renderSettings.height = 500;
	worldInfo.renderSettings.rayDepth = 25;

	int nb = 20;
	Hitable** list = new Hitable*[30];
	Hitable** boxlist = new Hitable*[10000];
	Hitable** boxlist2 = new Hitable*[10000];
	Material* white = new Lambertian(new ConstantColorTexture(Vec3(0.73f)));
	Material* ground = new Lambertian(new ConstantColorTexture(Vec3(0.48f, 0.83f, 0.53f)));
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
			boxlist[b++] = new Box(Vec3(x0, y0, z0), Vec3(x1, y1, z1), ground);
		}
	}
	int l = 0;
	list[l++] = new BVHNode(boxlist, b, 0, 1);
	
	Material* light = new DiffuseLight(new ConstantColorTexture(Vec3(7.0f)));
	list[l++] = new FlipNormals(new XZRect(123.0f, 423.0f, 147.0f, 412.0f, 554.0f, light));
	
	Vec3 center(400.0f, 400.0f, 200.0f);
	list[l++] = new MovingSphere(center, center + Vec3(30.0f, 0.0f, 0.0f), 0, 1.0f, 50.0f, new Lambertian(new ConstantColorTexture(Vec3(0.77f, 0.3f, 0.1f))));
	list[l++] = new Sphere(Vec3(260.0f, 150.0f, 45.0f), 50.0f, new Dielectric(1.5f));
	list[l++] = new Sphere(Vec3(0.0f, 150.0f, 145.0f), 50.0f, new Metal(Vec3(0.8f, 0.8f, 0.9f), 10.0f));

	Hitable* boundary = new Sphere(Vec3(360.0f, 150.0f, 145.0f), 70.0f, new Dielectric(1.5f));
	list[l++] = boundary;
	list[l++] = new ConstantMedium(boundary, 0.2f, new ConstantColorTexture(Vec3(0.2f, 0.4f, 0.9f)));
	
	boundary = new Sphere(Vec3(0.0f), 5000, new Dielectric(1.5f));
	list[l++] = new ConstantMedium(boundary, 0.0001f, new ConstantColorTexture(Vec3(1.0f, 1.0f, 1.0f)));

	int nx, ny, nn;
	unsigned char* textData = stbi_load("Resources/earthmap.jpg", &nx, &ny, &nn, 0);
	Material* emat = new Lambertian(new ImageTexture(textData, nx, ny, "Resources/earthmap.jpg"));
	list[l++] = new Sphere(Vec3(400.0f, 200.0f, 400.0f), 100.0f, emat);
	Texture* pertext = new NoiseTexture(0.1f);
	list[l++] = new Sphere(Vec3(220.0f, 280.0f, 300.0f), 80.0f, new Lambertian(pertext));
	
	int ns = 1000;
	
	for (int j = 0; j < ns; j++)
	{
		boxlist2[j] = new Sphere(Vec3(165.0f * rand_float(), 165 * rand_float(), 165 * rand_float()), 10.0f, white);
	}
	list[l++] = new TranslateHitable(new RotateY(new BVHNode(boxlist2, ns, 0.0f, 1.0f), 15.0f), Vec3(-100.0f, 270.0f, 395.0f));

	worldInfo.world = new HitableList(list, l);
	worldInfo.lightList = nullptr; // new FlipNormals(new XZRect(123.0f, 423.0f, 147.0f, 412.0f, 554.0f, light));

	return worldInfo;
}

int main()
{
#if 0
	JSONParser jsonWriter;
	jsonWriter.writeWorld("Scene/two_perlin_spheres.json", two_perlin_spheres());
	jsonWriter.writeWorld("Scene/two_spheres.json", two_spheres());
	jsonWriter.writeWorld("Scene/random_scene.json", random_scene());
	jsonWriter.writeWorld("Scene/texture_sphere.json", texture_sphere());
	jsonWriter.writeWorld("Scene/simple_light.json", simple_light());
	jsonWriter.writeWorld("Scene/cornell_box.json", cornell_box());
	jsonWriter.writeWorld("Scene/cornell_smoke.json", cornell_smoke());
	jsonWriter.writeWorld("Scene/final_chapter2.json", final_chapter2());
#else
	WorldInfo worldInfo = final_chapter2();

	//JSONParser parser;
	//parser.readWorld("Scene/final_chapter2.json", worldInfo);

	Renderer renderer;
	renderer.sample_count = worldInfo.renderSettings.sampleCount;
	renderer.width = worldInfo.renderSettings.width;
	renderer.height = worldInfo.renderSettings.height;
	renderer.ray_depth = worldInfo.renderSettings.rayDepth;
	renderer.render(worldInfo.camera, worldInfo.world, worldInfo.lightList);
#endif

	return 0;
}