#include "Perlin.h"

static Vec3* perlin_generate()
{
	Vec3* p = new Vec3[256];
	for (int i = 0; i < 256; i++)
	{
		p[i] = unit_vector(Vec3(-1.0f + 2.0f * rand_float(), -1.0f + 2.0f * rand_float(), -1.0f + 2.0f * rand_float()));
	}
	return p;
}

void permute(int* p, int n)
{
	for (int i = n - 1; i > 0; i--)
	{
		int target = int(rand_float() * (i + 1));
		int tmp = p[i];
		p[i] = p[target];
		p[target] = tmp;
	}
}

static int* perlin_generate_perm()
{
	int* p = new int[256];
	for (int i = 0; i < 256; i++)
	{
		p[i] = i;
	}
	permute(p, 256);
	return p;
}

Vec3 * Perlin::ranvec = perlin_generate();
int * Perlin::perm_x = perlin_generate_perm();
int * Perlin::perm_y = perlin_generate_perm();
int * Perlin::perm_z = perlin_generate_perm();