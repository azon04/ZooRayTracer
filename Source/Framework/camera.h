#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "Ray.h"

#define _USE_MATH_DEFINES

#include <math.h>
#include "rand_helper.h"
#include "math_utils.h"
#include "JSONParseable.h"

class Camera : public JSONParseable
{
public:
	Camera() {}
	Camera(Vec3 lookFrom, Vec3 lookAt, Vec3 vup, float vfov, float aspect, float aperture, float focus_dist, float t0, float t1)
	{
		time0 = t0;
		time1 = t1;
		lens_radius = aperture * 0.5f;
		this->vfov = vfov;
		this->aspect = aspect;
		this->focus_dist = focus_dist;
		origin = lookFrom;
		target = lookAt;
		this->vup = vup;		

		calculate();
	}

	void calculate()
	{
		float theta = vfov * PI / 180.0f;
		float half_height = tan(0.5f*theta);
		float half_width = aspect * half_height;
		w = unit_vector(origin - target);
		u = unit_vector(cross(vup, w));
		v = cross(w, u);
		lower_left_corner = origin - half_width * focus_dist * u - half_height * focus_dist * v - focus_dist * w;
		horizontal = 2 * half_width * focus_dist * u;
		vertical = 2 * half_height * focus_dist * v;;
	}

	Ray get_ray(float s, float t) 
	{
		Vec3 rd = lens_radius * random_in_unit_disk();
		Vec3 offset = rd.x() * u + rd.y() * v;
		float randTime = time0 + rand_float() * (time1 - time0);
		return Ray(origin + offset, lower_left_corner + s * horizontal + t * vertical - origin - offset, randTime); 
	}

	virtual void writeToJSON(rapidjson::Value* jsonValue, rapidjson::Document* document);
	virtual void readJSON(rapidjson::Value* jsonValue);

	Vec3 origin;
	Vec3 lower_left_corner;
	Vec3 horizontal;
	Vec3 vertical;
	Vec3 u, v, w;
	float time0, time1; // shutter time to open and close
	float lens_radius;

	// Input Cached Variables
	Vec3 target;
	Vec3 vup;
	float vfov;
	float aspect;
	float focus_dist;
};

#endif
