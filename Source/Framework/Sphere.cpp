#include "Sphere.h"
#include "JSONHelper.h"

bool Sphere::hit(const Ray& r, float t_min, float t_max, HitRecord& rec) const
{
	Vec3 oc = r.origin() - center;
	float a = dot(r.direction(), r.direction());
	float b = 2 * dot(r.direction(), oc);
	float c = dot(oc, oc) - radius * radius;
	float discriminant = b * b - 4 * a*c;
	if (discriminant > 0)
	{
		float t = (-b - sqrt(discriminant)) / (2.0f*a);
		if (t < t_max && t > t_min)
		{
			rec.t = t;
			rec.p = r.point_at_parameter(t);
			rec.normal = (rec.p - center) / radius;
			rec.mat_ptr = mat_ptr;
			get_sphere_uv(rec.normal, rec.u, rec.v);
			return true;
		}

		t = (-b + sqrt(discriminant)) / (2.0f*a);
		if (t < t_max && t > t_min)
		{
			rec.t = t;
			rec.p = r.point_at_parameter(t);
			rec.normal = (rec.p - center) / radius;
			rec.mat_ptr = mat_ptr;
			get_sphere_uv(rec.normal, rec.u, rec.v);
			return true;
		}
	}

	return false;
}

bool Sphere::bounding_box(float t0, float t1, AABB& box) const
{
	box = AABB(center - Vec3(radius), center + Vec3(radius));
	return true;
}

float Sphere::pdf_value(const Vec3& o, const Vec3& v) const
{
	HitRecord rec;
	if (this->hit(Ray(o, v), 0.001f, FLT_MAX, rec))
	{
		float cos_theta_max = sqrt(1 - radius * radius / (center - o).squared_length());
		float solid_angle = 2 * PI * (1 - cos_theta_max);
		return 1.0f / solid_angle;
	}
	else
		return 0;
}

Vec3 Sphere::random(const Vec3& o) const
{
	Vec3 direction = center - o;
	float distance_squared = direction.squared_length();
	onb uvw;
	uvw.build_from_w(direction);
	return uvw.local(random_to_sphere(radius, distance_squared));
}

void Sphere::writeToJSON(rapidjson::Value* jsonValue, rapidjson::Document* document)
{
	Hitable::writeToJSON(jsonValue, document);
	rapidjson::Value::Object jsonObject = jsonValue->GetObject();
	jsonObject.AddMember("Class", "Sphere", document->GetAllocator());
	jsonObject.AddMember("Radius", radius, document->GetAllocator());

	rapidjson::Value centerValue;
	Vec3ToJSON(center, centerValue, document);
	jsonObject.AddMember("Center", centerValue, document->GetAllocator());

	rapidjson::Value materialValue;
	if (mat_ptr)
	{
		mat_ptr->writeToJSON(&materialValue, document);
	}
	jsonObject.AddMember("Material", materialValue, document->GetAllocator());
}

void Sphere::readJSON(rapidjson::Value* jsonValue)
{
	Hitable::readJSON(jsonValue);

	rapidjson::Value::Object jsonObject = jsonValue->GetObject();

	if (jsonObject.HasMember("Radius")) { radius = jsonObject["Radius"].GetFloat(); }
	if (jsonObject.HasMember("Center")) { JSONToVec3(jsonObject["Center"], center); }
	if (jsonObject.HasMember("Material")) { mat_ptr = CreateMaterialFromJSON(jsonObject["Material"]); }
}

