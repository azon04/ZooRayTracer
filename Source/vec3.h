#ifndef _VEC3_H_
#define _VEC3_H_

#include <math.h>
#include <stdlib.h>
#include <iostream>

class vec3 
{
public:
	vec3() {}
	vec3(float e0, float e1, float e2) 
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

	inline const vec3& operator+() const { return *this; }
	inline vec3 operator-() const { return vec3(-e[0], -e[1], -e[2]); }
	inline float& operator[](int i) { return e[i]; }
	inline float operator[](int i) const { return e[i]; }
	
	vec3& operator+=(const vec3& v2);
	vec3& operator-=(const vec3& v2);
	vec3& operator*=(const vec3& v2);
	vec3& operator/=(const vec3& v2);
	vec3& operator*=(const float t);
	vec3& operator/=(const float t);

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

vec3 unit_vector(const vec3& v);
vec3 operator+(const vec3& v1, const vec3& v2);
vec3 operator-(const vec3& v1, const vec3& v2);
vec3 operator*(const vec3& v1, const vec3& v2);
vec3 operator/(const vec3& v1, const vec3& v2);
vec3 operator*(const float t, const vec3& v);
vec3 operator*(const vec3& v, const float t);
vec3 operator/(const vec3& v, const float t);
float dot(const vec3& v1, const vec3& v2);
vec3 cross(const vec3& v1, const vec3& v2);

vec3 reflect(const vec3& v, const vec3& n);
bool refract(const vec3& v, const vec3& n, float ni_over_nt, vec3& refracted);

inline std::istream& operator>>(std::istream& is, vec3& t)
{
	is >> t[0] >> t[1] >> t[2];
	return is;
}

inline std::ostream& operator <<(std::ostream& os, vec3& t)
{
	os << t[0] << " " << t[1] << " " << t[2];
	return os;
}

#endif