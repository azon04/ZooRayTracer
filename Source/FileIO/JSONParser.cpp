#include "JSONParser.h"
#include "Hitable.h"

#include <iostream>
#include <fstream>
#include <sstream>

#include "rapidjson/prettywriter.h"
#include "rapidjson/stringbuffer.h"

#include "Framework/JSONHelper.h"

void JSONParser::writeWorld(const char* filePath, WorldInfo& world)
{
	if (!world.world)
	{
		// error can't write null world
		return;
	}

	rapidjson::Document document;
	document.SetObject();
	document.AddMember("Version", "1.0", document.GetAllocator());

	// Write Camera
	{
		rapidjson::Value cameraValue;
		world.camera.writeToJSON(&cameraValue, &document);

		document.AddMember("Camera", cameraValue, document.GetAllocator());
	}

	// Write Render Settings
	{
		rapidjson::Value RenderSettingsValue;
		RenderSettingsValue.SetObject();
		rapidjson::Value::Object jsonObject = RenderSettingsValue.GetObject();
		jsonObject.AddMember("SampleCount", world.renderSettings.sampleCount, document.GetAllocator());
		jsonObject.AddMember("Width", world.renderSettings.width, document.GetAllocator());
		jsonObject.AddMember("Height", world.renderSettings.height, document.GetAllocator());
		jsonObject.AddMember("RayDepth", world.renderSettings.rayDepth, document.GetAllocator());

		document.AddMember("RenderSettings", RenderSettingsValue, document.GetAllocator());
	}

	rapidjson::Value worldValue;
	world.world->writeToJSON(&worldValue, &document);
	document.AddMember("World", worldValue, document.GetAllocator());

	if (world.lightList)
	{
		rapidjson::Value lightListValue;
		world.lightList->writeToJSON(&lightListValue, &document);
		document.AddMember("LightList", lightListValue, document.GetAllocator());
	}

	rapidjson::StringBuffer buffer;
	rapidjson::PrettyWriter<rapidjson::StringBuffer> writter(buffer);
	document.Accept(writter);

	std::ofstream outputStream;
	outputStream.open(filePath, std::ofstream::out);

	outputStream << buffer.GetString();
}

bool JSONParser::readWorld(const char* filePath, WorldInfo& world)
{
	rapidjson::Document document;
	std::ifstream inputStream;
	inputStream.open(filePath);
	if (!inputStream.is_open())
	{
		// Can't open input stream
		return false;
	}

	std::stringstream inputStringStream;
	inputStringStream << inputStream.rdbuf();

	if (document.Parse(inputStringStream.str().c_str()).HasParseError())
	{
		//Parse error
		return false;
	}

	// Read Version

	// Read Camera
	if(document.HasMember("Camera"))
	{
		rapidjson::Value& cameraValue = document["Camera"];
		world.camera.readJSON(&cameraValue);
	}

	// Write Render Settings
	if(document.HasMember("RenderSettings"))
	{
		rapidjson::Value& RenderSettingsValue = document["RenderSettings"];
		rapidjson::Value::Object jsonObject = RenderSettingsValue.GetObject();
		READ_JSON_OBJECT_INT(jsonObject, "SampleCount", world.renderSettings.sampleCount);
		READ_JSON_OBJECT_INT(jsonObject, "Width", world.renderSettings.width);
		READ_JSON_OBJECT_INT(jsonObject, "Height", world.renderSettings.height);
		READ_JSON_OBJECT_INT(jsonObject, "RayDepth", world.renderSettings.rayDepth);
	}

	if (document.HasMember("World"))
	{
		rapidjson::Value& worldValue = document["World"];
		world.world = CreateHitableFromJSON(worldValue);
	}
	else
	{
		world.world = nullptr;
	}

	if (document.HasMember("LightList"))
	{
		rapidjson::Value& lightListValue = document["LightList"];
		world.lightList = CreateHitableFromJSON(lightListValue);
	}
	else
	{
		world.lightList = nullptr;
	}

	return true;
}
