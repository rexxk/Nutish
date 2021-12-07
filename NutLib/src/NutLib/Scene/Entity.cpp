#include "nutpch.h"
#include "Entity.h"

#include "Components.h"


namespace Nut
{


	Entity::Entity()
	{
		m_ID = ECS::EntitySystem::Null;
	}

	Entity::Entity(Ref<ECS::Registry> registry, const std::string& tag)
	{
		m_ID = ECS::EntitySystem::Create(registry);

		AddComponent<TagComponent>(tag);
	}



}
