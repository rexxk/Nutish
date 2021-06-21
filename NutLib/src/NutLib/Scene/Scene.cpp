#include "nutpch.h"
#include "Scene.h"



namespace Nut
{

	Scene::Scene()
	{
		m_Registry = CreateRef<Registry>();
		m_Entity = CreateRef<Entity>(m_Registry);

	}

	Scene::~Scene()
	{

	}

	UUID Scene::CreateEntity(const std::string& tag)
	{
		return m_Entity->Create(tag);
	}

	void Scene::DeleteEntity(UUID id)
	{
		m_Entity->Remove(id);
	}

}
