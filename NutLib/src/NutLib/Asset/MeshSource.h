#pragma once


#include "Asset.h"

#include "NutLib/Core/DataBuffer.h"
#include "NutLib/Core/UUID.h"
#include "NutLib/Renderer/Shader.h"
#include "NutLib/Renderer/Buffer.h"


namespace Nut
{



	class Scene;

	struct MeshBuffers
	{
		UUID ID;
		Ref<Nut::VertexBuffer> VertexBuffer;
		Ref<Nut::IndexBuffer> IndexBuffer;
		Ref<Nut::VertexBuffer> InstanceBuffer;
		uint32_t Instances = 0;
	};

	class MeshSource : public Asset
	{
	public:
		MeshSource() = default;
		MeshSource(DataBuffer<ShaderLayoutItem> vertices, const std::vector<uint32_t>& indices, Ref<Scene> scene = nullptr);
		MeshSource(MeshSource& other)
		{
			m_Vertices = other.m_Vertices;
			m_Indices = other.m_Indices;

			m_ID = other.m_ID;

			m_Buffers = other.m_Buffers;
		}

		MeshSource(MeshSource&& other)
		{
			m_Vertices = other.m_Vertices;
			m_Indices = other.m_Indices;

			m_ID = other.m_ID;

			m_Buffers = other.m_Buffers;
		}

		MeshSource& operator=(MeshSource& other)
		{
			LOG_CORE_TRACE("MeshAsset: Copy operator");

			return *this;
		}

		MeshSource& operator=(MeshSource&& other)
		{
			m_Vertices = std::move(other.m_Vertices);
			m_Indices = other.m_Indices;

			m_ID = other.m_ID;

			m_Buffers = other.m_Buffers;

			return *this;
		}

		virtual void Load() override;

//		DataBuffer<ShaderLayoutItem> Vertices() { return m_Vertices; }
		DataBuffer<ShaderLayoutItem>& Vertices() { return m_Vertices; }
		std::vector<uint32_t> Indices() const { return m_Indices; }

		void SetVertexData(const DataBuffer<ShaderLayoutItem>& vertices);
		void SetIndexData(const std::vector<uint32_t>& indices);

		MeshBuffers GetMeshBuffers() { return m_Buffers; }
//		MeshBuffers& GetMeshBuffers() { return m_Buffers; }

		UUID ID() const { return m_ID; }

	private:


		DataBuffer<ShaderLayoutItem> m_Vertices;
		std::vector<uint32_t> m_Indices;

		UUID m_ID;

		MeshBuffers m_Buffers;

//		Ref<VertexBuffer> m_VertexBuffer;
//		Ref<IndexBuffer> m_IndexBuffer;

	};



}
