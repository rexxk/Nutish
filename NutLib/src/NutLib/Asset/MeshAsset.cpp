#include "nutpch.h"
#include "MeshAsset.h"



namespace Nut
{


	MeshAsset::MeshAsset(const DataBuffer<ShaderLayoutItem>& vertexBuffer, const std::vector<uint32_t>& indexBuffer, Ref<Pipeline> pipeline, Ref<Scene> scene)
		: m_Pipeline(pipeline), m_Scene(scene)
	{
		AddSubmesh(vertexBuffer, indexBuffer);
	}

	MeshAsset::MeshAsset(Ref<Pipeline> pipeline, Ref<Scene> scene)
		: m_Pipeline(pipeline), m_Scene(scene)
	{

	}

	void MeshAsset::AddSubmesh(const DataBuffer<ShaderLayoutItem>& vertexBuffer, const std::vector<uint32_t>& indexBuffer)
	{
		m_Submeshes.push_back(MeshSource(vertexBuffer, indexBuffer, m_Pipeline, m_Scene));

	}
}
