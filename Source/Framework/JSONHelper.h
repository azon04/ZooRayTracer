#ifndef _JSON_HELPER_H_
#define _JSON_HELPER_H_

#include "Math/Vec3.h"
#include "Math/AABB.h"
#include "rapidjson/document.h"

class Material;
class Hitable;
class PDF;
class Texture;

#define READ_JSON_OBJECT_FLOAT(JsonObject, StringId, Variable) \
	if(JsonObject.HasMember(StringId)) { Variable = JsonObject[StringId].GetFloat(); }

#define READ_JSON_OBJECT_INT(JsonObject, StringId, Variable) \
	if(JsonObject.HasMember(StringId)) { Variable = JsonObject[StringId].GetInt(); }

#define READ_JSON_OBJECT_MATERIAL(JsonObject, StringId, Variable) \
	if(JsonObject.HasMember(StringId)) { Variable = CreateMaterialFromJSON(JsonObject[StringId]); }

#define READ_JSON_OBJECT_TEXTURE(JsonObject, StringId, Variable) \
	if(JsonObject.HasMember(StringId)) { Variable = CreateTextureFromJSON(JsonObject[StringId]); }

#define READ_JSON_OBJECT_PDF(JsonObject, StringId, Variable) \
	if(JsonObject.HasMember(StringId)) { Variable = CreatePDFFromJSON(JsonObject[StringId]); }

#define READ_JSON_OBJECT_HITABLE(JsonObject, StringId, Variable) \
	if(JsonObject.HasMember(StringId)) { Variable = CreateHitableFromJSON(JsonObject[StringId]); }

#define READ_JSON_OBJECT_VEC3(JsonObject, StringId, Variable) \
	if(JsonObject.HasMember(StringId)) { JSONToVec3(JsonObject[StringId], Variable); }

#define READ_JSON_OBJECT_ONB(JsonObject, StringId, Variable) \
	if(JsonObject.HasMember(StringId)) { JSONToONB(JsonObject[StringId], Variable); }

void Vec3ToJSON(const Vec3& vec3, rapidjson::Value& value, rapidjson::Document* document);
void ONBToJSON(const onb& val, rapidjson::Value& value, rapidjson::Document* document);
void AABBToJSON(const AABB& val, rapidjson::Value& value, rapidjson::Document* document);

void JSONToVec3(const rapidjson::Value& value, Vec3& vec3);
void JSONToONB(const rapidjson::Value& value, onb& val);
void JSONToAABB(const rapidjson::Value& value, AABB& aabb);

Material* CreateMaterialFromJSON(rapidjson::Value& value);
Hitable* CreateHitableFromJSON(rapidjson::Value& value);
PDF* CreatePDFFromJSON(rapidjson::Value& value);
Texture* CreateTextureFromJSON(rapidjson::Value& value);

#endif
