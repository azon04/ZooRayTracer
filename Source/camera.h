#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "ray.h"

#define _USE_MATH_DEFINES

#include <math.h>
#include "rand_helper.h"

vec3 random_in_unit_disk()
{
	vec3 p;
	do 
	{
		p = 2.0f * vec3(rand_float(), rand_float(), 0) - vec3(1.0f, 1.0f, 0.0f);
	} while (dot(p, p) >= 1.0f);
	return p;
}

class camera
{
public:
	camera(vec3 lookFrom, vec3 lookAt, vec3 vup, float vfov, float aspect, float aperture, float focus_dist, float t0, float t1)
	{
		time0 = t0;
		time1 = t1;
		lens_radius = aperture * 0.5f;
		float theta = vfov * M_PI / 180.0f;
		float half_height = tan(0.5f*theta);
		float half_width = aspect * half_height;
		origin = lookFrom;
		w = unit_vector(lookFrom - lookAt);
		u = unit_vector(cross(vup, w));
		v = cross(w, u);
		lower_left_corner = origin - half_width * focus_dist * u - half_height * focus_dist * v - focus_dist * w;
		horizontal = 2 * half_width * focus_dist * u;
		vertical = 2 * half_height * focus_dist * v;;
	}

	ray get_ray(float s, float t) 
	{
		vec3 rd = lens_radius * random_in_unit_disk();
		vec3 offset = rd.x() * u + rd.y() * v;
		float randTime = time0 + rand_float() * (time1 - time0);
		return ray(origin + offset, lower_left_corner + s * horizontal + t * vertical - origin - offset, randTime); 
	}

	vec3 origin;
	vec3 lower_left_corner;
	vec3 horizontal;
	vec3 vertical;
	vec3 u, v, w;
	float time0, time1; // shutter time to open and close
	float lens_radius;
};

#endif
