#include "JSONParser.h"
#include "Hitable.h"

#include <iostream>
#include <fstream>

#include "rapidjson/prettywriter.h"
#include "rapidjson/stringbuffer.h"

void JSONWriter::writeWorld(Hitable* world, const char* filePath, Hitable* light_list)
{
	if (!world) 
	{ 
		// error can't write null world
		return; 
	} 

	rapidjson::Document document;
	document.SetObject();
	document.AddMember("Version", "1.0", document.GetAllocator());

	rapidjson::Value worldValue;
	world->writeToJSON(&worldValue, &document);
	document.AddMember("World", worldValue, document.GetAllocator());

	if (light_list)
	{
		rapidjson::Value lightListValue;
		light_list->writeToJSON(&lightListValue, &document);
		document.AddMember("LightList", lightListValue, document.GetAllocator());
	}

	rapidjson::StringBuffer buffer;
	rapidjson::PrettyWriter<rapidjson::StringBuffer> writter(buffer);
	document.Accept(writter);

	std::ofstream outputStream;
	outputStream.open(filePath, std::ofstream::out);

	outputStream << buffer.GetString();
}
