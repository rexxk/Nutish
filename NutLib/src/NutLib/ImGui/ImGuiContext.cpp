#include "nutpch.h"
#include "ImGuiContext.h"

#include "NutLib/Renderer/RenderThread.h"
#include "NutLib/Renderer/ShaderStore.h"

#include "NutLib/Event/Events.h"

#include <imgui.h>


namespace Nut
{

	void ImGuiContext::Init()
	{
		if (!s_Instance)
			s_Instance = new ImGuiContext();
	}

	ImGuiContext::ImGuiContext()
	{
		SubscribeToEvent<WindowResizedEvent>([&](WindowResizedEvent& event)
			{
				m_Width = event.Width();
				m_Height = event.Height();

				return false;
			});


		LOG_CORE_TRACE("ImGui: Initializing");

		m_Pipeline = Pipeline::Create(ShaderStore::Get("ImGui"));

		IMGUI_CHECKVERSION();
		ImGui::CreateContext();

		ImGuiIO& io = ImGui::GetIO();

		io.BackendPlatformName = "Nutish";
		io.BackendRendererName = "Nutish-Renderer";
		io.BackendFlags |= ImGuiBackendFlags_RendererHasVtxOffset;

		io.ImeWindowHandle = (HWND)Application::Get().GetWindow()->GetNativeHandle();

		auto [width, height] = Application::Get().GetWindow()->GetWindowSize();
		m_Width = width;
		m_Height = height;

		ImGui::StyleColorsDark();
	}

	ImGuiContext::~ImGuiContext()
	{
		ImGui::DestroyContext();


/*		RenderCommandQueue::Submit([]()
			{

			});
*/
	}

	void ImGuiContext::NewFrame(Timestep ts)
	{
		ImGuiIO& io = ImGui::GetIO();

		io.DisplaySize = ::ImVec2(static_cast<float>(s_Instance->m_Width), static_cast<float>(s_Instance->m_Height));
		io.DeltaTime = ts > 0.0f ? (float)ts : 1.0f / 60.0f;

		ImGui::NewFrame();

	}


}
