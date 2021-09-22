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
		OpenGLPipeline(Ref<Shader> shader);
		virtual ~OpenGLPipeline();

		virtual void Bind() const override;
		virtual void Unbind() const override;

//		void AttachVertexBuffer(Ref<VertexBuffer> vertexBuffer);
//		void AttachIndexBuffer(Ref<IndexBuffer> indexBuffer);

		virtual void SetBufferLayout() override;

		virtual void Submit(DataBuffer<ShaderLayoutItem>& vertexBuffer, const std::vector<uint32_t>& indexBuffer) override;
//		virtual void FlushBatch() override;
//		virtual void FlushDirect(DataBuffer<ShaderLayoutItem>& vertexBuffer, const std::vector<uint32_t>& indexBuffer) override;
		virtual void Flush() override;

		virtual Ref<Shader> GetShader() override { return m_Shader; }

//		Ref<IndexBuffer> GetIndexBuffer() { return m_IndexBuffer; }

		RendererID ID() const { return m_ID; }

	private:
		RendererID m_ID = 0;

		PipelineRenderData m_RenderData;

		std::vector<float> m_FloatVertex;

		Ref<Shader> m_Shader;

//		BufferLayout m_Layout;
	};

}



#endif

