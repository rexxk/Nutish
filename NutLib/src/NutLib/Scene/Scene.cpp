#include "nutpch.h"
#include "Scene.h"

#include "Entity.h"

#include "NutLib/Renderer/Renderer.h"
#include "NutLib/Renderer/RenderThread.h"

#include <glad/glad.h>

#include <glm/glm.hpp>


namespace Nut
{

	Scene::Scene()
	{
		m_Registry = CreateRef<ECS::Registry>();
		m_EntitySystem = CreateRef<ECS::EntitySystem>(m_Registry);

	}

	Scene::~Scene()
	{

	}

	Entity Scene::CreateEntity(const std::string& tag)
	{
		Entity entity(m_Registry, tag);

		return entity;
	}

	void Scene::DeleteEntity(const Entity& entity)
	{
		m_EntitySystem->Remove(entity.ID());
	}

	void Scene::Draw()
	{


		// Mesh Components
		auto [meshIDs] = ECS::EntitySystem::GetView<MeshComponent>();

		for (auto id : meshIDs)
		{

			auto& mesh = ECS::EntitySystem::GetComponent<MeshComponent>(id).Mesh;
			auto& transform = ECS::EntitySystem::GetComponent<TransformComponent>(id).Transform;

			Renderer::Submit(mesh, transform);

		}

	}

}
