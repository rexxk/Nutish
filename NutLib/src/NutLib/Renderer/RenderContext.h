#pragma once




namespace Nut
{

	struct RenderContextCapabilites
	{
		std::string ContextVersion;
		std::string Vendor;
		std::string Renderer;
		std::string ShaderVersion;

		uint32_t MaxTextureUnits;

		// TODO: Add more stats/caps here

	};

	struct RenderContextParameters
	{
		bool Wireframe = false;
	};

	class RenderContext
	{
	public:
		static Ref<RenderContext> Create();

		virtual ~RenderContext() {}

		virtual const RenderContextCapabilites& GetCapabilites() const = 0;
		virtual const RenderContextParameters& GetParameters() const = 0;
	};


}
