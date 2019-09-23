#include "BVH.h"
#include "JSONHelper.h"

int box_x_compare(const void* a, const void* b)
{
	AABB box_left, box_right;
	Hitable* ha = *(Hitable**)a;
	Hitable* hb = *(Hitable**)b;
	if (!ha->bounding_box(0, 0, box_left) || !hb->bounding_box(0, 0, box_right))
	{
		std::cerr << "no bounding box in bvh_node constructor\n";
	}

	if (box_left.min().x() - box_right.min().x() < 0.0f)
	{
		return -1;
	}
	else
	{
		return 1;
	}
}

int box_y_compare(const void* a, const void* b)
{
	AABB box_left, box_right;
	Hitable* ha = *(Hitable**)a;
	Hitable* hb = *(Hitable**)b;
	if (!ha->bounding_box(0, 0, box_left) || !hb->bounding_box(0, 0, box_right))
	{
		std::cerr << "no bounding box in bvh_node constructor\n";
	}

	if (box_left.min().y() - box_right.min().y() < 0.0f)
	{
		return -1;
	}
	else
	{
		return 1;
	}
}

int box_z_compare(const void* a, const void* b)
{
	AABB box_left, box_right;
	Hitable* ha = *(Hitable**)a;
	Hitable* hb = *(Hitable**)b;
	if (!ha->bounding_box(0, 0, box_left) || !hb->bounding_box(0, 0, box_right))
	{
		std::cerr << "no bounding box in bvh_node constructor\n";
	}

	if (box_left.min().z() - box_right.min().z() < 0.0f)
	{
		return -1;
	}
	else
	{
		return 1;
	}
}

BVHNode::BVHNode(Hitable **l, int n, float time0, float time1)
{
	list = l;
	listSize = n;
	this->time0 = time0;
	this->time1 = time1;

	int axis = int(rand_float() * 3);
	if (axis == 0)
	{
		qsort(l, n, sizeof(Hitable*), box_x_compare);
	}
	else if (axis == 1)
	{
		qsort(l, n, sizeof(Hitable*), box_y_compare);
	}
	else
	{
		qsort(l, n, sizeof(Hitable*), box_z_compare);
	}

	if (n == 1)
	{
		right = left = l[0];
	}
	else if (n == 2)
	{
		left = l[0];
		right = l[1];
	}
	else
	{
		left = new BVHNode(l, n / 2, time0, time1);
		right = new BVHNode(l + n / 2, n - n / 2, time0, time1);
	}

	AABB box_left, box_right;
	if (!left->bounding_box(time0, time1, box_left) || !right->bounding_box(time0, time1, box_right))
	{
		std::cerr << "no bounding box in bvh_node constructor\n";
	}

	box = surrounding_box(box_left, box_right);
}

void BVHNode::construct()
{
	int axis = int(rand_float() * 3);
	if (axis == 0)
	{
		qsort(list, listSize, sizeof(Hitable*), box_x_compare);
	}
	else if (axis == 1)
	{
		qsort(list, listSize, sizeof(Hitable*), box_y_compare);
	}
	else
	{
		qsort(list, listSize, sizeof(Hitable*), box_z_compare);
	}

	if (listSize == 1)
	{
		right = left = list[0];
	}
	else if (listSize == 2)
	{
		left = list[0];
		right = list[1];
	}
	else
	{
		left = new BVHNode(list, listSize / 2, time0, time1);
		right = new BVHNode(list + listSize / 2, listSize - listSize / 2, time0, time1);
	}

	AABB box_left, box_right;
	if (!left->bounding_box(time0, time1, box_left) || !right->bounding_box(time0, time1, box_right))
	{
		std::cerr << "no bounding box in bvh_node constructor\n";
	}

	box = surrounding_box(box_left, box_right);
}

bool BVHNode::hit(const Ray& r, float t_min, float t_max, HitRecord& rec) const
{
	if (box.hit(r, t_min, t_max))
	{
		HitRecord left_rec, right_rec;
		bool hit_left = left->hit(r, t_min, t_max, left_rec);
		bool hit_right = right->hit(r, t_min, t_max, right_rec);

		if (hit_left && hit_right)
		{
			if (left_rec.t < right_rec.t)
			{
				rec = left_rec;
			}
			else
			{
				rec = right_rec;
			}
			return true;
		}
		else if (hit_left)
		{
			rec = left_rec;
			return true;
		}
		else if (hit_right)
		{
			rec = right_rec;
			return true;
		}
		else
		{
			return false;
		}
	}
	return false;
}

bool BVHNode::bounding_box(float t0, float t1, AABB& b) const
{
	b = box;
	return true;
}

void BVHNode::writeToJSON(rapidjson::Value* jsonValue, rapidjson::Document* document)
{
	Hitable::writeToJSON(jsonValue, document);

	rapidjson::Value::Object& jsonObject = jsonValue->GetObject();
	jsonObject.AddMember("Class", "BVHNode", document->GetAllocator());
	jsonObject.AddMember("Time0", time0, document->GetAllocator());
	jsonObject.AddMember("Time1", time1, document->GetAllocator());
	jsonObject.AddMember("ListSize", listSize, document->GetAllocator());

	rapidjson::Value listArrayValue;
	listArrayValue.SetArray();
	for (int i = 0; i < listSize; i++)
	{
		rapidjson::Value value;
		list[i]->writeToJSON(&value, document);
		listArrayValue.GetArray().PushBack(value, document->GetAllocator());
	}

	jsonObject.AddMember("List", listArrayValue, document->GetAllocator());

	/*
	//Hitable *left;
	if (left)
	{
		rapidjson::Value leftValue;
		left->writeToJSON(&leftValue, document);
		jsonObject.AddMember("Left", leftValue, document->GetAllocator());
	}

	//Hitable *right;
	if (right)
	{
		rapidjson::Value rightValue;
		right->writeToJSON(&rightValue, document);
		jsonObject.AddMember("Right", rightValue, document->GetAllocator());
	}

	//AABB box;
	{
		rapidjson::Value aabbValue;
		AABBToJSON(box, aabbValue, document);
		jsonObject.AddMember("AABBBox", aabbValue, document->GetAllocator());
	}
	*/
}

void BVHNode::readJSON(rapidjson::Value* jsonValue)
{
	Hitable::readJSON(jsonValue);

	rapidjson::Value::Object jsonObject = jsonValue->GetObject();

	READ_JSON_OBJECT_FLOAT(jsonObject, "Time0", time0);
	READ_JSON_OBJECT_FLOAT(jsonObject, "Time1", time1);
	READ_JSON_OBJECT_INT(jsonObject, "ListSize", listSize);

	list = new Hitable*[listSize];
	rapidjson::Value::Array jsonArray = jsonObject["List"].GetArray();
	for (int i = 0; i < listSize; i++)
	{
		list[i] = CreateHitableFromJSON(jsonArray[i]);
	}

	construct();
}
