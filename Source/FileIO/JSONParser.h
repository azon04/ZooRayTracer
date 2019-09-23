#ifndef _JSON_PARSER_H_
#define _JSON_PARSER_H_

#include "rapidjson/document.h"
#include "Camera.h"

class Hitable;

struct WorldInfo
{
	Hitable* world;
	Hitable* lightList;
	Camera camera;

	struct RenderSettings
	{
		int sampleCount;
		int width;
		int height;
		int rayDepth;
	} renderSettings;
};

class JSONParser
{
public:

	void writeWorld(const char* filePath, WorldInfo&  world);
	bool readWorld(const char* filePath, WorldInfo& world);
};

#endif