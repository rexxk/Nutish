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

	OpenGLVertexBuffer::OpenGLVertexBuffer(const DataBuffer<ShaderLayoutItem>& vertexBuffer, BufferUsage usage)
	{
		CreateBuffer(vertexBuffer, usage);
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

	void OpenGLVertexBuffer::CreateBuffer(const DataBuffer<ShaderLayoutItem>& vertexBuffer, BufferUsage usage)
	{
		RendererID& id = m_ID;

		RenderThread::Submit([=, &id]()
			{
				glGenBuffers(1, &id);
				glBindBuffer(GL_ARRAY_BUFFER, id);
				glBufferData(GL_ARRAY_BUFFER, vertexBuffer.Size(), vertexBuffer.Data(), BufferUsageToOpenGLUsage(usage));
			});
	}

	void OpenGLVertexBuffer::SetData(const DataBuffer<ShaderLayoutItem>& dataBuffer)
	{
		RenderThread::Submit([=]()
			{
				glBufferData(m_ID, dataBuffer.Size(), dataBuffer.Data(), GL_STATIC_DRAW);
			});
	}

	void OpenGLVertexBuffer::UpdateData(const DataBuffer<ShaderLayoutItem>& dataBuffer)
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

	OpenGLIndexBuffer::OpenGLIndexBuffer(const std::vector<uint32_t>& indexBuffer, BufferUsage usage)
	{
		CreateBuffer(indexBuffer, usage);
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

	void OpenGLIndexBuffer::CreateBuffer(const std::vector<uint32_t>& indexBuffer, BufferUsage usage)
	{
		m_IndexCount = static_cast<uint32_t>(indexBuffer.size());

		RendererID& id = m_ID;

		RenderThread::Submit([=, &id]()
			{
				glGenBuffers(1, &id);
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexBuffer.size() * sizeof(uint32_t), indexBuffer.data(), BufferUsageToOpenGLUsage(usage));
			});
	}

	void OpenGLIndexBuffer::SetData(const std::vector<uint32_t>& indexBuffer)
	{
		m_IndexCount = static_cast<uint32_t>(indexBuffer.size());

		RenderThread::Submit([=]()
			{
				glBufferData(m_ID, indexBuffer.size() * sizeof(uint32_t), indexBuffer.data(), GL_STATIC_DRAW);
			});
	}

	void OpenGLIndexBuffer::UpdateData(const std::vector<uint32_t>& indexBuffer)
	{
		m_IndexCount = static_cast<uint32_t>(indexBuffer.size());

		RenderThread::Submit([=]()
			{
				glNamedBufferSubData(m_ID, 0, indexBuffer.size() * sizeof(uint32_t), indexBuffer.data());
			});
	}

}
