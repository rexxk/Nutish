#include "nutpch.h"
#include "OpenGLBuffer.h"

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
			glDeleteBuffers(1, &m_ID);
		}
	}

	void OpenGLVertexBuffer::Bind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_ID);
	}

	void OpenGLVertexBuffer::Unbind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void OpenGLVertexBuffer::CreateBuffer(void* data, uint32_t size)
	{
		glGenBuffers(1, &m_ID);
		glNamedBufferData(m_ID, size, data, GL_STATIC_DRAW);
	}




	OpenGLIndexBuffer::OpenGLIndexBuffer(void* data, uint32_t count)
	{
		LOG_CORE_TRACE("Creating OpenGL index buffer");
	}

	OpenGLIndexBuffer::~OpenGLIndexBuffer()
	{
		if (m_ID != 0)
		{
			glDeleteBuffers(1, &m_ID);
		}
	}

	void OpenGLIndexBuffer::Bind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID);
	}

	void OpenGLIndexBuffer::Unbind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	void OpenGLIndexBuffer::CreateBuffer(void* data, uint32_t count)
	{
		m_IndexCount = count;

		glGenBuffers(1, &m_ID);
		glNamedBufferData(m_ID, sizeof(uint32_t) * count, data, GL_STATIC_DRAW);
	}
}
