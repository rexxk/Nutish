#include "nutpch.h"
#include "OpenGLBuffer.h"

#include "NutLib/Renderer/RenderCommandQueue.h"

#include <glad/glad.h>


namespace Nut
{

	OpenGLVertexBuffer::OpenGLVertexBuffer(void* data, uint32_t size)
	{
		LOG_CORE_TRACE("Creating OpenGL vertex buffer");

		CreateBuffer(data, size);
	}

	OpenGLVertexBuffer::~OpenGLVertexBuffer()
	{
		if (m_ID != 0)
		{
			RenderCommandQueue::Submit([=]()
				{
					glDeleteBuffers(1, &m_ID);
				});
		}
	}

	void OpenGLVertexBuffer::Bind() const
	{
		RenderCommandQueue::Submit([=]()
			{
				glBindBuffer(GL_ARRAY_BUFFER, m_ID);
			});
	}

	void OpenGLVertexBuffer::Unbind() const
	{
		RenderCommandQueue::Submit([]()
			{
				glBindBuffer(GL_ARRAY_BUFFER, 0);
			});
	}

	void OpenGLVertexBuffer::CreateBuffer(void* data, uint32_t size)
	{
		RendererID& id = m_ID;
		std::vector<float> vec(size / sizeof(float));
		memcpy(vec.data(), data, size);

		RenderCommandQueue::Submit([=, &id]()
			{
				glGenBuffers(1, &id);
				glBindBuffer(GL_ARRAY_BUFFER, id);
				glBufferData(GL_ARRAY_BUFFER, size, vec.data(), GL_STATIC_DRAW);
//				glNamedBufferData(id, size, vec.data(), GL_STATIC_DRAW);
			});
	}




	OpenGLIndexBuffer::OpenGLIndexBuffer(void* data, uint32_t count)
	{
		LOG_CORE_TRACE("Creating OpenGL index buffer");

		CreateBuffer(data, count);
	}

	OpenGLIndexBuffer::~OpenGLIndexBuffer()
	{
		if (m_ID != 0)
		{
			RenderCommandQueue::Submit([=]()
				{
					glDeleteBuffers(1, &m_ID);
				});
		}
	}

	void OpenGLIndexBuffer::Bind() const
	{
		RenderCommandQueue::Submit([=]()
			{
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID);
			});
	}

	void OpenGLIndexBuffer::Unbind() const
	{
		RenderCommandQueue::Submit([]()
			{
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
			});
	}

	void OpenGLIndexBuffer::CreateBuffer(void* data, uint32_t count)
	{
		m_IndexCount = count;

		RendererID& id = m_ID;
		std::vector<uint32_t> vec(count);
		memcpy(vec.data(), data, sizeof(uint32_t) * count);

		RenderCommandQueue::Submit([=, &id]()
			{
				glGenBuffers(1, &id);
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * count, vec.data(), GL_STATIC_DRAW);
//				glNamedBufferData(id, sizeof(uint32_t) * count, vec.data(), GL_STATIC_DRAW);
			});
	}
}
