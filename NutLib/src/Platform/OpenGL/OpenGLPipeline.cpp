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
		m_Shader->Unbind();
	}


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
							// TODO: Fix hardcode

							glEnableVertexAttribArray(item.Location);
							glVertexAttribPointer(item.Location, 4, GL_FLOAT, GL_FALSE, 4 * 4 * 4, (const void*)(uint64_t)0);
							glEnableVertexAttribArray(item.Location + 1);
							glVertexAttribPointer(item.Location + 1, 4, GL_FLOAT, GL_FALSE, 4 * 4 * 4, (const void*)(uint64_t)(1 * 4 * 4));
							glEnableVertexAttribArray(item.Location + 2);
							glVertexAttribPointer(item.Location + 2, 4, GL_FLOAT, GL_FALSE, 4 * 4 * 4, (const void*)(uint64_t)(2 * 4 * 4));
							glEnableVertexAttribArray(item.Location + 3);
							glVertexAttribPointer(item.Location + 3, 4, GL_FLOAT, GL_FALSE, 4 * 4 * 4, (const void*)(uint64_t)(3 * 4 * 4));

							glVertexAttribDivisor(item.Location, 1);
							glVertexAttribDivisor(item.Location + 1, 1);
							glVertexAttribDivisor(item.Location + 2, 1);
							glVertexAttribDivisor(item.Location + 3, 1);

						}

						break;
					}
				}
			});

	}

}

