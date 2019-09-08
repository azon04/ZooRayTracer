#ifndef _JSON_PARSER_H_
#define _JSON_PARSER_H_

#include "rapidjson/document.h"

class Hitable;
class JSONWriter
{
public:

	void writeWorld(Hitable* world, const char* filePath, Hitable* lightList = nullptr);
};

#endif