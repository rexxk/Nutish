#include "nutpch.h"
#include "Texture.h"

#include "Renderer.h"

#include "Platform/OpenGL/OpenGLTexture.h"


namespace Nut
{


	Ref<Texture2D> Texture2D::Create(const std::string& filepath, TextureFilter filter)
	{
		switch (Renderer::API())
		{
		case RendererAPIType::OpenGL: return CreateRef<OpenGLTexture2D>(filepath, filter);

		default:
			LOG_CORE_ERROR("Texture2D: No valid API");
		}

		return nullptr;
	}


	Ref<Texture2D> Texture2D::Create(uint8_t* data, uint32_t width, uint32_t height, TextureFilter filter)
	{
		switch (Renderer::API())
		{
		case RendererAPIType::OpenGL: return CreateRef<OpenGLTexture2D>(data, width, height, filter);

		default:
			LOG_CORE_ERROR("Texture2D: No valid API");
		}

		return nullptr;
	}


}
