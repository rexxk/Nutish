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

	uint32_t DataTypeToGLSize(DataType type)
	{
		switch (type)
		{
			case DataType::Bool: return 1;
			case DataType::Int: return 4;
			case DataType::UInt: return 4;
			case DataType::Float: return 4;
			case DataType::Double: return 4;
			case DataType::Vec2: return 4 * 2;
			case DataType::Vec3: return 4 * 3;
			case DataType::Vec4: return 4 * 4;
			case DataType::Matrix3x3: return 4;
			case DataType::Matrix4x4: return 4;

			case DataType::Texture2D: return 4;
			case DataType::TextureCube: return 4;
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

		m_RenderData.DirectVertexBuffer = VertexBuffer::Create(nullptr, 0, BufferUsage::Static);
		m_RenderData.DirectIndexBuffer = IndexBuffer::Create(nullptr, 0, BufferUsage::Static);

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
					if (item.Slot == ShaderLayoutItem::ShaderSlot::InstanceMatrix)
						continue;

					glEnableVertexAttribArray(item.Location);

					if (item.Type == DataType::Int || item.Type == DataType::UInt)
						glVertexAttribIPointer(item.Location, DataTypeCount(item.Type), DataTypeToGLType(item.Type), layout.Stride(), (const void*)(uint64_t)item.Offset);
					else
						glVertexAttribPointer(item.Location, DataTypeCount(item.Type), DataTypeToGLType(item.Type), item.Normalized, layout.Stride(), (const void*)(uint64_t)item.Offset);
				}
			});

	}

	void OpenGLPipeline::SetInstanceLayout()
	{
		auto& layout = m_Shader->GetShaderLayout();
		auto& items = layout.Items();


		RenderThread::Submit([=]()
			{
				for (ShaderLayoutItem item : items)
				{
					if (item.Slot == ShaderLayoutItem::ShaderSlot::InstanceMatrix)
					{
						if (item.Type == DataType::Matrix4x4)
						{
							glEnableVertexAttribArray(item.Location);
							glEnableVertexAttribArray(item.Location + 1);
							glEnableVertexAttribArray(item.Location + 2);
							glEnableVertexAttribArray(item.Location + 3);

							// TODO: Fix hardcode
							glVertexAttribPointer(item.Location, 4, GL_FLOAT, GL_FALSE, 4 * 4 * 4, (const void*)(uint64_t)0);
							glVertexAttribPointer(item.Location + 1, 4, GL_FLOAT, GL_FALSE, 4 * 4 * 4, (const void*)(uint64_t)(1 * 4 * 4));
							glVertexAttribPointer(item.Location + 2, 4, GL_FLOAT, GL_FALSE, 4 * 4 * 4, (const void*)(uint64_t)(2 * 4 * 4));
							glVertexAttribPointer(item.Location + 3, 4, GL_FLOAT, GL_FALSE, 4 * 4 * 4, (const void*)(uint64_t)(3 * 4 * 4));

//							glVertexAttribPointer(item.Location, DataTypeToGLSize(item.Type), DataTypeToGLType(item.Type), item.Normalized, layout.Stride(), (const void*)(uint64_t)item.Offset);
//							glVertexAttribPointer(item.Location + 1, DataTypeToGLSize(item.Type), DataTypeToGLType(item.Type), item.Normalized, layout.Stride(), (const void*)(uint64_t)(item.Offset + (1 * DataTypeToGLSize(item.Type))));
//							glVertexAttribPointer(item.Location + 2, DataTypeToGLSize(item.Type), DataTypeToGLType(item.Type), item.Normalized, layout.Stride(), (const void*)(uint64_t)(item.Offset + (2 * DataTypeToGLSize(item.Type))));
//							glVertexAttribPointer(item.Location + 3, DataTypeToGLSize(item.Type), DataTypeToGLType(item.Type), item.Normalized, layout.Stride(), (const void*)(uint64_t)(item.Offset + (3 * DataTypeToGLSize(item.Type))));

							glVertexAttribDivisor(item.Location, 1);
							glVertexAttribDivisor(item.Location + 1, 1);
							glVertexAttribDivisor(item.Location + 2, 1);
							glVertexAttribDivisor(item.Location + 3, 1);
						}
					}
				}
			});

	}

	void OpenGLPipeline::Submit(DataBuffer<ShaderLayoutItem>& vertexBuffer, const std::vector<uint32_t>& indexBuffer)
	{
		if (vertexBuffer.Count() > m_RenderData.MAX_VERTICES)
		{
			// Direct output

			LOG_CORE_TRACE("Pipeline::Submit: Direct output, mesh exceeds {0} vertices", m_RenderData.MAX_VERTICES);

			// TODO: Need a batch flush to get correct drawing order? Remove if unnecessary
//			Flush();

/*			m_RenderData.DirectVertexData = vertexBuffer;
			m_RenderData.DirectIndexData = indexBuffer;

			m_RenderData.DirectVertexBuffer->SetData(m_RenderData.DirectVertexData);
			m_RenderData.DirectIndexBuffer->SetData(m_RenderData.DirectIndexData);

			m_RenderData.DirectVertexBuffer->Bind();
			m_RenderData.DirectIndexBuffer->Bind();
*/
//			SetBufferLayout();

			uint32_t indexCount = m_RenderData.DirectIndexBuffer->GetIndexCount();

			RenderThread::Submit([=]()
				{
					glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, nullptr);
				});

//			m_RenderData.DirectIndexBuffer.reset(new OpenGLIndexBuffer(indexBuffer, BufferUsage::Static));

//			m_RenderData.DirectVertexBuffer->Bind();
//			m_RenderData.DirectIndexBuffer->Bind();

//			uint32_t indexCount = m_RenderData.DirectIndexBuffer->GetIndexCount();

//			RenderThread::Submit([=]()
//				{
//					glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, nullptr);
//				});

		}
		else
		{
			// Batched output

			if ((m_RenderData.VertexData.Position() + vertexBuffer.Count()) > m_RenderData.MAX_VERTICES)
			{
				Flush();
			}

			uint32_t indexBase = m_RenderData.VertexData.Position();

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

			for (auto i : indexBuffer)
			{
				m_RenderData.IndexData.push_back(i + indexBase);
			}

		}

	}

	void OpenGLPipeline::Flush()
	{
		m_RenderData.BatchVertexBuffer->Bind();
		m_RenderData.BatchIndexBuffer->Bind();

		m_RenderData.BatchVertexBuffer->UpdateData(m_RenderData.VertexData);
		m_RenderData.BatchIndexBuffer->UpdateData(m_RenderData.IndexData);

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

