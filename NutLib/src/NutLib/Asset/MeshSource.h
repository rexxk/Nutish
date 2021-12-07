#pragma once


#include "Asset.h"

#include "NutLib/Core/DataBuffer.h"

#include "NutLib/Renderer/Shader.h"
#include "NutLib/Renderer/Buffer.h"
#include "NutLib/Renderer/Pipeline.h"

#include "NutLib/Scene/Entity.h"


namespace Nut
{

	class Scene;
//	class Entity;

	class MeshObject
	{
	public:
		MeshObject() = default;
		MeshObject(Ref<Pipeline> pipeline, Ref<Scene> scene);
		~MeshObject();

		void SetVertexBuffer(Ref<VertexBuffer> vertexBuffer);
		void SetIndexBuffer(Ref<IndexBuffer> indexBuffer);
		void SetInstanceBuffer(Ref<VertexBuffer> instanceBuffer);

		Ref<VertexBuffer> InstanceBuffer() { return m_InstanceBuffer; }
		Ref<IndexBuffer> IndexBuffer() { return m_IndexBuffer; }

		void Bind() const;
		void Unbind() const;

		UUID ObjectID() const { return m_Entity.ID(); }

		RendererID ID() const { return m_ID; }

		uint32_t InstanceCount() const { return m_InstanceCount; }
		void SetInstanceCount(uint32_t instanceCount) { m_InstanceCount = instanceCount; }

		uint32_t GetIndexCount() const { return m_IndexBuffer->GetIndexCount(); }

	private:
		RendererID m_ID;

		Entity m_Entity;
		Ref<VertexBuffer> m_VertexBuffer;
		Ref<Nut::IndexBuffer> m_IndexBuffer;
		Ref<VertexBuffer> m_InstanceBuffer;

		Ref<Pipeline> m_Pipeline;

		uint32_t m_InstanceCount = 0;
	};

	class MeshSource : public Asset
	{
	public:
		MeshSource() = default;
		MeshSource(DataBuffer<ShaderLayoutItem> vertices, const std::vector<uint32_t>& indices, Ref<Pipeline> pipeline = nullptr, Ref<Scene> scene = nullptr);
		MeshSource(MeshSource& other)
		{
			m_Vertices = other.m_Vertices;
			m_Indices = other.m_Indices;

			m_MeshObject = other.m_MeshObject;
		}

		MeshSource(MeshSource&& other) noexcept
		{
			m_Vertices = other.m_Vertices;
			m_Indices = other.m_Indices;

			m_MeshObject = other.m_MeshObject;
		}

		MeshSource& operator=(MeshSource& other)
		{
			LOG_CORE_TRACE("MeshAsset: Copy operator");

			return *this;
		}

		MeshSource& operator=(MeshSource&& other) noexcept
		{
			m_Vertices = std::move(other.m_Vertices);
			m_Indices = other.m_Indices;

			m_MeshObject = other.m_MeshObject;

			return *this;
		}

		virtual void Load() override;

//		DataBuffer<ShaderLayoutItem> Vertices() { return m_Vertices; }
		DataBuffer<ShaderLayoutItem>& Vertices() { return m_Vertices; }
		std::vector<uint32_t> Indices() const { return m_Indices; }

		void SetVertexData(const DataBuffer<ShaderLayoutItem>& vertices);
		void SetIndexData(const std::vector<uint32_t>& indices);

//		MeshBuffers GetMeshBuffers() { return m_Buffers; }
		Ref<MeshObject> GetMeshObject() { return m_MeshObject; }

	private:

		DataBuffer<ShaderLayoutItem> m_Vertices;
		std::vector<uint32_t> m_Indices;

		Ref<MeshObject> m_MeshObject;

//		Ref<VertexBuffer> m_VertexBuffer;
//		Ref<IndexBuffer> m_IndexBuffer;

	};



}
