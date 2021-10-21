#pragma once

#include "MeshSource.h"

#include "NutLib/Renderer/Buffer.h"
#include "NutLib/Renderer/Pipeline.h"


namespace Nut
{

	enum class MeshType
	{
		Static = 0,
		Dynamic,
	};



	class MeshAsset
	{
	public:
		MeshAsset(const DataBuffer<ShaderLayoutItem>& vertexBuffer, const std::vector<uint32_t>& indexBuffer, Ref<Pipeline> pipeline, Ref<Scene> scene);
		MeshAsset()
		{
			LOG_CORE_TRACE("MeshAsset()");
		}
		MeshAsset(MeshAsset& other)
		{
			LOG_CORE_TRACE("MeshAsset() copy");
		}
		MeshAsset(MeshAsset&& other)
		{
			LOG_CORE_TRACE("MeshAsset() move");
		}
		MeshAsset& operator=(MeshAsset& other)
		{
			LOG_CORE_TRACE("MeshAsset operator copy");
			return *this;
		}
		MeshAsset& operator=(MeshAsset&& other)
		{
			m_Type = other.m_Type;
			m_Pipeline = other.m_Pipeline;
			m_Scene = other.m_Scene;

			m_Submeshes = other.m_Submeshes;

			LOG_CORE_TRACE("MeshAsset operator move");
			return *this;
		}


		const uint32_t SubmeshCount() const { return static_cast<uint32_t>(m_Submeshes.size()); }

		std::vector<MeshSource>& Submeshes() { return m_Submeshes; }
		const std::vector<MeshSource>& Submeshes() const { return m_Submeshes; }

		void AddSubmesh(const DataBuffer<ShaderLayoutItem>& vertexBuffer, const std::vector<uint32_t>& indexBuffer);


		MeshType Type() const { return m_Type; }
		void SetType(MeshType type) { m_Type = type; }

		Ref<Pipeline> GetPipeline() const { return m_Pipeline; }

	private:
		MeshType m_Type = MeshType::Static;

		std::vector<MeshSource> m_Submeshes;

		Ref<Pipeline> m_Pipeline;
		Ref<Scene> m_Scene;
	};

}
