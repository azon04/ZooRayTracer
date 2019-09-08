#ifndef _JSON_HELPER_H_
#define _JSON_HELPER_H_

#include "Math/Vec3.h"
#include "Math/AABB.h"
#include "rapidjson/document.h"

void Vec3ToJSON(Vec3& vec3, rapidjson::Value& value, rapidjson::Document* document);
void ONBToJSON(onb& val, rapidjson::Value& value, rapidjson::Document* document);
void AABBToJSON(AABB& val, rapidjson::Value& value, rapidjson::Document* document);
#endif
