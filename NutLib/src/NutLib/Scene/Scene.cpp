#include "nutpch.h"
#include "Scene.h"

#include "NutLib/Renderer/Renderer.h"
#include "NutLib/Renderer/RenderThread.h"

#include <glad/glad.h>

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

	void Scene::Draw()
	{

		// Mesh Components
		auto [meshIDs] = Entity::GetView<MeshComponent>();

		for (auto id : meshIDs)
		{

			auto& mesh = Entity::GetComponent<MeshComponent>(id).Mesh;
			auto& transform = Entity::GetComponent<TransformComponent>(id).Transform;

			Renderer::Submit(mesh, transform);

		}

	}

}
