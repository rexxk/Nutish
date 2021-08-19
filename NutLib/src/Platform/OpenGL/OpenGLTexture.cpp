#include "nutpch.h"
#include "OpenGLTexture.h"

#include "NutLib/Renderer/RenderCommandQueue.h"

#include <stb_image.h>

#include <glad/glad.h>


namespace Nut
{

	GLint TextureFilterToGLTextureFilter(TextureFilter filter)
	{
		switch (filter)
		{
		case TextureFilter::Linear: return GL_LINEAR;
		case TextureFilter::Nearest: return GL_NEAREST;
		}

		return 0;
	}

	GLint TextureWrapToGLTextureWrap(TextureWrap wrap)
	{
		switch (wrap)
		{
		case TextureWrap::Repeat: return GL_REPEAT;
		case TextureWrap::MirroredRepeat: return GL_MIRRORED_REPEAT;
		case TextureWrap::ClampToEdge: return GL_CLAMP_TO_EDGE;
		case TextureWrap::ClampToBorder: return GL_CLAMP_TO_BORDER;
		}

		return 0;
	}


	OpenGLTexture2D::OpenGLTexture2D(const std::string& filepath, TextureFilter filter)
	{
		Create(filter);

		int32_t width;
		int32_t height;
		int32_t channels;

		stbi_set_flip_vertically_on_load(true);

		uint8_t* pixels = stbi_load(filepath.c_str(), &width, &height, &channels, 0);


		if (pixels == nullptr)
		{
			LOG_CORE_WARN("Texture: could not get data from file {0}", filepath.c_str());
		}

		if (channels != 4)
		{
			LOG_CORE_WARN("Texture: image is not 4 channels");
			return;
		}

		SetData(pixels, static_cast<uint32_t>(width), static_cast<uint32_t>(height));

		stbi_image_free(pixels);
	}

	OpenGLTexture2D::OpenGLTexture2D(uint8_t* data, uint32_t width, uint32_t height, TextureFilter filter)
	{
		Create(filter);
		SetData(data, 1, 1);
	}

	OpenGLTexture2D::~OpenGLTexture2D()
	{
		RendererID& id = m_ID;

		RenderCommandQueue::Submit([&]()
			{
				glDeleteTextures(1, &id);
			});
	}

	void OpenGLTexture2D::Create(TextureFilter filter)
	{
		RendererID& id = m_ID;

		RenderCommandQueue::Submit([&]()
			{
				glGenTextures(1, &id);
				glBindTexture(GL_TEXTURE_2D, id);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, TextureFilterToGLTextureFilter(filter));
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, TextureFilterToGLTextureFilter(filter));
			});
	}

	void OpenGLTexture2D::SetData(uint8_t* data, uint32_t width, uint32_t height)
	{
		std::vector<uint8_t> vec(width * height * 4);
		memcpy(vec.data(), data, width * height * 4);

		RenderCommandQueue::Submit([=]()
			{
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, vec.data());
				glGenerateMipmap(GL_TEXTURE_2D);
			});
	}

	void OpenGLTexture2D::Bind(uint32_t slot) const
	{
		RenderCommandQueue::Submit([=]()
			{
				glActiveTexture(GL_TEXTURE0 + slot);
				glBindTexture(GL_TEXTURE_2D, m_ID);
			});
	}

	void OpenGLTexture2D::Unbind(uint32_t slot) const
	{
		RenderCommandQueue::Submit([=]()
			{
//				glActiveTexture(GL_TEXTURE0 + slot);
				glBindTextureUnit(GL_TEXTURE_2D, 0);
			});
	}

	RendererID OpenGLTexture2D::ID() const
	{
		return m_ID;
	}

}
