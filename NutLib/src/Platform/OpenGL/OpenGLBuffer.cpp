#include "nutpch.h"
#include "OpenGLBuffer.h"

#include "NutLib/Renderer/RenderThread.h"

#include <glad/glad.h>


namespace Nut
{

	GLenum BufferUsageToOpenGLUsage(BufferUsage usage)
	{
		switch (usage)
		{
		case BufferUsage::Static: return GL_STATIC_DRAW;
		case BufferUsage::Dynamic: return GL_DYNAMIC_DRAW;
		case BufferUsage::Stream: return GL_STREAM_DRAW;
		}

		return 0;
	}



	OpenGLVertexBuffer::OpenGLVertexBuffer(void* data, uint32_t size, BufferUsage usage)
	{
		LOG_CORE_TRACE("Creating OpenGL vertex buffer");

		CreateBuffer(data, size, usage);
	}

	OpenGLVertexBuffer::~OpenGLVertexBuffer()
	{
		if (m_ID != 0)
		{
			RenderThread::Submit([=]()
				{
					glDeleteBuffers(1, &m_ID);
				});
		}
	}

	void OpenGLVertexBuffer::Bind() const
	{
		RenderThread::Submit([=]()
			{
				glBindBuffer(GL_ARRAY_BUFFER, m_ID);
			});
	}

	void OpenGLVertexBuffer::Unbind() const
	{
		RenderThread::Submit([]()
			{
				glBindBuffer(GL_ARRAY_BUFFER, 0);
			});
	}

	void OpenGLVertexBuffer::CreateBuffer(void* data, uint32_t size, BufferUsage usage)
	{
		RendererID& id = m_ID;
		std::vector<float> vec(size / sizeof(float));
		memcpy(vec.data(), data, size);

		RenderThread::Submit([=, &id]()
			{
				glGenBuffers(1, &id);
				glBindBuffer(GL_ARRAY_BUFFER, id);
				glBufferData(GL_ARRAY_BUFFER, size, vec.data(), BufferUsageToOpenGLUsage(usage));
//				glNamedBufferData(id, size, vec.data(), GL_STATIC_DRAW);
			});
	}




	OpenGLIndexBuffer::OpenGLIndexBuffer(void* data, uint32_t count, BufferUsage usage)
	{
		LOG_CORE_TRACE("Creating OpenGL index buffer");

		CreateBuffer(data, count, usage);
	}

	OpenGLIndexBuffer::~OpenGLIndexBuffer()
	{
		if (m_ID != 0)
		{
			RenderThread::Submit([=]()
				{
					glDeleteBuffers(1, &m_ID);
				});
		}
	}

	void OpenGLIndexBuffer::Bind() const
	{
		RenderThread::Submit([=]()
			{
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID);
			});
	}

	void OpenGLIndexBuffer::Unbind() const
	{
		RenderThread::Submit([]()
			{
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
			});
	}

	void OpenGLIndexBuffer::CreateBuffer(void* data, uint32_t count, BufferUsage usage)
	{
		m_IndexCount = count;

		RendererID& id = m_ID;
		std::vector<uint32_t> vec(count);
		memcpy(vec.data(), data, sizeof(uint32_t) * count);

		RenderThread::Submit([=, &id]()
			{
				glGenBuffers(1, &id);
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * count, vec.data(), BufferUsageToOpenGLUsage(usage));
//				glNamedBufferData(id, sizeof(uint32_t) * count, vec.data(), GL_STATIC_DRAW);
			});
	}
}
