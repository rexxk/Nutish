#include "nutpch.h"
#include "Object.h"


namespace Nut
{


	Object::Object(Ref<Scene> scene, const std::string& tag)
		: m_EntityID(scene->CreateEntity(tag))
	{

	}

	Object::~Object()
	{

	}


}