bool MovingSphere::hit(const Ray& r, float t_min, float t_max, HitRecord& rec) const
{
	Vec3 centerAtTime = center(r.time());
	Vec3 oc = r.origin() - centerAtTime;
	float a = dot(r.direction(), r.direction());
	float b = 2 * dot(r.direction(), oc);
	float c = dot(oc, oc) - radius * radius;
	float discriminant = b * b - 4 * a*c;
	if (discriminant > 0)
	{
		float t = (-b - sqrt(discriminant)) / (2.0f*a);
		if (t < t_max && t > t_min)
		{
			rec.t = t;
			rec.p = r.point_at_parameter(t);
			rec.normal = (rec.p - centerAtTime) / radius;
			rec.mat_ptr = mat_ptr;
			get_sphere_uv(rec.normal, rec.u, rec.v);
			return true;
		}

		t = (-b + sqrt(discriminant)) / (2.0f*a);
		if (t < t_max && t > t_min)
		{
			rec.t = t;
			rec.p = r.point_at_parameter(t);
			rec.normal = (rec.p - centerAtTime) / radius;
			rec.mat_ptr = mat_ptr;
			get_sphere_uv(rec.normal, rec.u, rec.v);
			return true;
		}
	}

	return false;
}

bool MovingSphere::bounding_box(float t0, float t1, AABB& box) const
{
	AABB box0(center(t0) - Vec3(radius), center(t0) + Vec3(radius));
	AABB box1(center(t1) - Vec3(radius), center(t1) + Vec3(radius));
	box = surrounding_box(box0, box1);
	return true;
}

void MovingSphere::writeToJSON(rapidjson::Value* jsonValue, rapidjson::Document* document)
{
	Hitable::writeToJSON(jsonValue, document);
	rapidjson::Value::Object jsonObject = jsonValue->GetObject();
	jsonObject.AddMember("Class", "MovingSphere", document->GetAllocator());
	jsonObject.AddMember("Radius", radius, document->GetAllocator());

	jsonObject.AddMember("Time0", time0, document->GetAllocator());
	jsonObject.AddMember("Time1", time1, document->GetAllocator());

	{
		rapidjson::Value centerValue;
		Vec3ToJSON(center0, centerValue, document);
		jsonObject.AddMember("Center0", centerValue, document->GetAllocator());
	}

	{
		rapidjson::Value centerValue;
		Vec3ToJSON(center1, centerValue, document);
		jsonObject.AddMember("Center1", centerValue, document->GetAllocator());
	}

	rapidjson::Value materialValue;
	if (mat_ptr)
	{
		mat_ptr->writeToJSON(&materialValue, document);
	}
	jsonObject.AddMember("Material", materialValue, document->GetAllocator());
}

void MovingSphere::readJSON(rapidjson::Value* jsonValue)
{
	Hitable::readJSON(jsonValue);

	rapidjson::Value::Object jsonObject = jsonValue->GetObject();

	if (jsonObject.HasMember("Radius")) { radius = jsonObject["Radius"].GetFloat(); }
	if (jsonObject.HasMember("Time0")) { time0 = jsonObject["Time0"].GetFloat(); }
	if (jsonObject.HasMember("Time1")) { time1 = jsonObject["Time1"].GetFloat(); }
	if (jsonObject.HasMember("Center0")) { JSONToVec3(jsonObject["Center0"], center0); }
	if (jsonObject.HasMember("Center1")) { JSONToVec3(jsonObject["Center1"], center1); }
	if (jsonObject.HasMember("Material")) { mat_ptr = CreateMaterialFromJSON(jsonObject["Material"]); }
}

Vec3 MovingSphere::center(float time) const
{
	return center0 + ((time - time0) / (time1 - time0)) * (center1 - center0);
}

void get_sphere_uv(const Vec3& p, float& u, float& v)
{
	float phi = atan2(p.z(), p.x());
	float thetha = asin(p.y());
	u = 1 - (phi + PI) / (2 * PI);
	v = (thetha + PI / 2.0f) / PI;
}

Vec3 random_to_sphere(float radius, float distance_squared)
{
	float r1 = rand_float();
	float r2 = rand_float();
	float z = 1 + r2 * (sqrt(1 - radius * radius / distance_squared) - 1.0f);
	float phi = 2 * PI * r1;
	float x = cos(phi) * sqrt(1 - z * z);
	float y = sin(phi) * sqrt(1 - z * z);
	return Vec3(x, y, z);
}