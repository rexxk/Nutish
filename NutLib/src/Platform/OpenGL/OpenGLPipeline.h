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

		virtual void SetBufferLayout() override;
		virtual void SetInstanceLayout() override;

		virtual Ref<Shader> GetShader() override { return m_Shader; }

		RendererID ID() const { return m_ID; }

	private:
		RendererID m_ID = 0;
		Ref<Shader> m_Shader;

	};

}



#endif

