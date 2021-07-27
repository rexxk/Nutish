#ifndef __NUTLIB_OPENGL_RENDERCONTEXT_H
#define __NUTLIB_OPENGL_RENDERCONTEXT_H

#include "NutLib/Renderer/RenderContext.h"


namespace Nut
{

	class OpenGLRenderContext : public RenderContext
	{
	public:
		OpenGLRenderContext(void* handle = nullptr);
		virtual ~OpenGLRenderContext();

		virtual void Bind() override;
		virtual void Unbind() override;

		virtual const RenderContextCapabilites& GetCapabilites() const override { return m_Capabilities; }
		virtual const RenderContextParameters& GetParameters() const override { return m_Parameters; }

	private:
		RenderContextCapabilites m_Capabilities;
		RenderContextParameters m_Parameters;

		void* m_ContextHandle = nullptr;
	};


}


#endif
