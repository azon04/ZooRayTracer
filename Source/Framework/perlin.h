#ifndef _PERLIN_H_
#define _PERLIN_H_

#include "Vec3.h"
#include "rand_helper.h"

inline float trilinear_interp(float c[2][2][2], float u, float v, float w)
{
	float accum = 0.0f;
	for (int i = 0; i < 2; i++)
		for (int j = 0; j < 2; j++)
			for (int k = 0; k < 2; k++)
				accum += (i * u + (1.0f - i) * (1.0f - u)) *
						(j * v + (1.0f - j) * (1.0f - v)) *
						(k * w + (1.0f - k) * (1.0f - w)) * c[i][j][k];
	return accum;
}

inline float perlin_interp(Vec3 c[2][2][2], float u, float v, float w)
{
	float accum = 0.0f;

	// Hermite cubic to remove Mach Bands
	float uu = u * u * (3 - 2 * u);
	float vv = v * v * (3 - 2 * v);
	float ww = w * w * (3 - 2 * w);

	for (int i = 0; i < 2; i++)
		for (int j = 0; j < 2; j++)
			for (int k = 0; k < 2; k++)
			{
				Vec3 weight_v(u - i, v - j, w - k);
				accum += (i * uu + (1.0f - i) * (1.0f - uu)) *
					(j * vv + (1.0f - j) * (1.0f - vv)) *
					(k * ww + (1.0f - k) * (1.0f - ww)) * dot(c[i][j][k], weight_v);
			}
	return accum;
}

class Perlin
{
public:
	float noise(const Vec3& p) const
	{
		float u = p.x() - floor(p.x());
		float v = p.y() - floor(p.y());
		float w = p.z() - floor(p.z());

		int i = floor(p.x());
		int j = floor(p.y());
		int k = floor(p.z());

		Vec3 c[2][2][2];
		for (int di = 0; di < 2; di++)
			for (int dj = 0; dj < 2; dj++)
				for (int dk = 0; dk < 2; dk++)
					c[di][dj][dk] = ranvec[perm_x[(i + di) & 255] ^ perm_y[(j + dj) & 255] ^ perm_z[(k + dk) & 255]];

		return perlin_interp(c, u, v, w);
	}

	float turb(const Vec3& p, int depth = 7) const
	{
		float accum = 0;
		Vec3 temp_p = p;
		float weight = 1.0f;
		for (int i = 0; i < depth; i++)
		{
			accum += weight * noise(temp_p);
			weight *= 0.5f;
			temp_p *= 2.0f;
		}

		return fabs(accum);
	}

	static Vec3 *ranvec;
	static int *perm_x;
	static int *perm_y;
	static int *perm_z;
};

#endif
