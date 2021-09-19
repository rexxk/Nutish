#include "nutpch.h"
#include "Application.h"

#include "Timer.h"
#include "Timestep.h"

#include "NutLib/Renderer/Renderer.h"
#include "NutLib/Renderer/RenderThread.h"

#include "NutLib/Renderer/ShaderStore.h"

#include "NutLib/ImGui/ImGuiContext.h"



namespace Nut
{

	Application::Application()
	{
		if (!s_Instance)
			s_Instance = this;
		else
			return;

		RenderThread::Init();

		m_EventQueue = CreateRef<EventQueue>();

		Renderer::Init();

		WindowProperties windowProps;
		windowProps.Title = "Nut Sandbox";

		m_Window = Window::Create(windowProps);

		RenderThread::Run();

		m_Window->EnableVSync(false);

		ShaderStore::Add("ImGui", Shader::Create("assets/shaders/ImGui.shader"));

		ImGuiContext::Init();

		m_IsRunning = true;

		SubscribeToEvent<WindowClosedEvent>([&](WindowClosedEvent& event)
			{
				m_IsRunning = false;
				return true;
			});
	}

	Application::~Application()
	{
		m_LayerStack.~LayerStack();

		LOG_CORE_TRACE("Application destructor");
		Shutdown();
	}

	void Application::Shutdown()
	{
		LOG_CORE_TRACE("Application shutdown");

		RenderThread::Stop();

		while (!RenderThread::ThreadStopped())
		{

		}

		RenderThread::Shutdown();
	}

	void Application::Run()
	{
		constexpr double updateFps = 60.0;

		Ref<Timer> runTimer = CreateRef<Timer>(1.0);
		Ref<Timer> updateTimer = CreateRef<Timer>(1.0 / updateFps);

		Timestep timestep;

		uint32_t fpsCount = 0;
		uint32_t upsCount = 0;

		SubscribeToEvent<TimerTimeoutEvent>([&](TimerTimeoutEvent& event)
			{
				if (event.Id() == runTimer->Id())
				{
					LOG_CORE_TRACE("FPS: {0} ({1}), UPS: {2}", RenderThread::FPS(), fpsCount, upsCount); //fpsCount, upsCount);

					RenderThread::ResetFPS();
					fpsCount = 0;
					upsCount = 0;

					return true;
				}

				if (event.Id() == updateTimer->Id())
				{
					timestep.Update(event.Timeout());

					for (auto& layer : m_LayerStack)
					{
						layer->OnUpdate(timestep);
					}

					upsCount++;

					return true;
				}

				return false;
			});


		while (m_IsRunning)
		{
			RenderThread::Execute();

			m_Window->Update();

			while (m_EventQueue->Size() > 0)
			{
				ProcessEvent(m_EventQueue->GetEvent());
			}

			for (auto& layer : m_LayerStack)
			{
				layer->OnRender();
			}

			RenderThread::Present();

			while (!RenderThread::IsFrameDone())
			{

			}

			fpsCount++;
		}

		runTimer->Stop();
		updateTimer->Stop();
	}

	void Application::AttachLayer(const Ref<Layer>& layer)
	{
		m_LayerStack.AttachLayer(layer);
	}

	void Application::DetachLayer(const Ref<Layer>& layer)
	{
		m_LayerStack.DetachLayer(layer);
	}

	void Application::AttachOverlay(const Ref<Layer>& overlay)
	{
		m_LayerStack.AttachOverlay(overlay);
	}

	void Application::DetachOverlay(const Ref<Layer>& overlay)
	{
		m_LayerStack.DetachOverlay(overlay);
	}

}
