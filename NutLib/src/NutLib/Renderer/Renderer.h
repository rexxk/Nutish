#pragma once



namespace Nut
{

	enum class RendererAPIType
	{
		None,
		OpenGL,
		DirectX11,
		DirectX12,
		Vulkan,
	};


	class Renderer
	{
	public:

		static void SetAPI(RendererAPIType type) { s_API = type; }
		static RendererAPIType API() { return s_API; }

		static void Init();

		static void BeginScene();
		static void EndScene();

		static void Present();

	private:
		virtual void BeginSceneImplementation() = 0;
		virtual void EndSceneImplementation() = 0;

		virtual void PresentImplementation() = 0;

	private:

		static inline RendererAPIType s_API = RendererAPIType::OpenGL;

		static inline Scope<Renderer> s_Renderer = nullptr;
	};



}
