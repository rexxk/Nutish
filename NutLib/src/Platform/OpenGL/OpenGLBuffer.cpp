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

		RenderThread::Submit([=, &id]()
			{
				glGenBuffers(1, &id);
				glBindBuffer(GL_ARRAY_BUFFER, id);
				glBufferData(GL_ARRAY_BUFFER, size, data, BufferUsageToOpenGLUsage(usage));
			});
	}

	void OpenGLVertexBuffer::SetData(void* data, uint32_t size)
	{
		RenderThread::Submit([=]()
			{
				glNamedBufferSubData(m_ID, 0, size, data);
			});
	}

	void OpenGLVertexBuffer::SetData(const DataBuffer<ShaderLayoutItem>& dataBuffer)
	{
		RenderThread::Submit([=]()
			{
				glNamedBufferSubData(m_ID, 0, dataBuffer.Size(), dataBuffer.Data());
			});
	}



	OpenGLIndexBuffer::OpenGLIndexBuffer(void* data, uint32_t count, BufferUsage usage)
	{
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

		RenderThread::Submit([=, &id]()
			{
				glGenBuffers(1, &id);
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * count, data, BufferUsageToOpenGLUsage(usage));
			});
	}

	void OpenGLIndexBuffer::SetData(void* data, uint32_t size)
	{
		m_IndexCount = size;

		size <<= 2;

		RenderThread::Submit([=]()
			{
				glNamedBufferSubData(m_ID, 0, size, data);
			});

	}

	void OpenGLIndexBuffer::SetData(const std::vector<uint32_t>& indexBuffer)
	{
		m_IndexCount = indexBuffer.size();

		RenderThread::Submit([=]()
			{
				glNamedBufferSubData(m_ID, 0, indexBuffer.size() * sizeof(uint32_t), indexBuffer.data());
			});
	}

}
