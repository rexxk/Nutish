#ifndef __NUTLIB_ENTITY_H
#define __NUTLIB_ENTITY_H

#include "nutpch.h"

#include "Components.h"
#include "NutLib/Core/UUID.h"


namespace Nut
{

	template<typename T>
	class ComponentContainer;

	template<typename T>
	class View;

	class Registry
	{
		friend class Entity;

	private:
		UUID GetUUID()
		{
			UUID newID = UUID();
			m_Registry.insert(newID);

			return newID;
		}

		void Remove(UUID id)
		{
			auto loc = m_Registry.find(id);

			if (loc != m_Registry.end())
			{
				m_Registry.erase(loc);
			}
		}

	private:

		std::unordered_set<UUID> m_Registry;
	};


	class Entity
	{
	public:
		static const uint64_t Null = 0;

		Entity(Ref<Registry> registry)
			: m_Registry(registry)
		{

		}

		UUID Create(const std::string& tag)
		{
			UUID id = m_Registry->GetUUID();
			AddComponent<TagComponent>(id, tag);

			return id;
		}

		void Remove(UUID id)
		{
			m_Registry->Remove(id);
		}

		template<typename T, typename ... Args>
		static void AddComponent(UUID id, Args&&... args)
		{
			if (Entity::HaveComponent<T>(id))
			{
				LOG_CORE_WARN("Entity {0} already have component.", id);
				return;
			}

			ComponentContainer<T>::Add(id, T(std::forward<Args&&>(args)...));
			View<T>::Add(id);
		}

		template<typename T>
		static void RemoveComponent(UUID id)
		{
			if (Entity::HaveComponent<T>(id))
			{
				ComponentContainer<T>::Remove(id);
				View<T>::Remove(id);
			}
		}

		template<typename T>
		static bool HaveComponent(UUID id)
		{
			return View<T>::Find(id);
		}

		template<typename T>
		static T& GetComponent(UUID id)
		{
			return ComponentContainer<T>::GetComponent(id);
		}

		template<typename T>
		static std::tuple<const std::vector<UUID>> GetView()
		{
			return View<T>::GetView();
		}

	private:
		Ref<Registry> m_Registry;
	};

	template<typename T>
	class ComponentContainer
	{
	public:
		static void Add(UUID id, T&& component)
		{
			if (s_Instance == nullptr)
				s_Instance = new ComponentContainer();

			s_Instance->m_Components.try_emplace(id, component);
		}

		static void Remove(UUID id)
		{
			auto loc = s_Instance->m_Components.find(id);

			if (loc != s_Instance->m_Components.end())
			{
				s_Instance->m_Components.erase(loc);
			}
		}

		static T& GetComponent(UUID id)
		{
			if (s_Instance)
				return s_Instance->m_Components[id];

			return *(T*)nullptr;
		}

	private:
		static inline ComponentContainer* s_Instance = nullptr;

		std::unordered_map<UUID, T> m_Components;
	};

	template<typename T>
	class View
	{
	public:
		static void Add(UUID id)
		{
			if (s_Instance == nullptr)
				s_Instance = new View();

			s_Instance->m_Entities.push_back(id);
		}

		static void Remove(UUID id)
		{
			if (s_Instance)
			{
				auto loc = std::find(s_Instance->m_Entities.begin(), s_Instance->m_Entities.end(), id);

				if (loc != s_Instance->m_Entities.end())
				{
					s_Instance->m_Entities.erase(loc);
				}
			}
		}

		static bool Find(UUID id)
		{
			if (s_Instance != nullptr)
			{
				return std::find(s_Instance->m_Entities.begin(), s_Instance->m_Entities.end(), id) != s_Instance->m_Entities.end();
			}

			return false;
		}

		static const std::vector<UUID>& Get()
		{
			if (s_Instance == nullptr)
				s_Instance = new View;

			return s_Instance->m_Entities;
		}

	private:
		static inline View* s_Instance = nullptr;

		std::vector<UUID> m_Entities;
	};
}


#endif
