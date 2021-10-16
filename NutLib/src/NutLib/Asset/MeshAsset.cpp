#include "nutpch.h"
#include "MeshAsset.h"


namespace Nut
{


	MeshAsset::MeshAsset(DataBuffer<ShaderLayoutItem> vertices, const std::vector<uint32_t>& indices)
	{
		m_Vertices = vertices;
		m_Indices = indices;
	}

	void MeshAsset::Load()
	{
		// TODO: Implement MeshAsset::Load()
		LOG_CORE_WARN("MeshAsset::Load is not implemented");
	}


	void MeshAsset::SetVertexData(const DataBuffer<ShaderLayoutItem>& vertices)
	{
		m_Vertices = vertices;
	}

	void MeshAsset::SetIndexData(const std::vector<uint32_t>& indices)
	{
		m_Indices = indices;
	}


}
