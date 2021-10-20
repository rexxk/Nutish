#pragma once


#include "Asset.h"

#include "NutLib/Core/DataBuffer.h"
#include "NutLib/Core/UUID.h"
#include "NutLib/Renderer/Shader.h"


namespace Nut
{



	class Scene;

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
		}

		MeshSource(MeshSource&& other)
		{
			LOG_CORE_TRACE("MeshSource: Move constructor");
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

			return *this;
		}

		virtual void Load() override;

//		DataBuffer<ShaderLayoutItem> Vertices() { return m_Vertices; }
		DataBuffer<ShaderLayoutItem>& Vertices() { return m_Vertices; }
		std::vector<uint32_t> Indices() const { return m_Indices; }

		void SetVertexData(const DataBuffer<ShaderLayoutItem>& vertices);
		void SetIndexData(const std::vector<uint32_t>& indices);

		UUID ID() const { return m_ID; }

	private:


		DataBuffer<ShaderLayoutItem> m_Vertices;
		std::vector<uint32_t> m_Indices;

		UUID m_ID;

//		Ref<VertexBuffer> m_VertexBuffer;
//		Ref<IndexBuffer> m_IndexBuffer;

	};



}
