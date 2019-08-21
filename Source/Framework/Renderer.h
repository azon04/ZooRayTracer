#ifndef __RENDERER_H__
#define __RENDERER_H__

#include "Camera.h"
#include "Hitable.h"

class Renderer
{
public:

	Renderer() {}
	
	void render(Camera camera, Hitable* world, Hitable* light_shape);

protected:
	Vec3 color(const Ray& r, Hitable* world, Hitable* light_shape, int depth);

public:
	int sample_count;
	int width;
	int height;
	int ray_depth;
};

#endif
