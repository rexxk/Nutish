#include "nutpch.h"
#include "OpenGLPipeline.h"

#include "NutLib/Renderer/RenderCommandQueue.h"

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
		RenderCommandQueue::Submit([]()
			{
//				glBindVertexArray(0);
			});
	}

	void OpenGLPipeline::AttachVertexBuffer(Ref<VertexBuffer> vertexBuffer)
	{
		m_VB = vertexBuffer;
	}

	void OpenGLPipeline::AttachIndexBuffer(Ref<IndexBuffer> indexBuffer)
	{
		m_IB = indexBuffer;
	}

	void OpenGLPipeline::SetBufferLayout()
	{

		auto& layout = m_Shader->GetShaderLayout();
		auto& items = layout.Items();


		RenderCommandQueue::Submit([=]()
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
}

