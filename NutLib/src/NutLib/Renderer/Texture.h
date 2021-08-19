#pragma once




namespace Nut
{


	enum TextureFilter
	{
		Linear,
		Nearest,
	};

	enum TextureWrap
	{
		Repeat,
		MirroredRepeat,
		ClampToEdge,
		ClampToBorder,
	};
	
	class Texture2D
	{
	public:
		static Ref<Texture2D> Create(const std::string& filepath, TextureFilter filter = TextureFilter::Linear);
		static Ref<Texture2D> Create(uint8_t* data, uint32_t width, uint32_t height, TextureFilter filter = TextureFilter::Linear);

		virtual void Bind(uint32_t slot) const = 0;
		virtual void Unbind(uint32_t slot) const = 0;

		virtual RendererID ID() const = 0;
	};


}
