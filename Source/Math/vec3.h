#ifndef _VEC3_H_
#define _VEC3_H_

#include <math.h>
#include <stdlib.h>
#include <iostream>

class Vec3 
{
public:
	Vec3() {}
	Vec3(float e0)
	{
		e[0] = e[1] = e[2] = e0;
	}

	Vec3(float e0, float e1, float e2) 
	{
		e[0] = e0;
		e[1] = e1;
		e[2] = e2;
	}

	inline float& x() { return e[0]; }
	inline float& y() { return e[1]; }
	inline float& z() { return e[2]; }

	inline float& r() { return e[0]; }
	inline float& g() { return e[1]; }
	inline float& b() { return e[2]; }

	inline float x() const { return e[0]; }
	inline float y() const { return e[1]; }
	inline float z() const { return e[2]; }

	inline float r() const { return e[0]; }
	inline float g() const { return e[1]; }
	inline float b() const { return e[2]; }

	inline const Vec3& operator+() const { return *this; }
	inline Vec3 operator-() const { return Vec3(-e[0], -e[1], -e[2]); }
	inline float& operator[](int i) { return e[i]; }
	inline float operator[](int i) const { return e[i]; }
	
	Vec3& operator+=(const Vec3& v2);
	Vec3& operator-=(const Vec3& v2);
	Vec3& operator*=(const Vec3& v2);
	Vec3& operator/=(const Vec3& v2);
	Vec3& operator*=(const float t);
	Vec3& operator/=(const float t);

	inline float length() const 
	{
		return sqrt(e[0] * e[0] + e[1] * e[1] + e[2] * e[2]);
	}

	inline float squared_length() const
	{
		return e[0] * e[0] + e[1] * e[1] + e[2] * e[2];
	}

	inline void make_unit_vector() 
	{
		*this /= length();
	}

	float e[3];
};

Vec3 unit_vector(const Vec3& v);
Vec3 operator+(const Vec3& v1, const Vec3& v2);
Vec3 operator-(const Vec3& v1, const Vec3& v2);
Vec3 operator*(const Vec3& v1, const Vec3& v2);
Vec3 operator/(const Vec3& v1, const Vec3& v2);
Vec3 operator*(const float t, const Vec3& v);
Vec3 operator*(const Vec3& v, const float t);
Vec3 operator/(const Vec3& v, const float t);
float dot(const Vec3& v1, const Vec3& v2);
Vec3 cross(const Vec3& v1, const Vec3& v2);

Vec3 reflect(const Vec3& v, const Vec3& n);
bool refract(const Vec3& v, const Vec3& n, float ni_over_nt, Vec3& refracted);

inline std::istream& operator>>(std::istream& is, Vec3& t)
{
	is >> t[0] >> t[1] >> t[2];
	return is;
}

inline std::ostream& operator <<(std::ostream& os, Vec3& t)
{
	os << t[0] << " " << t[1] << " " << t[2];
	return os;
}

class onb
{
public:
	onb() {}
	inline Vec3 operator[](int i) const { return axis[i]; }
	Vec3 u() const { return axis[0]; }
	Vec3 v() const { return axis[1]; }
	Vec3 w() const { return axis[2]; }
	Vec3 local(float a, float b, float c) const { return a * u() + b * v() + c * w(); }
	Vec3 local(const Vec3& a) const { return a.x() * u() + a.y() * v() + a.z() * w(); }
	void build_from_w(const Vec3& n);
	Vec3 axis[3];
};


#endif