#include "JSONHelper.h"

void Vec3ToJSON(Vec3& vec3, rapidjson::Value& value, rapidjson::Document* document)
{
	value.SetObject();
	rapidjson::Value::Object& object = value.GetObject();
	object.AddMember("X", vec3.x(), document->GetAllocator());
	object.AddMember("Y", vec3.y(), document->GetAllocator());
	object.AddMember("Z", vec3.z(), document->GetAllocator());
}

void ONBToJSON(onb& val, rapidjson::Value& value, rapidjson::Document* document)
{
	value.SetObject();
	rapidjson::Value::Object& object = value.GetObject();
	
	rapidjson::Value uValue;
	Vec3ToJSON(val.u(), uValue, document);
	object.AddMember("U", uValue, document->GetAllocator());

	rapidjson::Value vValue;
	Vec3ToJSON(val.v(), vValue, document);
	object.AddMember("V", vValue, document->GetAllocator());

	rapidjson::Value wValue;
	Vec3ToJSON(val.w(), wValue, document);
	object.AddMember("W", wValue, document->GetAllocator());
}

void AABBToJSON(AABB& val, rapidjson::Value& value, rapidjson::Document* document)
{
	value.SetObject();
	rapidjson::Value::Object& object = value.GetObject();

	rapidjson::Value minValue;
	Vec3ToJSON(val.min(), minValue, document);
	object.AddMember("Min", minValue, document->GetAllocator());

	rapidjson::Value maxValue;
	Vec3ToJSON(val.max(), maxValue, document);
	object.AddMember("Max", maxValue, document->GetAllocator());
}

