#include "ClassRegistry.h"

#include "Material.h"
#include "ConstantMedium.h"
#include "PDF.h"
#include "Texture.h"
#include "Box.h"
#include "BVH.h"
#include "HitableList.h"
#include "Mesh.h"
#include "PlaneHitable.h"
#include "Sphere.h"
#include "TransformHitable.h"

#define CREATE_CLASS_FUNCTION(ClassName) \
	void* CreateObject ## ClassName ## () {\
		return new ClassName;\
	}

// Class creation Functions
CREATE_CLASS_FUNCTION(Material)
CREATE_CLASS_FUNCTION(Lambertian)
CREATE_CLASS_FUNCTION(Metal)
CREATE_CLASS_FUNCTION(Dielectric)
CREATE_CLASS_FUNCTION(DiffuseLight)
CREATE_CLASS_FUNCTION(Box)
CREATE_CLASS_FUNCTION(BVHNode)
CREATE_CLASS_FUNCTION(Isotropic)
CREATE_CLASS_FUNCTION(ConstantMedium)
CREATE_CLASS_FUNCTION(HitableList)
CREATE_CLASS_FUNCTION(Triangle)
CREATE_CLASS_FUNCTION(Mesh)
CREATE_CLASS_FUNCTION(CosinePDF)
CREATE_CLASS_FUNCTION(HitablePDF)
CREATE_CLASS_FUNCTION(MixturePDF)
CREATE_CLASS_FUNCTION(ConstantColorTexture)
CREATE_CLASS_FUNCTION(CheckerTexture)
CREATE_CLASS_FUNCTION(NoiseTexture)
CREATE_CLASS_FUNCTION(ImageTexture)
CREATE_CLASS_FUNCTION(XYRect)
CREATE_CLASS_FUNCTION(XZRect)
CREATE_CLASS_FUNCTION(YZRect)
CREATE_CLASS_FUNCTION(Sphere)
CREATE_CLASS_FUNCTION(MovingSphere)
CREATE_CLASS_FUNCTION(FlipNormals)
CREATE_CLASS_FUNCTION(TranslateHitable)
CREATE_CLASS_FUNCTION(RotateY)


ClassRegistry* ClassRegistry::CreateClassRegistry()
{
	if (!s_instance)
	{
		s_instance = new ClassRegistry;
		s_instance->registerClasses();
	}
	return s_instance;
}

#define REGISTER_CLASS(ClassName) \
	m_creationMap[#ClassName] = CreateObject ## ClassName

void ClassRegistry::registerClasses()
{
	REGISTER_CLASS(Material);
	REGISTER_CLASS(Lambertian);
	REGISTER_CLASS(Metal);
	REGISTER_CLASS(Dielectric);
	REGISTER_CLASS(DiffuseLight);
	REGISTER_CLASS(Box);
	REGISTER_CLASS(BVHNode);
	REGISTER_CLASS(Isotropic);
	REGISTER_CLASS(ConstantMedium);
	REGISTER_CLASS(HitableList);
	REGISTER_CLASS(Triangle);
	REGISTER_CLASS(Mesh);
	REGISTER_CLASS(CosinePDF);
	REGISTER_CLASS(HitablePDF);
	REGISTER_CLASS(MixturePDF);
	REGISTER_CLASS(ConstantColorTexture);
	REGISTER_CLASS(CheckerTexture);
	REGISTER_CLASS(NoiseTexture);
	REGISTER_CLASS(ImageTexture);
	REGISTER_CLASS(XYRect);
	REGISTER_CLASS(XZRect);
	REGISTER_CLASS(YZRect); 
	REGISTER_CLASS(Sphere);
	REGISTER_CLASS(MovingSphere);
	REGISTER_CLASS(FlipNormals);
	REGISTER_CLASS(TranslateHitable);
	REGISTER_CLASS(RotateY);
}

void* ClassRegistry::createObjectByClassName(const char* className)
{
	if (m_creationMap.find(className) == m_creationMap.end())
	{
		return nullptr;
	}

	return m_creationMap[className]();
}

ClassRegistry* ClassRegistry::s_instance;
