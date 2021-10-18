#pragma once


#include "Asset.h"

#include "NutLib/Core/DataBuffer.h"
#include "NutLib/Renderer/Shader.h"


namespace Nut
{


	enum class MeshType
	{
		Static = 0,
		Dynamic,
	};


	class MeshAsset : public Asset
	{
	public:
		MeshAsset() = default;
		MeshAsset(DataBuffer<ShaderLayoutItem> vertices, const std::vector<uint32_t>& indices);
		MeshAsset(MeshAsset& other)
		{
			m_Vertices = other.m_Vertices;
			m_Indices = other.m_Indices;
		}

		MeshAsset(MeshAsset&& other)
		{
			LOG_CORE_TRACE("MeshAsset: Move");
		}

		MeshAsset& operator=(MeshAsset& other)
		{
			LOG_CORE_TRACE("MeshAsset: Copy operator");

			return *this;
		}

		MeshAsset& operator=(MeshAsset&& other)
		{
			m_Vertices = std::move(other.m_Vertices);
			m_Indices = other.m_Indices;

			return *this;
		}

		virtual void Load() override;

//		DataBuffer<ShaderLayoutItem> Vertices() { return m_Vertices; }
		DataBuffer<ShaderLayoutItem>& Vertices() { return m_Vertices; }
		std::vector<uint32_t> Indices() const { return m_Indices; }

		void SetVertexData(const DataBuffer<ShaderLayoutItem>& vertices);
		void SetIndexData(const std::vector<uint32_t>& indices);

		MeshType Type() const { return m_Type; }
		void SetType(MeshType type) { m_Type = type; }

	private:

		MeshType m_Type = MeshType::Static;

		DataBuffer<ShaderLayoutItem> m_Vertices;
		std::vector<uint32_t> m_Indices;

	};



}
