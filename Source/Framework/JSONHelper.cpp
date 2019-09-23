#include "JSONHelper.h"
#include "ClassRegistry.h"
#include "Hitable.h"
#include "Material.h"
#include "Texture.h"
#include "PDF.h"
#include <iostream>
#include <assert.h>

void Vec3ToJSON(const Vec3& vec3, rapidjson::Value& value, rapidjson::Document* document)
{
	value.SetObject();
	rapidjson::Value::Object& object = value.GetObject();
	object.AddMember("X", vec3.x(), document->GetAllocator());
	object.AddMember("Y", vec3.y(), document->GetAllocator());
	object.AddMember("Z", vec3.z(), document->GetAllocator());
}

void ONBToJSON(const onb& val, rapidjson::Value& value, rapidjson::Document* document)
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

void AABBToJSON(const AABB& val, rapidjson::Value& value, rapidjson::Document* document)
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

void JSONToVec3(const rapidjson::Value& value, Vec3& vec3)
{
	rapidjson::Value::ConstObject jsonObject = value.GetObject();
	if (jsonObject.HasMember("X")) { vec3.e[0] = jsonObject["X"].GetFloat(); }
	if (jsonObject.HasMember("Y")) { vec3.e[1] = jsonObject["Y"].GetFloat(); }
	if (jsonObject.HasMember("Z")) { vec3.e[2] = jsonObject["Z"].GetFloat(); }
}

void JSONToONB(const rapidjson::Value& value, onb& val)
{
	rapidjson::Value::ConstObject jsonObject = value.GetObject();
	if (jsonObject.HasMember("U")) { JSONToVec3(jsonObject["U"], val.axis[0]); }
	if (jsonObject.HasMember("V")) { JSONToVec3(jsonObject["V"], val.axis[1]); }
	if (jsonObject.HasMember("W")) { JSONToVec3(jsonObject["W"], val.axis[2]); }
}

void JSONToAABB(const rapidjson::Value& value, AABB& aabb)
{
	rapidjson::Value::ConstObject jsonObject = value.GetObject();
	if (jsonObject.HasMember("Min")) { JSONToVec3(jsonObject["Min"], aabb._min); }
	if (jsonObject.HasMember("Max")) { JSONToVec3(jsonObject["Max"], aabb._max); }
}

Material* CreateMaterialFromJSON(rapidjson::Value& value)
{
	assert(value.IsObject());
	assert(value.GetObject().HasMember("Class"));

	Material* material = reinterpret_cast<Material*>(ClassRegistry::CreateClassRegistry()->createObjectByClassName(value.GetObject()["Class"].GetString()));
	
	assert(material != nullptr);

	material->readJSON(&value);

	return material;
}

Hitable* CreateHitableFromJSON(rapidjson::Value& value)
{
	assert(value.IsObject());
	assert(value.GetObject().HasMember("Class"));

	Hitable* hitable = reinterpret_cast<Hitable*>(ClassRegistry::CreateClassRegistry()->createObjectByClassName(value.GetObject()["Class"].GetString()));

	assert(hitable != nullptr);

	hitable->readJSON(&value);

	return hitable;
}

PDF* CreatePDFFromJSON(rapidjson::Value& value)
{
	assert(value.IsObject());
	assert(value.GetObject().HasMember("Class"));

	PDF* pdf = reinterpret_cast<PDF*>(ClassRegistry::CreateClassRegistry()->createObjectByClassName(value.GetObject()["Class"].GetString()));

	assert(pdf != nullptr);

	pdf->readJSON(&value);

	return pdf;
}

Texture* CreateTextureFromJSON(rapidjson::Value& value)
{
	assert(value.IsObject());
	assert(value.GetObject().HasMember("Class"));

	Texture* texture = reinterpret_cast<Texture*>(ClassRegistry::CreateClassRegistry()->createObjectByClassName(value.GetObject()["Class"].GetString()));

	assert(texture != nullptr);

	texture->readJSON(&value);

	return texture;
}

