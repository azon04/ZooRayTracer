#include "HitableList.h"
#include "JSONHelper.h"

bool HitableList::hit(const Ray& r, float t_min, float t_max, HitRecord& rec) const
{
	HitRecord temp_rec;
	bool hit_anything = false;
	float closest = t_max;
	for (int i = 0; i < list_size; i++)
	{
		if (list[i]->hit(r, t_min, closest, temp_rec))
		{
			hit_anything = true;
			closest = temp_rec.t;
			rec = temp_rec;
		}
	}
	return hit_anything;
}

bool HitableList::bounding_box(float t0, float t1, AABB& box) const
{
	if (list_size < 1) { return false; }
	AABB temp_box;
	bool first_true = list[0]->bounding_box(t0, t1, temp_box);
	if (!first_true)
		return false;
	else
		box = temp_box;

	for (int i = 1; i < list_size; i++)
	{
		if (list[i]->bounding_box(t0, t1, temp_box))
		{
			box = surrounding_box(box, temp_box);
		}
		else
			return false;
	}
	return true;
}

float HitableList::pdf_value(const Vec3& o, const Vec3& v) const
{
	float weight = 1.0f / list_size;
	float sum = 0;
	for (int i = 0; i < list_size; i++)
	{
		sum += weight * list[i]->pdf_value(o, v);
	}
	return sum;
}

Vec3 HitableList::random(const Vec3& o) const
{
	int index = int(rand_float() * list_size);
	if (index == list_size) index = list_size - 1;
	return list[index]->random(o);
}

void HitableList::writeToJSON(rapidjson::Value* jsonValue, rapidjson::Document* document)
{
	Hitable::writeToJSON(jsonValue, document);
	rapidjson::Value::Object& jsonObject = jsonValue->GetObject();
	jsonObject.AddMember("Class", "HitableList", document->GetAllocator());
	jsonObject.AddMember("ListSize", list_size, document->GetAllocator());

	rapidjson::Value listArrayValue;
	listArrayValue.SetArray();
	for (int i = 0; i < list_size; i++)
	{
		rapidjson::Value value;
		list[i]->writeToJSON(&value, document);
		listArrayValue.GetArray().PushBack(value, document->GetAllocator());
	}

	jsonObject.AddMember("List", listArrayValue, document->GetAllocator());
}

void HitableList::readJSON(rapidjson::Value* jsonValue)
{
	Hitable::readJSON(jsonValue);

	rapidjson::Value::Object jsonObject = jsonValue->GetObject();
	READ_JSON_OBJECT_INT(jsonObject, "ListSize", list_size);

	if (list_size <= 0) { return; }

	list = new Hitable*[list_size];
	rapidjson::Value::Array jsonArray = jsonObject["List"].GetArray();
	for (int i = 0; i < list_size; i++)
	{
		list[i] = CreateHitableFromJSON(jsonArray[i]);
	}
}