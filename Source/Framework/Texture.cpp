#include "Texture.h"
#include "JSONHelper.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

void ConstantColorTexture::writeToJSON(rapidjson::Value* jsonValue, rapidjson::Document* document)
{
	Texture::writeToJSON(jsonValue, document);

	rapidjson::Value::Object jsonObject = jsonValue->GetObject();
	jsonObject.AddMember("Class", "ConstantColorTexture", document->GetAllocator());

	rapidjson::Value colorValue;
	Vec3ToJSON(color, colorValue, document);
	jsonObject.AddMember("Color", colorValue, document->GetAllocator());
}

void ConstantColorTexture::readJSON(rapidjson::Value* jsonValue)
{
	Texture::readJSON(jsonValue);

	rapidjson::Value::Object jsonObject = jsonValue->GetObject();
	READ_JSON_OBJECT_VEC3(jsonObject, "Color", color);
}

void CheckerTexture::writeToJSON(rapidjson::Value* jsonValue, rapidjson::Document* document)
{
	Texture::writeToJSON(jsonValue, document);

	rapidjson::Value::Object jsonObject = jsonValue->GetObject();
	jsonObject.AddMember("Class", "CheckerTexture", document->GetAllocator());

	if (odd)
	{
		rapidjson::Value oddValue;
		odd->writeToJSON(&oddValue, document);
		jsonObject.AddMember("OddTexture", oddValue, document->GetAllocator());
	}

	if (even)
	{
		rapidjson::Value evenValue;
		even->writeToJSON(&evenValue, document);
		jsonObject.AddMember("EvenTexture", evenValue, document->GetAllocator());
	}
}

void CheckerTexture::readJSON(rapidjson::Value* jsonValue)
{
	Texture::readJSON(jsonValue);

	rapidjson::Value::Object jsonObject = jsonValue->GetObject();

	READ_JSON_OBJECT_TEXTURE(jsonObject, "OddTexture", odd);
	READ_JSON_OBJECT_TEXTURE(jsonObject, "EvenTexture", even);
}

void NoiseTexture::writeToJSON(rapidjson::Value* jsonValue, rapidjson::Document* document)
{
	Texture::writeToJSON(jsonValue, document);

	rapidjson::Value::Object& jsonObject = jsonValue->GetObject();
	jsonObject.AddMember("Class", "NoiseTexture", document->GetAllocator());
	jsonObject.AddMember("NoiseScale", scale, document->GetAllocator());

}

void NoiseTexture::readJSON(rapidjson::Value* jsonValue)
{
	Texture::readJSON(jsonValue);

	rapidjson::Value::Object jsonObject = jsonValue->GetObject();
	READ_JSON_OBJECT_FLOAT(jsonObject, "NoiseScale", scale);
}

void ImageTexture::writeToJSON(rapidjson::Value* jsonValue, rapidjson::Document* document)
{
	Texture::writeToJSON(jsonValue, document);

	rapidjson::Value::Object jsonObject = jsonValue->GetObject();
	jsonObject.AddMember("Class", "ImageTexture", document->GetAllocator());
	jsonObject.AddMember("ImagePath", rapidjson::StringRef(imagePath), document->GetAllocator());
	jsonObject.AddMember("Width", nx, document->GetAllocator());
	jsonObject.AddMember("Height", ny, document->GetAllocator());
}

void ImageTexture::readJSON(rapidjson::Value* jsonValue)
{
	Texture::readJSON(jsonValue);

	rapidjson::Value::Object jsonObject = jsonValue->GetObject();
	READ_JSON_OBJECT_INT(jsonObject, "Width", nx);
	READ_JSON_OBJECT_INT(jsonObject, "Height", ny);

	if (jsonObject.HasMember("ImagePath"))
	{
		strncpy(imagePath, jsonObject["ImagePath"].GetString(), jsonObject["ImagePath"].GetStringLength());
		imagePath[jsonObject["ImagePath"].GetStringLength()] = '\0';
		int width, height, channel;
		data = stbi_load(imagePath, &width, &height, &channel, 0);
	}
}
