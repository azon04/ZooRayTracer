#include "PDF.h"
#include "JSONHelper.h"

void PDF::writeToJSON(rapidjson::Value* jsonValue, rapidjson::Document* document)
{
	jsonValue->SetObject();
	rapidjson::Value::Object jsonObject = jsonValue->GetObject();
	jsonObject.AddMember("Type", "PDF", document->GetAllocator());
}

void CosinePDF::writeToJSON(rapidjson::Value* jsonValue, rapidjson::Document* document)
{
	PDF::writeToJSON(jsonValue, document);
	rapidjson::Value::Object jsonObject = jsonValue->GetObject();
	jsonObject.AddMember("Class", "CosinePDF", document->GetAllocator());

	rapidjson::Value uvwValue;
	ONBToJSON(uvw, uvwValue, document);
	jsonObject.AddMember("UVW", uvwValue, document->GetAllocator());
}

void CosinePDF::readJSON(rapidjson::Value* jsonValue)
{
	PDF::readJSON(jsonValue);

	rapidjson::Value::Object jsonObject = jsonValue->GetObject();
	READ_JSON_OBJECT_ONB(jsonObject, "UVW", uvw);
}

void HitablePDF::writeToJSON(rapidjson::Value* jsonValue, rapidjson::Document* document)
{
	PDF::writeToJSON(jsonValue, document);
	rapidjson::Value::Object jsonObject = jsonValue->GetObject();
	jsonObject.AddMember("Class", "HitablePDF", document->GetAllocator());

	rapidjson::Value offsetValue;
	Vec3ToJSON(o, offsetValue, document);
	jsonObject.AddMember("UVW", offsetValue, document->GetAllocator());

	if (ptr)
	{
		rapidjson::Value hitableValue;
		ptr->writeToJSON(&hitableValue, document);
		jsonObject.AddMember("Hitable", hitableValue, document->GetAllocator());
	}
}

void HitablePDF::readJSON(rapidjson::Value* jsonValue)
{
	PDF::readJSON(jsonValue);

	rapidjson::Value::Object jsonObject = jsonValue->GetObject();
	READ_JSON_OBJECT_VEC3(jsonObject, "UVW", o);
	READ_JSON_OBJECT_HITABLE(jsonObject, "Hitable", ptr);
}

void MixturePDF::writeToJSON(rapidjson::Value* jsonValue, rapidjson::Document* document)
{
	PDF::writeToJSON(jsonValue, document);
	rapidjson::Value::Object jsonObject = jsonValue->GetObject();
	jsonObject.AddMember("Class", "MixturePDF", document->GetAllocator());

	if (p[0])
	{
		rapidjson::Value pdfValue;
		p[0]->writeToJSON(&pdfValue, document);
		jsonObject.AddMember("PDF0", pdfValue, document->GetAllocator());
	}

	if (p[1])
	{
		rapidjson::Value pdfValue;
		p[1]->writeToJSON(&pdfValue, document);
		jsonObject.AddMember("PDF1", pdfValue, document->GetAllocator());
	}
}

void MixturePDF::readJSON(rapidjson::Value* jsonValue)
{
	PDF::readJSON(jsonValue);

	rapidjson::Value::Object jsonObject = jsonValue->GetObject();
	READ_JSON_OBJECT_PDF(jsonObject, "PDF0", p[0]);
	READ_JSON_OBJECT_PDF(jsonObject, "PDF1", p[1]);
}
