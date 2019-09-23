#ifndef _PDF_H_
#define _PDF_H_

#include "math_utils.h"
#include "Hitable.h"
#include "JSONParseable.h"
#include "rand_helper.h"

class PDF : public JSONParseable
{
public:
	virtual float value(const Vec3& direction) const = 0;
	virtual Vec3 generate() const = 0;
	
	virtual void writeToJSON(rapidjson::Value* jsonValue, rapidjson::Document* document);
	virtual void readJSON(rapidjson::Value* jsonValue) {}
};

class CosinePDF : public PDF
{
public:
	CosinePDF() {}
	CosinePDF(const Vec3& w) { uvw.build_from_w(w); }

	void build_from_w(const Vec3& w) { uvw.build_from_w(w); }
	
	virtual float value(const Vec3& direction) const
	{
		float cosine = dot(unit_vector(direction), uvw.w());
		if (cosine > 0)
			return cosine / PI;
		else
			return 0;
	}

	virtual Vec3 generate() const
	{
		return uvw.local(random_cosine_direction());
	}

	virtual void writeToJSON(rapidjson::Value* jsonValue, rapidjson::Document* document);
	virtual void readJSON(rapidjson::Value* jsonValue);

	onb uvw;
};

class HitablePDF : public PDF
{
public:
	HitablePDF() : ptr(nullptr) {}
	HitablePDF(Hitable* p, const Vec3& origin) : ptr(p), o(origin) {}
	virtual float value(const Vec3& direction) const
	{
		return ptr->pdf_value(o, direction);
	}

	virtual Vec3 generate() const
	{
		return ptr->random(o);
	}

	virtual void writeToJSON(rapidjson::Value* jsonValue, rapidjson::Document* document);
	virtual void readJSON(rapidjson::Value* jsonValue);

	Vec3 o;
	Hitable* ptr;
};

class MixturePDF : public PDF
{
public:
	MixturePDF() {}

	MixturePDF(PDF* p0, PDF* p1) { p[0] = p0; p[1] = p1; }
	virtual float value(const Vec3& direction) const
	{
		return 0.5f * p[0]->value(direction) + 0.5f * p[1]->value(direction);
	}

	virtual Vec3 generate() const
	{
		if (rand_float() < 0.5f)
		{
			return p[0]->generate();
		}
		else
		{
			return p[1]->generate();
		}
	}

	virtual void writeToJSON(rapidjson::Value* jsonValue, rapidjson::Document* document);
	virtual void readJSON(rapidjson::Value* jsonValue);

	PDF* p[2];
};
#endif
