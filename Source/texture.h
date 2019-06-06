#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include "vec3.h"
#include "perlin.h"

class texture
{
public:
	virtual vec3 value(float u, float v, const vec3& p) const = 0;
};

class constant_texture : public texture
{
public:
	constant_texture() {}
	constant_texture(const vec3& c) : color(c) {}

	virtual vec3 value(float u, float v, const vec3& p) const
	{
		return color;
	}

	vec3 color;
};

class checker_texture : public texture
{
public:
	checker_texture() {}
	checker_texture(texture* t0, texture* t1) : odd(t0), even(t1) {}

	virtual vec3 value(float u, float v, const vec3& p) const
	{
		float sines = sin(10 * p.x()) * sin(10 * p.y()) * sin(10 * p.z());
		if (sines < 0.0f)
		{
			return odd->value(u, v, p);
		}
		else
		{
			return even->value(u, v, p);
		}
	}

	texture* odd;
	texture* even;
};

class noise_texture : public texture 
{
public:
	noise_texture() {}
	noise_texture(float sc) : scale(sc) {}
	virtual vec3 value(float u, float v, const vec3& p) const
	{
		//return vec3(1.0f) * 0.5f *  (1 + noise.noise(scale * p));
		//return vec3(1.0f) * noise.turb(scale * p);
		return vec3(1.0f) * 0.5f * (1 + sin(scale * p.z() + 10 * noise.turb(p)));
	}
	perlin noise;
	float scale;
};

class image_texture : public texture
{
public:
	image_texture() {}
	image_texture(unsigned char* pixel, int width, int height) : data(pixel), nx(width), ny(height) {}

	virtual vec3 value(float u, float v, const vec3& p) const;

	unsigned char* data;
	int nx, ny;
};

vec3 image_texture::value(float u, float v, const vec3& p) const
{
	int i = u * nx;
	int j = (1-v) * ny - 0.001;
	if (i < 0) i = 0;
	if (j < 0) j = 0;
	if (i > nx - 1) i = nx - 1;
	if (j > ny - 1) j = ny - 1;

	float r = int(data[3 * i + 3 * nx * j]) / 255.0f;
	float g = int(data[3 * i + 3 * nx * j + 1]) / 255.0f;
	float b = int(data[3 * i + 3 * nx * j + 2]) / 255.0f;

	return vec3(r, g, b);
}

#endif
