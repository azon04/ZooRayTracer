#ifndef _PDF_H_
#define _PDF_H_

#include "math_utils.h"
#include "Hitable.h"

class PDF 
{
public:
	virtual float value(const Vec3& direction) const = 0;
	virtual Vec3 generate() const = 0;
};

class cosine_pdf : public PDF
{
public:
	cosine_pdf(const Vec3& w) { uvw.build_from_w(w); }
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

	onb uvw;
};

class Hitable_pdf : public PDF
{
public:
	Hitable_pdf(Hitable* p, const Vec3& origin) : ptr(p), o(origin) {}
	virtual float value(const Vec3& direction) const
	{
		return ptr->pdf_value(o, direction);
	}

	virtual Vec3 generate() const
	{
		return ptr->random(o);
	}

	Vec3 o;
	Hitable* ptr;
};

class Mixture_pdf : public PDF
{
public:
	Mixture_pdf(PDF* p0, PDF* p1) { p[0] = p0; p[1] = p1; }
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
	PDF* p[2];
};
#endif
