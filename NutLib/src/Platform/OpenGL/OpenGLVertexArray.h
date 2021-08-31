#ifndef __NUTISH_OPENGL_VERTEXARRAY_H
#define __NUTISH_OPENGL_VERTEXARRAY_H


#include "NutLib/Renderer/Shader.h"
#include "OpenGLBuffer.h"


namespace Nut
{

	class OpenGLVertexArray
	{
	public:

		struct BufferLayoutItem
		{
			
			ShaderLayoutDescriptor LayoutDescriptor;

			uint32_t Offset = 0;
			bool Normalized = false;

//			BufferLayoutItem(const std::string& name, LayoutType type, bool normalized = false)
//				: Name(name), Type(type), Normalized(normalized)
			BufferLayoutItem(const ShaderLayoutDescriptor& descriptor)
				: LayoutDescriptor(descriptor)
			{
				
			}

		};

		struct BufferLayout
		{
			std::vector<BufferLayoutItem> m_Items;

			uint32_t Stride = 0;
		};

	public:

		static Ref<OpenGLVertexArray> Create();

		OpenGLVertexArray();
		virtual ~OpenGLVertexArray();

		void Bind() const;
		void Unbind() const;

		void AttachVertexBuffer(Ref<VertexBuffer> vertexBuffer);
		void AttachIndexBuffer(Ref<IndexBuffer> indexBuffer);

		void SetBufferLayout(const std::vector<ShaderLayoutDescriptor>& layout);

		Ref<IndexBuffer> GetIndexBuffer() { return m_IB; }

		RendererID ID() const { return m_ID; }

	private:
		RendererID m_ID = 0;

		Ref<VertexBuffer> m_VB = nullptr;
		Ref<IndexBuffer> m_IB = nullptr;

		BufferLayout m_Layout;
	};

}



#endif

