#ifndef __NUTLIB_RENDERER_H
#define __NUTLIB_RENDERER_H


#include "NutLib/Renderer/Buffer.h"
#include "NutLib/Scene/Model.h"


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


	struct RenderDataStats
	{
		uint32_t DrawCalls = 0;
	};


	struct RenderData
	{
		static const uint32_t MAX_VERTICES = 30000;
		static const uint32_t MAX_INDICES = 10000;

		Ref<VertexBuffer> BatchVertexBuffer = nullptr;
		Ref<IndexBuffer> BatchIndexBuffer = nullptr;

		uint32_t VertexCount = 0;
		uint32_t IndexCount = 0;

		RenderDataStats Statistics;

		const RenderDataStats& GetStats() const { return Statistics; }
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

		static void Submit(Ref<Model> model);
		static void Flush();

		static RenderData& GetRenderData() { return s_Renderer->m_RenderData; }

	private:
		virtual void BeginSceneImplementation() = 0;
		virtual void EndSceneImplementation() = 0;

		virtual void PresentImplementation() = 0;

		virtual void SubmitImplementation(Ref<Model> model) = 0;
		virtual void FlushImplementation() = 0;

	private:

		static inline RendererAPIType s_API = RendererAPIType::OpenGL;

		static inline Scope<Renderer> s_Renderer = nullptr;

		RenderData m_RenderData;
	};



}


#endif
