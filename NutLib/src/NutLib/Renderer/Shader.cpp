#include "nutpch.h"
#include "Shader.h"

#include "Renderer.h"

#include "Platform/OpenGL/OpenGLShader.h"


namespace Nut
{


	Ref<Shader> Shader::Create(const std::string& shaderFile)
	{
		switch (Renderer::API())
		{
		case RendererAPIType::OpenGL: return CreateRef<OpenGLShader>(shaderFile);
		}

		LOG_CORE_ERROR("Shader: No API defined.");

		return nullptr;
	}


}
