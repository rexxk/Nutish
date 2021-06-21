#pragma once

#include "NutLib/Renderer/RenderContext.h"


namespace Nut
{

	class OpenGLRenderContext : public RenderContext
	{
	public:
		OpenGLRenderContext();
		virtual ~OpenGLRenderContext();

		const RenderContextCapabilites& GetCapabilites() const { return m_Capabilities; }

	private:
		RenderContextCapabilites m_Capabilities;
	};


}
