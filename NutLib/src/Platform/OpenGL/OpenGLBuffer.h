#ifndef __NUTLIB_OPENGL_BUFFER_H
#define __NUTLIB_OPENGL_BUFFER_H

#include "NutLib/Renderer/Buffer.h"



namespace Nut
{

	class OpenGLVertexBuffer : public VertexBuffer
	{
	public:
		OpenGLVertexBuffer(void* data, uint32_t size);
		virtual ~OpenGLVertexBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual RendererID ID() const override { return m_ID; }

	private:
		void CreateBuffer(void* data, uint32_t size);

	private:
		RendererID m_ID = 0;
	};


	class OpenGLIndexBuffer : public IndexBuffer
	{
	public:
		OpenGLIndexBuffer(void* data, uint32_t count);
		virtual ~OpenGLIndexBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual uint32_t GetIndexCount() const override { return m_IndexCount; }

		virtual RendererID ID() const override { return m_ID; }

	private:
		void CreateBuffer(void* data, uint32_t count);

	private:
		RendererID m_ID = 0;

		uint32_t m_IndexCount = 0;
	};

}


#endif
