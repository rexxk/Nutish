#pragma once


#include "NutLib/Core/UUID.h"
#include "Scene.h"


namespace Nut
{

	class Object
	{
	public:
		Object(Ref<Scene> scene, const std::string& tag = "object");
		virtual ~Object();

		virtual const UUID ID() const { return m_EntityID; }

	private:
		UUID m_EntityID;

	};


}
