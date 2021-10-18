#include "nutpch.h"
#include "Scene.h"

#include "NutLib/Renderer/Renderer.h"


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
//			Renderer::GetRenderData().

			auto& mesh = Entity::GetComponent<MeshComponent>(id);
			auto& transform = Entity::GetComponent<TransformComponent>(id).Transform;

//			Renderer::Submit(mesh.AssetData, transform);

			mesh.Pipeline->Submit(mesh.AssetData->Vertices(), mesh.AssetData->Indices());
//			mesh.Pipeline->Submit(mesh.FloatVertices, mesh.Indices);
//			Renderer::Submit(mesh.Vertices, mesh.Indices);
		}

	}

}
