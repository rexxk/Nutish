#pragma once

#include "NutLib/ECS/ECS.h"


namespace Nut
{


	class Entity
	{
	public:
		Entity();
		Entity(Ref<ECS::Registry> registry, const std::string& tag);

		template<typename T, typename... Args>
		void AddComponent(Args&&... args)
		{
			ECS::EntitySystem::AddComponent<T>(m_ID, std::forward<Args&&>(args)...);
		}

		template<typename T>
		void RemoveComponent()
		{
			ECS::EntitySystem::RemoveComponent<T>(m_ID);
		}

		template<typename T>
		bool HaveComponent()
		{
			return ECS::EntitySystem::HaveComponent<T>(m_ID);
		}

		template<typename T>
		T& GetComponent()
		{
			return ECS::EntitySystem::GetComponent<T>(m_ID);
		}

		UUID ID() const { return m_ID; }

	private:
		UUID m_ID;
	};


}
