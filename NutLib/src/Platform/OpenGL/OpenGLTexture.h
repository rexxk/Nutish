#pragma once

#include "NutLib/Renderer/Texture.h"


namespace Nut
{


	class OpenGLTexture2D : public Texture2D
	{
	public:
		OpenGLTexture2D(const std::string& filepath, TextureFilter filter);
		OpenGLTexture2D(uint8_t* data, uint32_t width, uint32_t height, TextureFilter filter);
		virtual ~OpenGLTexture2D();

		virtual void Bind(uint32_t slot) const override;
		virtual void Unbind(uint32_t slot) const override;

		virtual RendererID ID() const override;

	private:
		void Create(TextureFilter filter);
		void SetData(uint8_t* data, uint32_t width, uint32_t height);

	private:
		RendererID m_ID = 0;
	};


}
