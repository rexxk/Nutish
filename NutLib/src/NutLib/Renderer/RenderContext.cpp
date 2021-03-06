#include "nutpch.h"
#include "RenderContext.h"
#include "Renderer.h"

#include "Platform/OpenGL/OpenGLRenderContext.h"


namespace Nut
{


	Ref<RenderContext> RenderContext::Create(void* handle)
	{
		switch (Renderer::API())
		{
		case RendererAPIType::None: LOG_CORE_ERROR("No renderer API defined."); return nullptr;
		case RendererAPIType::OpenGL: return CreateRef<OpenGLRenderContext>(handle);
		}

		return nullptr;
	}


}
