#include "nutpch.h"
#include "OpenGLVertexArray.h"

#include "NutLib/Renderer/RenderCommandQueue.h"

#include <glad/glad.h>


namespace Nut
{

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

	void OpenGLVertexArray::SetBufferLayout()
	{
		RenderCommandQueue::Submit([=]()
			{
				glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (const void*)0);
				glEnableVertexAttribArray(0);
			});
	}

}
