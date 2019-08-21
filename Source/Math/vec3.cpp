#include "Vec3.h"

Vec3& Vec3::operator+=(const Vec3& v2)
{
	e[0] += v2[0];
	e[1] += v2[1];
	e[2] += v2[2];

	return *this;
}

Vec3& Vec3::operator-=(const Vec3& v2)
{
	e[0] -= v2[0];
	e[1] -= v2[1];
	e[2] -= v2[2];

	return *this;
}

Vec3& Vec3::operator/=(const float t)
{
	e[0] /= t;
	e[1] /= t;
	e[2] /= t;

	return *this;
}

Vec3& Vec3::operator*=(const float t)
{
	e[0] *= t;
	e[1] *= t;
	e[2] *= t;

	return *this;
}

Vec3& Vec3::operator/=(const Vec3& v2)
{
	e[0] /= v2[0];
	e[1] /= v2[1];
	e[2] /= v2[2];

	return *this;
}

Vec3& Vec3::operator*=(const Vec3& v2)
{
	e[0] *= v2[0];
	e[1] *= v2[1];
	e[2] *= v2[2];

	return *this;
}

Vec3 unit_vector(const Vec3& v)
{
	return v / v.length();
}

Vec3 operator+(const Vec3& v1, const Vec3& v2)
{
	return Vec3(v1[0] + v2[0], v1[1] + v2[1], v1[2] + v2[2]);
}

Vec3 operator-(const Vec3& v1, const Vec3& v2)
{
	return Vec3(v1[0] - v2[0], v1[1] - v2[1], v1[2] - v2[2]);
}

Vec3 operator*(const Vec3& v1, const Vec3& v2)
{
	return Vec3(v1[0] * v2[0], v1[1] * v2[1], v1[2] * v2[2]);
}

Vec3 operator*(const float t, const Vec3& v)
{
	return Vec3(v[0] * t, v[1] * t, v[2] * t);
}

Vec3 operator*(const Vec3& v, const float t)
{
	return Vec3(v[0] * t, v[1] * t, v[2] * t);
}

Vec3 operator/(const Vec3& v1, const Vec3& v2)
{
	return Vec3(v1[0] / v2[0], v1[1] / v2[1], v1[2] / v2[2]);
}

Vec3 operator/(const Vec3& v, const float t)
{
	return Vec3(v[0] / t, v[1] / t, v[2] / t);
}

float dot(const Vec3& v1, const Vec3& v2)
{
	return v1[0] * v2[0] + v1[1] * v2[1] + v1[2] * v2[2];
}

Vec3 cross(const Vec3& v1, const Vec3& v2)
{
	return Vec3((v1[1] * v2[2] - v1[2] * v2[1]),
		-(v1[0] * v2[2] - v1[2] * v2[0]),
		(v1[0] * v2[1] - v1[1] * v2[0]));
}

Vec3 reflect(const Vec3& v, const Vec3& n)
{
	return v - 2 * dot(v, n) * n;
}

bool refract(const Vec3& v, const Vec3& n, float ni_over_nt, Vec3& refracted)
{
	Vec3 uv = unit_vector(v);
	float dt = dot(uv, n);
	float discriminant = 1.0f - ni_over_nt * ni_over_nt*(1 - dt * dt);
	if (discriminant > 0.0f)
	{
		refracted = ni_over_nt * (uv - n * dt) - n * sqrt(discriminant);
		return true;
	}
	else
	{
		return false;
	}
}

void onb::build_from_w(const Vec3& n)
{
	axis[2] = unit_vector(n);
	Vec3 a;
	if (fabs(w().x()) > 0.9)
	{
		a = Vec3(0.0f, 1.0f, 0.0f);
	}
	else
	{
		a = Vec3(1.0f, 0.0f, 0.0f);
	}

	axis[1] = unit_vector(cross(w(), a));
	axis[0] = cross(w(), v());
}
