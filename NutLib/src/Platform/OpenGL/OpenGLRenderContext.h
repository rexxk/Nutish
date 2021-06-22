#pragma once

#include "NutLib/Renderer/RenderContext.h"


namespace Nut
{

	class OpenGLRenderContext : public RenderContext
	{
	public:
		OpenGLRenderContext(void* handle = nullptr);
		virtual ~OpenGLRenderContext();

		virtual void Bind() override;

		virtual const RenderContextCapabilites& GetCapabilites() const override { return m_Capabilities; }
		virtual const RenderContextParameters& GetParameters() const override { return m_Parameters; }

	private:
		RenderContextCapabilites m_Capabilities;
		RenderContextParameters m_Parameters;

		void* m_ContextHandle = nullptr;
	};


}
