#ifndef __NUTISH_OPENGL_VERTEXARRAY_H
#define __NUTISH_OPENGL_VERTEXARRAY_H


#include "NutLib/Renderer/Shader.h"
#include "NutLib/Renderer/Pipeline.h"
#include "OpenGLBuffer.h"


namespace Nut
{

	class OpenGLPipeline : public Pipeline
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

		OpenGLPipeline(Ref<Shader> shader);
		virtual ~OpenGLPipeline();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		void AttachVertexBuffer(Ref<VertexBuffer> vertexBuffer);
		void AttachIndexBuffer(Ref<IndexBuffer> indexBuffer);

		virtual void SetBufferLayout() override;

		Ref<IndexBuffer> GetIndexBuffer() { return m_IB; }

		RendererID ID() const { return m_ID; }

	private:
		void CalculateBufferLayout();

	private:
		RendererID m_ID = 0;

		Ref<VertexBuffer> m_VB = nullptr;
		Ref<IndexBuffer> m_IB = nullptr;

		Ref<Shader> m_Shader;

		BufferLayout m_Layout;
	};

}



#endif

