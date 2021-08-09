#ifndef __NUTISH_OPENGL_VERTEXARRAY_H
#define __NUTISH_OPENGL_VERTEXARRAY_H


#include "OpenGLBuffer.h"


namespace Nut
{

	class OpenGLVertexArray
	{
	public:

		struct BufferLayoutItem
		{
			enum class LayoutType
			{
				None = 0,
				Bool,
				Int,
				UInt,
				Float,
				Vec2,
				Vec3,
				Vec4,
				Byte4,
			};

			std::string Name = "";
			LayoutType Type = LayoutType::None;
			uint32_t Offset = 0;
			bool Normalized = false;

			BufferLayoutItem(const std::string& name, LayoutType type, bool normalized = false)
				: Name(name), Type(type), Normalized(normalized)
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

		void SetBufferLayout(std::initializer_list<BufferLayoutItem> layout);

		RendererID ID() const { return m_ID; }

	private:
		RendererID m_ID = 0;

		Ref<VertexBuffer> m_VB = nullptr;
		Ref<IndexBuffer> m_IB = nullptr;

		BufferLayout m_Layout;
	};

}



#endif

