#include "nutpch.h"
#include "OpenGLPipeline.h"

#include "NutLib/Renderer/RenderThread.h"

#include <glad/glad.h>


namespace Nut
{

	GLenum DataTypeToGLType(DataType type)
	{
		switch (type)
		{
			case DataType::Bool: return GL_BOOL;
			case DataType::Int: return GL_INT;
			case DataType::UInt: return GL_UNSIGNED_INT;
			case DataType::Float: return GL_FLOAT;
			case DataType::Vec2: return GL_FLOAT;
			case DataType::Vec3: return GL_FLOAT;
			case DataType::Vec4: return GL_FLOAT;
		}

		return GL_INVALID_ENUM;
	}

	OpenGLPipeline::OpenGLPipeline(Ref<Shader> shader)
		: m_Shader(shader)
	{
		if (shader == nullptr)
		{
			LOG_CORE_ERROR("Pipeline assigned with empty shader!");
			return;
		}

		m_RenderData.BatchVertexBuffer = VertexBuffer::Create(nullptr, PipelineRenderData::MAX_VERTICES * shader->GetShaderLayout().Stride(), BufferUsage::Dynamic);
		m_RenderData.BatchIndexBuffer = IndexBuffer::Create(nullptr, PipelineRenderData::MAX_TRIANGLES, BufferUsage::Dynamic);

		m_RenderData.VertexData.SetLayout(shader->GetShaderLayout());
		m_RenderData.VertexData.Allocate(m_RenderData.MAX_VERTICES);

	}

	OpenGLPipeline::~OpenGLPipeline()
	{

	}

	void OpenGLPipeline::Bind() const
	{
		m_Shader->Bind();
	}

	void OpenGLPipeline::Unbind() const
	{
		RenderThread::Submit([]()
			{
//				glBindVertexArray(0);
			});
	}

/*	void OpenGLPipeline::AttachVertexBuffer(Ref<VertexBuffer> vertexBuffer)
	{
		m_VB = vertexBuffer;
	}

	void OpenGLPipeline::AttachIndexBuffer(Ref<IndexBuffer> indexBuffer)
	{
		m_IB = indexBuffer;
	}
*/

	void OpenGLPipeline::SetBufferLayout()
	{

		auto& layout = m_Shader->GetShaderLayout();
		auto& items = layout.Items();


		RenderThread::Submit([=]()
			{
				for (ShaderLayoutItem item : items)
				{
					glEnableVertexAttribArray(item.Location);

					if (item.Type == DataType::Int || item.Type == DataType::UInt)
						glVertexAttribIPointer(item.Location, DataTypeCount(item.Type), DataTypeToGLType(item.Type), layout.Stride(), (const void*)(uint64_t)item.Offset);
					else
						glVertexAttribPointer(item.Location, DataTypeCount(item.Type), DataTypeToGLType(item.Type), item.Normalized, layout.Stride(), (const void*)(uint64_t)item.Offset);
				}
			});

	}

	void OpenGLPipeline::Submit(DataBuffer<ShaderLayoutItem>& vertexBuffer, const std::vector<uint32_t>& indexBuffer)
	{
//		LOG_CORE_TRACE("Submitting render data: {0}", (uint64_t)vertexBuffer.Data());
//		m_RenderData.VertexData = vertexBuffer;

		if (vertexBuffer.Count() > m_RenderData.MAX_VERTICES)
		{
			LOG_CORE_TRACE("Pipeline::Submit: Direct output");
		}
		else
		{
//			LOG_CORE_TRACE("Pipeline::Submit: Batched output");

			if ((m_RenderData.VertexData.Position() + vertexBuffer.Count()) > m_RenderData.MAX_VERTICES)
			{
				Flush();
			}

			for (uint32_t i = 0; i < vertexBuffer.Count(); i++)
			{
				for (auto& vbItem : vertexBuffer.GetLayout().Items())
				{
					for (auto& item : m_RenderData.VertexData.GetLayout().Items())
					{
						if (vbItem.Slot == item.Slot)
						{
							uint8_t* destination = (uint8_t*)m_RenderData.VertexData.Data() + (m_RenderData.VertexData.Position() * m_RenderData.VertexData.GetLayout().Stride() + item.Offset);
							uint8_t* source = (uint8_t*)vertexBuffer.Data() + i * vertexBuffer.GetLayout().Stride() + vbItem.Offset;

//							LOG_CORE_TRACE("Pipeline::Submit: Type match ({0}) {1}->{2}", vbItem.Name.c_str(), (uint64_t)source, (uint64_t)destination);

							memcpy(destination, source, DataTypeSize(item.Type));

							continue;
						}

					}

				}

				m_RenderData.VertexData.Position()++;

			}

		}

		m_RenderData.IndexData = indexBuffer;
//		LOG_CORE_TRACE("End submitting (copying data) {0} : new vb = {1}", (uint64_t)vertexBuffer.Data(), (uint64_t)m_RenderData.VertexData.Data());
	}

	void OpenGLPipeline::Flush()
	{
		m_RenderData.BatchVertexBuffer->Bind();
		m_RenderData.BatchIndexBuffer->Bind();

//		auto flushVertices = m_RenderData.VertexData;
		auto flushIndices = m_RenderData.IndexData;

//		m_RenderData.BatchVertexBuffer->SetData(m_RenderData.VertexData.Data(), m_RenderData.VertexData.Size());
//		m_RenderData.BatchIndexBuffer->SetData(m_RenderData.IndexData.data(), static_cast<uint32_t>(m_RenderData.IndexData.size()));

//		m_RenderData.BatchVertexBuffer->SetData(flushVertices.Data(), flushVertices.Size());
		m_RenderData.BatchVertexBuffer->SetData(m_RenderData.VertexData);
		m_RenderData.BatchIndexBuffer->SetData(flushIndices.data(), static_cast<uint32_t>(flushIndices.size()));

		SetBufferLayout();

		uint32_t indexCount = m_RenderData.BatchIndexBuffer->GetIndexCount();

//		LOG_CORE_TRACE("Flushing pipeline: {0} indices", indexCount);

		RenderThread::Submit([=]()
			{
				glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, nullptr);
			});

		m_RenderData.VertexData.Clear();
		m_RenderData.VertexData.Position() = 0;
		m_RenderData.IndexData.clear();

	}

}

