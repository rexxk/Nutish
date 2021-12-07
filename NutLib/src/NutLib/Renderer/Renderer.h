#ifndef __NUTLIB_RENDERER_H
#define __NUTLIB_RENDERER_H


#include "NutLib/Renderer/Buffer.h"
#include "NutLib/Scene/Model.h"

#include "NutLib/Scene/Entity.h"

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


	struct RenderStats
	{
		uint32_t DrawCalls = 0;

		uint32_t NumVerticesInScene = 0;
	};


	struct RenderData
	{
		std::unordered_map<UUID, std::vector<glm::mat4>> InstanceMap;
		std::unordered_map<UUID, std::vector<Ref<MeshObject>>> MeshObjects;
	};



	class Renderer
	{
	public:

		static void SetAPI(RendererAPIType type) { s_API = type; }
		static RendererAPIType API() { return s_API; }

		static void Init();

		static void BeginScene();
		static void EndScene();

		static void Submit(Ref<MeshAsset> mesh, const glm::mat4& transform);
		static void Draw();

		static RenderStats& GetRenderStats() { return s_Renderer->m_RenderStats; }
		static RenderData& GetRenderData() { return s_Renderer->m_RenderData; }

	private:
		virtual void BeginSceneImplementation() = 0;
		virtual void EndSceneImplementation() = 0;

		virtual void SubmitImplementation(Ref<MeshAsset> mesh, const glm::mat4& transform) = 0;
		virtual void DrawImplementation() = 0;

	private:

		static inline RendererAPIType s_API = RendererAPIType::OpenGL;

		static inline Scope<Renderer> s_Renderer = nullptr;

		RenderStats m_RenderStats;
		RenderData m_RenderData;
	};



}


#endif
