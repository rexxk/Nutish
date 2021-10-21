#include "nutpch.h"
#include "MeshSource.h"

#include "NutLib/Scene/Scene.h"


namespace Nut
{


	MeshSource::MeshSource(DataBuffer<ShaderLayoutItem> vertices, const std::vector<uint32_t>& indices, Ref<Scene> scene)
	{
		if (scene != nullptr)
			m_ID = scene->CreateEntity("MeshSource");

		m_Vertices = vertices;
		m_Indices = indices;

		m_Buffers.VertexBuffer = VertexBuffer::Create(vertices);
		m_Buffers.IndexBuffer = IndexBuffer::Create(indices);
		m_Buffers.InstanceBuffer = VertexBuffer::Create(nullptr, 10000 * sizeof(glm::mat4), BufferUsage::Dynamic);

		m_Buffers.ID = m_ID;
	}

	void MeshSource::Load()
	{
		// TODO: Implement MeshAsset::Load()
		LOG_CORE_WARN("MeshAsset::Load is not implemented");
	}


	void MeshSource::SetVertexData(const DataBuffer<ShaderLayoutItem>& vertices)
	{
		m_Vertices = vertices;
	}

	void MeshSource::SetIndexData(const std::vector<uint32_t>& indices)
	{
		m_Indices = indices;
	}


}
