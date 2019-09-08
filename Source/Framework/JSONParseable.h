#ifndef _JSON_PARSEABLE_H_
#define _JSON_PARSEABLE_H_

#include "rapidjson/document.h"

// Interface for JSON Parse-able
class JSONParseable
{
public:

	virtual void writeToJSON(rapidjson::Value* jsonValue, rapidjson::Document* document) = 0;
};
#endif // !_JSON_PARSEABLE_H_
