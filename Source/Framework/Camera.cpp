#include "Camera.h"
#include "JSONHelper.h"

void Camera::writeToJSON(rapidjson::Value* jsonValue, rapidjson::Document* document)
{
	jsonValue->SetObject();

	rapidjson::Value::Object& jsonObject = jsonValue->GetObject();

	jsonObject.AddMember("Time0", time0, document->GetAllocator());
	jsonObject.AddMember("Time1", time1, document->GetAllocator());
	jsonObject.AddMember("LensRadius", lens_radius, document->GetAllocator());
	jsonObject.AddMember("VFOV", vfov, document->GetAllocator());
	jsonObject.AddMember("AspectRatio", aspect, document->GetAllocator());
	jsonObject.AddMember("FocusDistance", focus_dist, document->GetAllocator());

	{
		rapidjson::Value origionValue;
		Vec3ToJSON(origin, origionValue, document);
		jsonObject.AddMember("Origin", origionValue, document->GetAllocator());
	}

	{
		rapidjson::Value targetValue;
		Vec3ToJSON(target, targetValue, document);
		jsonObject.AddMember("Target", targetValue, document->GetAllocator());
	}

	{
		rapidjson::Value vUpValue;
		Vec3ToJSON(vup, vUpValue, document);
		jsonObject.AddMember("VectorUp", vUpValue, document->GetAllocator());
	}
}

void Camera::readJSON(rapidjson::Value* jsonValue)
{
	rapidjson::Value::Object jsonObject = jsonValue->GetObject();

	READ_JSON_OBJECT_FLOAT(jsonObject, "Time0", time0);
	READ_JSON_OBJECT_FLOAT(jsonObject, "Time1", time1);
	READ_JSON_OBJECT_FLOAT(jsonObject, "LensRadius", lens_radius);
	READ_JSON_OBJECT_FLOAT(jsonObject, "VFOV", vfov);
	READ_JSON_OBJECT_FLOAT(jsonObject, "AspectRatio", aspect);
	READ_JSON_OBJECT_FLOAT(jsonObject, "FocusDistance", focus_dist);

	READ_JSON_OBJECT_VEC3(jsonObject, "Origin", origin);
	READ_JSON_OBJECT_VEC3(jsonObject, "Target", target);
	READ_JSON_OBJECT_VEC3(jsonObject, "VectorUp", vup);

	calculate();
}
