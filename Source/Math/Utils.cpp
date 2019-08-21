#include "math_utils.h"
#include "rand_helper.h"

float rand_float()
{
	return drand48();
}

Vec3 random_cosine_direction()
{
	float r1 = rand_float();
	float r2 = rand_float();
	float z = sqrt(1 - r2);
	float phi = 2 * PI * r1;
	float x = cos(phi) * 2 * sqrt(r2);
	float y = sin(phi) * 2 * sqrt(r2);
	return Vec3(x, y, z);
}

Vec3 random_in_unit_disk()
{
	Vec3 p;
	do
	{
		p = 2.0f * Vec3(rand_float(), rand_float(), 0) - Vec3(1.0f, 1.0f, 0.0f);
	} while (dot(p, p) >= 1.0f);
	return p;
}

Vec3 random_in_unit_sphere()
{
	Vec3 p;
	do
	{
		p = 2.0f * Vec3(rand_float(), rand_float(), rand_float()) - Vec3(1.0f, 1.0f, 1.0f);
	} while (p.squared_length() >= 1.0f);

	return p;
}