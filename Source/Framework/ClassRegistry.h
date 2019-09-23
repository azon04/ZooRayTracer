#ifndef _CLASS_REGISTRY_H_
#define _CLASS_REGISTRY_H_

#include <string>
#include <map>

class ClassRegistry
{
public:
	typedef void*(*ObjectCreation)();

	static ClassRegistry* CreateClassRegistry();

	void registerClasses();
	void* createObjectByClassName(const char* className);

protected:
	ClassRegistry() {}

	std::map<std::string, ObjectCreation> m_creationMap;

private:
	static ClassRegistry* s_instance;

};
#endif
