#ifndef __NUTLIB_OPENGL_BUFFER_H
#define __NUTLIB_OPENGL_BUFFER_H

#include "NutLib/Renderer/Buffer.h"



namespace Nut
{

	class OpenGLVertexBuffer : public VertexBuffer
	{
	public:
		OpenGLVertexBuffer(void* data, uint32_t size, BufferUsage usage);
		virtual ~OpenGLVertexBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void SetData(void* data, uint32_t size) override;

		virtual RendererID ID() const override { return m_ID; }

	private:
		void CreateBuffer(void* data, uint32_t size, BufferUsage usage);

	private:
		RendererID m_ID = 0;
	};


	class OpenGLIndexBuffer : public IndexBuffer
	{
	public:
		OpenGLIndexBuffer(void* data, uint32_t count, BufferUsage usage);
		virtual ~OpenGLIndexBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void SetData(void* data, uint32_t size) override;

		virtual uint32_t GetIndexCount() const override { return m_IndexCount; }

		virtual RendererID ID() const override { return m_ID; }

	private:
		void CreateBuffer(void* data, uint32_t count, BufferUsage usage);

	private:
		RendererID m_ID = 0;

		uint32_t m_IndexCount = 0;
	};

}


#endif
