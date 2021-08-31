#include "nutpch.h"
#include "OpenGLPipeline.h"

#include "NutLib/Renderer/RenderCommandQueue.h"

#include <glad/glad.h>


namespace Nut
{

	GLenum LayoutItemTypeToGLType(ShaderLayoutDescriptor::Type type)
	{
		switch (type)
		{
			case ShaderLayoutDescriptor::Type::Bool: return GL_BOOL;
			case ShaderLayoutDescriptor::Type::Int: return GL_INT;
			case ShaderLayoutDescriptor::Type::UInt: return GL_UNSIGNED_INT;
			case ShaderLayoutDescriptor::Type::Float: return GL_FLOAT;
			case ShaderLayoutDescriptor::Type::Vec2: return GL_FLOAT;
			case ShaderLayoutDescriptor::Type::Vec3: return GL_FLOAT;
			case ShaderLayoutDescriptor::Type::Vec4: return GL_FLOAT;
//			case ShaderLayoutDescriptor::Type::Byte4: return GL_UNSIGNED_BYTE;
		}

		return GL_INVALID_ENUM;
	}

	uint32_t LayoutItemTypeCount(ShaderLayoutDescriptor::Type type)
	{
		switch (type)
		{
			case ShaderLayoutDescriptor::Type::Bool: return 1;
			case ShaderLayoutDescriptor::Type::Int: return 1;
			case ShaderLayoutDescriptor::Type::UInt: return 1;
			case ShaderLayoutDescriptor::Type::Float: return 1;
			case ShaderLayoutDescriptor::Type::Vec2: return 2;
			case ShaderLayoutDescriptor::Type::Vec3: return 3;
			case ShaderLayoutDescriptor::Type::Vec4: return 4;
//			case ShaderLayoutDescriptor::Type::Byte4: return 4;
		}

		return 0;
	}

	uint32_t LayoutItemTypeSize(ShaderLayoutDescriptor::Type type)
	{
		switch (type)
		{
			case ShaderLayoutDescriptor::Type::Bool: return 1;
			case ShaderLayoutDescriptor::Type::Int: return 4;
			case ShaderLayoutDescriptor::Type::UInt: return 4;
			case ShaderLayoutDescriptor::Type::Float: return 4;
			case ShaderLayoutDescriptor::Type::Vec2: return 4 * 2;
			case ShaderLayoutDescriptor::Type::Vec3: return 4 * 3;
			case ShaderLayoutDescriptor::Type::Vec4: return 4 * 4;
//			case ShaderLayoutDescriptor::Type::Byte4: return 1 * 4;
		}

		return 0;
	}

	OpenGLPipeline::OpenGLPipeline(Ref<Shader> shader)
		: m_Shader(shader)
	{
		if (shader == nullptr)
		{
			LOG_CORE_ERROR("Pipeline assigned with empty shader!");
			return;
		}

		auto layout = shader->GetShaderLayout();

		for (auto& item : layout)
		{
			m_Layout.m_Items.emplace_back(item);
		}

		CalculateBufferLayout();
	}

	OpenGLPipeline::~OpenGLPipeline()
	{

	}

	void OpenGLPipeline::Bind() const
	{
		m_Shader->Bind();

//		SetBufferLayout();

	/*		RenderCommandQueue::Submit([=]()
			{
//				glBindVertexArray(m_ID);
			});
*/
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

	void OpenGLPipeline::CalculateBufferLayout()
	{
		uint32_t offset = 0;

		for (auto& item : m_Layout.m_Items)
		{
			item.Offset += offset;

			m_Layout.Stride += LayoutItemTypeSize(item.LayoutDescriptor.LayoutType);
			offset = m_Layout.Stride;
		}
	}

	void OpenGLPipeline::SetBufferLayout()
	{

		RenderCommandQueue::Submit([=]()
			{
//				uint32_t location = 0;

				for (auto& item : m_Layout.m_Items)
				{
					glEnableVertexAttribArray(item.LayoutDescriptor.Location);

					if (item.LayoutDescriptor.LayoutType == ShaderLayoutDescriptor::Type::Int || item.LayoutDescriptor.LayoutType == ShaderLayoutDescriptor::Type::UInt)
						glVertexAttribIPointer(item.LayoutDescriptor.Location, LayoutItemTypeCount(item.LayoutDescriptor.LayoutType), LayoutItemTypeToGLType(item.LayoutDescriptor.LayoutType), m_Layout.Stride, (const void*)(uint64_t)item.Offset);
					else
						glVertexAttribPointer(item.LayoutDescriptor.Location, LayoutItemTypeCount(item.LayoutDescriptor.LayoutType), LayoutItemTypeToGLType(item.LayoutDescriptor.LayoutType), item.Normalized ? GL_TRUE : GL_FALSE, m_Layout.Stride, (const void*)(uint64_t)item.Offset);
				}
			});

	}
}

