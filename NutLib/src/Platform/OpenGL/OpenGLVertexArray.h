#ifndef __NUTISH_OPENGL_VERTEXARRAY_H
#define __NUTISH_OPENGL_VERTEXARRAY_H


#include "OpenGLBuffer.h"


namespace Nut
{

	class OpenGLVertexArray
	{
	public:

		static Ref<OpenGLVertexArray> Create();

		OpenGLVertexArray();
		virtual ~OpenGLVertexArray();

		void Bind() const;
		void Unbind() const;

		void AttachVertexBuffer(Ref<VertexBuffer> vertexBuffer);
		void AttachIndexBuffer(Ref<IndexBuffer> indexBuffer);

		void SetBufferLayout();

		RendererID ID() const { return m_ID; }

	private:
		RendererID m_ID = 0;

		Ref<VertexBuffer> m_VB = nullptr;
		Ref<IndexBuffer> m_IB = nullptr;
	};

}



#endif

