#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include "Vec3.h"
#include "Perlin.h"
#include "JSONParseable.h"
#include "JSONHelper.h"
#include <sstream>

class Texture : public JSONParseable
{
public:
	virtual Vec3 value(float u, float v, const Vec3& p) const = 0;

	virtual void writeToJSON(rapidjson::Value* jsonValue, rapidjson::Document* document)
	{
		jsonValue->SetObject();
		rapidjson::Value::Object jsonObject = jsonValue->GetObject();
		jsonObject.AddMember("Type", "Texture", document->GetAllocator());
	}

	virtual void readJSON(rapidjson::Value* jsonValue) {}
};

class ConstantColorTexture : public Texture
{
public:
	ConstantColorTexture() {}
	ConstantColorTexture(const Vec3& c) : color(c) {}

	virtual Vec3 value(float u, float v, const Vec3& p) const
	{
		return color;
	}

	virtual void writeToJSON(rapidjson::Value* jsonValue, rapidjson::Document* document);
	virtual void readJSON(rapidjson::Value* jsonValue);

	Vec3 color;
};

class CheckerTexture : public Texture
{
public:
	CheckerTexture() {}
	CheckerTexture(Texture* t0, Texture* t1) : odd(t0), even(t1) {}

	virtual Vec3 value(float u, float v, const Vec3& p) const
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

	virtual void writeToJSON(rapidjson::Value* jsonValue, rapidjson::Document* document);
	virtual void readJSON(rapidjson::Value* jsonValue);

	Texture* odd;
	Texture* even;
};

class NoiseTexture : public Texture 
{
public:
	NoiseTexture() {}
	NoiseTexture(float sc) : scale(sc) {}
	virtual Vec3 value(float u, float v, const Vec3& p) const
	{
		//return vec3(1.0f) * 0.5f *  (1 + noise.noise(scale * p));
		//return vec3(1.0f) * noise.turb(scale * p);
		return Vec3(1.0f) * 0.5f * (1 + sin(scale * p.z() + 10 * noise.turb(p)));
	}

	virtual void writeToJSON(rapidjson::Value* jsonValue, rapidjson::Document* document);
	virtual void readJSON(rapidjson::Value* jsonValue);

	Perlin noise;
	float scale;
};

class ImageTexture : public Texture
{
public:
	ImageTexture() {}
	ImageTexture(unsigned char* pixel, int width, int height, const char* imagePath) : data(pixel), nx(width), ny(height) 
	{
		std::stringstream stringStream;
		stringStream << imagePath;
		stringStream >> this->imagePath;
	}

	virtual Vec3 value(float u, float v, const Vec3& p) const
	{
		int i = int(u * nx);
		int j = int((1 - v) * ny - 0.001);
		if (i < 0) i = 0;
		if (j < 0) j = 0;
		if (i > nx - 1) i = nx - 1;
		if (j > ny - 1) j = ny - 1;

		float r = int(data[3 * i + 3 * nx * j]) / 255.0f;
		float g = int(data[3 * i + 3 * nx * j + 1]) / 255.0f;
		float b = int(data[3 * i + 3 * nx * j + 2]) / 255.0f;

		return Vec3(r, g, b);
	}

	virtual void writeToJSON(rapidjson::Value* jsonValue, rapidjson::Document* document);
	virtual void readJSON(rapidjson::Value* jsonValue);

	unsigned char* data;
	char imagePath[50];
	int nx, ny;
};

#endif
