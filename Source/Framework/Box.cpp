#include "Box.h"

Box::Box(const Vec3& p0, const Vec3& p1, Material *ptr)
{
	pmin = p0;
	pmax = p1;
	mat = ptr;
	setup();
}

Box::~Box()
{
	if (list_ptr)
	{
		delete list_ptr;
		list_ptr = nullptr;
	}
}

void Box::setup()
{
	Hitable **list = new Hitable *[6];
	list[0] = new XYRect(pmin.x(), pmax.x(), pmin.y(), pmax.y(), pmax.z(), mat);
	list[1] = new FlipNormals(new XYRect(pmin.x(), pmax.x(), pmin.y(), pmax.y(), pmin.z(), mat));
	list[2] = new XZRect(pmin.x(), pmax.x(), pmin.z(), pmax.z(), pmax.y(), mat);
	list[3] = new FlipNormals(new XZRect(pmin.x(), pmax.x(), pmin.z(), pmax.z(), pmin.y(), mat));
	list[4] = new YZRect(pmin.y(), pmax.y(), pmin.z(), pmax.z(), pmax.x(), mat);
	list[5] = new FlipNormals(new YZRect(pmin.y(), pmax.y(), pmin.z(), pmax.z(), pmin.x(), mat));

	list_ptr = new HitableList(list, 6);
}

bool Box::hit(const Ray& r, float t_min, float t_max, HitRecord& rec) const
{
	return list_ptr->hit(r, t_min, t_max, rec);
}

void Box::writeToJSON(rapidjson::Value* jsonValue, rapidjson::Document* document)
{
	Hitable::writeToJSON(jsonValue, document);

	rapidjson::Value::Object& jsonObject = jsonValue->GetObject();
	jsonObject.AddMember("Class", "Box", document->GetAllocator());

	{
		rapidjson::Value value;
		Vec3ToJSON(pmin, value, document);
		jsonObject.AddMember("Min", value, document->GetAllocator());
	}

	{
		rapidjson::Value value;
		Vec3ToJSON(pmax, value, document);
		jsonObject.AddMember("Max", value, document->GetAllocator());
	}

	if (mat)
	{
		rapidjson::Value matValue;
		mat->writeToJSON(&matValue, document);
		jsonObject.AddMember("Material", matValue, document->GetAllocator());
	}
}

void Box::readJSON(rapidjson::Value* jsonValue)
{
	Hitable::readJSON(jsonValue);

	rapidjson::Value::Object jsonObject = jsonValue->GetObject();

	READ_JSON_OBJECT_VEC3(jsonObject, "Min", pmin);
	READ_JSON_OBJECT_VEC3(jsonObject, "Max", pmax);

	READ_JSON_OBJECT_MATERIAL(jsonObject, "Material", mat);

	setup();
}
