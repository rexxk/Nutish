#include "nutpch.h"
#include "Application.h"

#include "Timer.h"
#include "Timestep.h"

#include "NutLib/Renderer/Renderer.h"
#include "NutLib/Renderer/RenderCommandQueue.h"


namespace Nut
{

	Application::Application()
	{
		if (!s_Instance)
			s_Instance = this;
		else
			return;

		RenderCommandQueue::Init();

		m_EventQueue = CreateRef<EventQueue>();

		Renderer::Init();

		WindowProperties windowProps;
		windowProps.Title = "Nut Sandbox";

		m_Window = Window::Create(windowProps);

		RenderCommandQueue::Run();

		m_Window->EnableVSync(false);

		m_IsRunning = true;


		SubscribeToEvent<WindowClosedEvent>([&](WindowClosedEvent& event)
			{
				m_IsRunning = false;
				return true;
			});
	}

	Application::~Application()
	{
		Shutdown();
	}

	void Application::Shutdown()
	{
		LOG_CORE_TRACE("Application shutdown");

//		RenderCommandQueue::Shutdown();
	}

	void Application::Run()
	{
		constexpr double updateFps = 60.0;

		Timer runTimer(1.0);
		Timer updateTimer(1.0 / updateFps);

		Timestep timestep;

		uint32_t fpsCount = 0;
		uint32_t upsCount = 0;

		SubscribeToEvent<TimerTimeoutEvent>([&](TimerTimeoutEvent& event)
			{
				if (event.Id() == runTimer.Id())
				{
					LOG_CORE_TRACE("FPS: {0} ({1}), UPS: {2}", RenderCommandQueue::FPS(), fpsCount, upsCount); //fpsCount, upsCount);

					RenderCommandQueue::ResetFPS();
					fpsCount = 0;
					upsCount = 0;

					return true;
				}

				if (event.Id() == updateTimer.Id())
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
			RenderCommandQueue::Execute();

			m_Window->Update();

			while (m_EventQueue->Size() > 0)
			{
				ProcessEvent(m_EventQueue->GetEvent());
			}

			for (auto& layer : m_LayerStack)
			{
				layer->OnRender();
			}

			RenderCommandQueue::Present();

			while (!RenderCommandQueue::IsFrameDone())
			{

			}

			fpsCount++;
		}

		RenderCommandQueue::Stop();
//		RenderCommandQueue::Join();

		while (!RenderCommandQueue::ThreadStopped())
		{
			LOG_CORE_TRACE("Application is waiting for the render thread to stop.");
		}

		LOG_CORE_TRACE("Application knows that the render thread is stopped.");
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
