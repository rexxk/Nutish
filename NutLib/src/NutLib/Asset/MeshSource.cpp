#include "nutpch.h"
#include "MeshSource.h"

#include "NutLib/Scene/Scene.h"

#include "NutLib/Renderer/RenderThread.h"

#include <glad/glad.h>


namespace Nut
{


	MeshObject::MeshObject(Ref<Pipeline> pipeline)
		: m_Pipeline(pipeline)
	{
		RendererID& id = m_ID;

		RenderThread::Submit([&id]()
			{
				glGenVertexArrays(1, &id);
				glBindVertexArray(id);
			});

		m_Pipeline->Bind();
	}

	MeshObject::~MeshObject()
	{
		RenderThread::Submit([=]()
			{
				glDeleteVertexArrays(1, &m_ID);
			});
	}

	void MeshObject::SetVertexBuffer(Ref<VertexBuffer> vertexBuffer)
	{
		m_VertexBuffer = vertexBuffer;

		m_VertexBuffer->Bind();

		m_Pipeline->SetBufferLayout();
	}

	void MeshObject::SetIndexBuffer(Ref<Nut::IndexBuffer> indexBuffer)
	{
		m_IndexBuffer = indexBuffer;

		m_IndexBuffer->Bind();
	}

	void MeshObject::SetInstanceBuffer(Ref<VertexBuffer> instanceBuffer)
	{
		m_InstanceBuffer = instanceBuffer;

		m_InstanceBuffer->Bind();

		m_Pipeline->SetInstanceLayout();
	}

	void MeshObject::Bind() const
	{
		RenderThread::Submit([=]()
			{
				glBindVertexArray(m_ID);
			});
	}

	void MeshObject::Unbind() const
	{
		RenderThread::Submit([=]()
			{
				glBindVertexArray(0);
			});
	}



	MeshSource::MeshSource(DataBuffer<ShaderLayoutItem> vertices, const std::vector<uint32_t>& indices, Ref<Pipeline> pipeline, Ref<Scene> scene)
	{
		if (scene != nullptr)
			m_ID = scene->CreateEntity("MeshSource");

		m_Vertices = vertices;
		m_Indices = indices;

		m_MeshObject = CreateRef<MeshObject>(pipeline);

		m_MeshObject->SetVertexBuffer(VertexBuffer::Create(vertices));
		m_MeshObject->SetIndexBuffer(IndexBuffer::Create(indices));
		m_MeshObject->SetInstanceBuffer(VertexBuffer::Create(nullptr, 10000 * sizeof(glm::mat4), BufferUsage::Dynamic));

		m_MeshObject->Unbind();

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
