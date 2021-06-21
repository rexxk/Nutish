#pragma once

#include "Entity.h"



namespace Nut
{

	class Scene
	{
	public:
		Scene();
		virtual ~Scene();

		UUID CreateEntity(const std::string& tag);
		void DeleteEntity(UUID id);

	private:

		Ref<Registry> m_Registry;
		Ref<Entity> m_Entity;

	};

}
