#pragma once

#include "Shader.h"

#include "NutLib/Core/DataBuffer.h"
#include "Buffer.h"

namespace Nut
{

	struct PipelineRenderData
	{
//		static const uint32_t MAX_VERTICES = 30000;
		static const uint32_t MAX_TRIANGLES = 30000;

		Ref<VertexBuffer> BatchVertexBuffer = nullptr;
		Ref<IndexBuffer> BatchIndexBuffer = nullptr;

		DataBuffer<ShaderLayoutItem> VertexData;
		std::vector<uint32_t> IndexData;

		uint32_t IndexCount() const { return static_cast<uint32_t>(IndexData.size()); }
//		PipelineRenderData()
//		{
//			BatchVertexBuffer = VertexBuffer::Create(nullptr, MAX_VERTICES, BufferUsage::Dynamic);
//			BatchIndexBuffer = IndexBuffer::Create(nullptr, MAX_INDICES, BufferUsage::Dynamic);
//		}
	};




	class Pipeline
	{
	public:
		static Ref<Pipeline> Create(Ref<Shader> shader);

		virtual ~Pipeline() {}

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void SetBufferLayout() = 0;

		virtual void Submit(DataBuffer<ShaderLayoutItem>& vertexBuffer, const std::vector<uint32_t>& indexBuffer) = 0;
//		virtual void FlushBatch() = 0;
//		virtual void FlushDirect(DataBuffer<ShaderLayoutItem>& vertexBuffer, const std::vector<uint32_t>& indexBuffer) = 0;
		virtual void Flush() = 0;


		virtual Ref<Shader> GetShader() = 0;

	};

}
