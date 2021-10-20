#include "nutpch.h"
#include "MeshAsset.h"



namespace Nut
{


	MeshAsset::MeshAsset()
	{


	}

	void MeshAsset::AddSubmesh(const DataBuffer<ShaderLayoutItem>& vertexBuffer, const std::vector<uint32_t>& indexBuffer)
	{
		m_Submeshes.push_back(MeshSource(vertexBuffer, indexBuffer));

	}
}
