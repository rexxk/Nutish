#include "nutpch.h"
#include "OpenGLVertexArray.h"

#include "NutLib/Renderer/RenderCommandQueue.h"

#include <glad/glad.h>


namespace Nut
{

	GLenum LayoutItemTypeToGLType(OpenGLVertexArray::BufferLayoutItem::LayoutType type)
	{
		switch (type)
		{
			case OpenGLVertexArray::BufferLayoutItem::LayoutType::Bool: return GL_BOOL;
			case OpenGLVertexArray::BufferLayoutItem::LayoutType::Int: return GL_INT;
			case OpenGLVertexArray::BufferLayoutItem::LayoutType::UInt: return GL_UNSIGNED_INT;
			case OpenGLVertexArray::BufferLayoutItem::LayoutType::Float: return GL_FLOAT;
			case OpenGLVertexArray::BufferLayoutItem::LayoutType::Vec2: return GL_FLOAT;
			case OpenGLVertexArray::BufferLayoutItem::LayoutType::Vec3: return GL_FLOAT;
			case OpenGLVertexArray::BufferLayoutItem::LayoutType::Vec4: return GL_FLOAT;
			case OpenGLVertexArray::BufferLayoutItem::LayoutType::Byte4: return GL_UNSIGNED_BYTE;
		}

		return GL_INVALID_ENUM;
	}

	uint32_t LayoutItemTypeCount(OpenGLVertexArray::BufferLayoutItem::LayoutType type)
	{
		switch (type)
		{
			case OpenGLVertexArray::BufferLayoutItem::LayoutType::Bool: return 1;
			case OpenGLVertexArray::BufferLayoutItem::LayoutType::Int: return 1;
			case OpenGLVertexArray::BufferLayoutItem::LayoutType::UInt: return 1;
			case OpenGLVertexArray::BufferLayoutItem::LayoutType::Float: return 1;
			case OpenGLVertexArray::BufferLayoutItem::LayoutType::Vec2: return 2;
			case OpenGLVertexArray::BufferLayoutItem::LayoutType::Vec3: return 3;
			case OpenGLVertexArray::BufferLayoutItem::LayoutType::Vec4: return 4;
			case OpenGLVertexArray::BufferLayoutItem::LayoutType::Byte4: return 4;
		}

		return 0;
	}

	uint32_t LayoutItemTypeSize(OpenGLVertexArray::BufferLayoutItem::LayoutType type)
	{
		switch (type)
		{
			case OpenGLVertexArray::BufferLayoutItem::LayoutType::Bool: return 1;
			case OpenGLVertexArray::BufferLayoutItem::LayoutType::Int: return 4;
			case OpenGLVertexArray::BufferLayoutItem::LayoutType::UInt: return 4;
			case OpenGLVertexArray::BufferLayoutItem::LayoutType::Float: return 4;
			case OpenGLVertexArray::BufferLayoutItem::LayoutType::Vec2: return 4 * 2;
			case OpenGLVertexArray::BufferLayoutItem::LayoutType::Vec3: return 4 * 3;
			case OpenGLVertexArray::BufferLayoutItem::LayoutType::Vec4: return 4 * 4;
			case OpenGLVertexArray::BufferLayoutItem::LayoutType::Byte4: return 1 * 4;
		}

		return 0;
	}


	Ref<OpenGLVertexArray> OpenGLVertexArray::Create()
	{
		return CreateRef<OpenGLVertexArray>();
	}


	OpenGLVertexArray::OpenGLVertexArray()
	{
		RenderCommandQueue::Submit([&]()
			{
				glCreateVertexArrays(1, &m_ID);
			});

	}

	OpenGLVertexArray::~OpenGLVertexArray()
	{

	}

	void OpenGLVertexArray::Bind() const
	{
		RenderCommandQueue::Submit([=]()
			{
				glBindVertexArray(m_ID);
			});
	}

	void OpenGLVertexArray::Unbind() const
	{
		RenderCommandQueue::Submit([]()
			{
				glBindVertexArray(0);
			});
	}

	void OpenGLVertexArray::AttachVertexBuffer(Ref<VertexBuffer> vertexBuffer)
	{
		m_VB = vertexBuffer;
	}

	void OpenGLVertexArray::AttachIndexBuffer(Ref<IndexBuffer> indexBuffer)
	{
		m_IB = indexBuffer;
	}

	void OpenGLVertexArray::SetBufferLayout(std::initializer_list<BufferLayoutItem> layout)
	{
		for (auto& item : layout)
		{
			m_Layout.m_Items.emplace_back(item);
		}

		uint32_t offset = 0;

		for (auto& item : m_Layout.m_Items)
		{
			item.Offset += offset;

			m_Layout.Stride += LayoutItemTypeSize(item.Type);
			offset = m_Layout.Stride;
		}

		RenderCommandQueue::Submit([=]()
			{
				uint32_t location = 0;

				for (auto& item : m_Layout.m_Items)
				{
					glEnableVertexAttribArray(location);

					if (item.Type == BufferLayoutItem::LayoutType::Int || item.Type == BufferLayoutItem::LayoutType::UInt)
						glVertexAttribIPointer(location, LayoutItemTypeCount(item.Type), LayoutItemTypeToGLType(item.Type), m_Layout.Stride, (const void*)(uint64_t)item.Offset);
					else
						glVertexAttribPointer(location, LayoutItemTypeCount(item.Type), LayoutItemTypeToGLType(item.Type), item.Normalized ? GL_TRUE : GL_FALSE, m_Layout.Stride, (const void*)(uint64_t)item.Offset);
				}

				location++;
			});

	}
}

