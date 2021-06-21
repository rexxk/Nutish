#include "nutpch.h"
#include "Application.h"

#include "Timer.h"
#include "Timestep.h"

#include "NutLib/Renderer/Renderer.h"


namespace Nut
{

	Application::Application()
	{
		if (!s_Instance)
			s_Instance = this;
		else
			return;

		m_EventQueue = CreateRef<EventQueue>();

		Renderer::Init();

		WindowProperties windowProps;
		windowProps.Title = "Nut Sandbox";

		m_Window = Window::Create(windowProps);

		m_IsRunning = true;


		SubscribeToEvent<WindowClosedEvent>([&](WindowClosedEvent& event)
			{
				LOG_CORE_TRACE("Window closed event received");

				m_IsRunning = false;

				return true;
			});
	}

	Application::~Application()
	{

	}

	void Application::Shutdown()
	{

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
					LOG_CORE_TRACE("FPS: {0}, UPS: {1}", fpsCount, upsCount);

					fpsCount = 0;
					upsCount = 0;
				}

				return false;
			});

		SubscribeToEvent<TimerTimeoutEvent>([&](TimerTimeoutEvent& event)
			{
				if (event.Id() == updateTimer.Id())
				{
					timestep.Update(event.Timeout());

					for (auto& layer : m_LayerStack)
					{
						layer->OnUpdate(timestep);
					}

					upsCount++;
				}

				return false;
			});

		m_Window->EnableVSync(false);

		while (m_IsRunning)
		{
			m_Window->Update();

			while (m_EventQueue->Size() > 0)
			{
				ProcessEvent(m_EventQueue->GetEvent());
			}

			for (auto& layer : m_LayerStack)
			{
				layer->OnRender();
			}

			fpsCount++;
		}
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
