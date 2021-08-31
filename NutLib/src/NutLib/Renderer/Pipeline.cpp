#include "nutpch.h"
#include "Pipeline.h"

#include "Platform/OpenGL/OpenGLPipeline.h"

#include "Renderer.h"



namespace Nut
{

	Ref<Pipeline> Pipeline::Create(Ref<Shader> shader)
	{
		switch (Renderer::API())
		{
		case RendererAPIType::OpenGL: return CreateRef<OpenGLPipeline>(shader);
		}

		LOG_CORE_WARN("[Pipeline]: No api defined.");

		return nullptr;
	}


}
