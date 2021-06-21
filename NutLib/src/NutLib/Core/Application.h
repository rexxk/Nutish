#pragma once

#include "LayerStack.h"
#include "Window.h"

#include "NutLib/Event/Events.h"


namespace Nut
{


	class Application
	{
	public:
		Application();
		virtual ~Application();

		void Shutdown();

		void Run();

		void AttachLayer(const Ref<Layer>& layer);
		void DetachLayer(const Ref<Layer>& layer);
		void AttachOverlay(const Ref<Layer>& overlay);
		void DetachOverlay(const Ref<Layer>& overlay);


		Ref<EventQueue> GetEventQueue() { return m_EventQueue; }

		Ref<Window>& GetWindow() { return m_Window; }

		static Application& Get() { return *s_Instance; }

	private:
		static inline Application* s_Instance = nullptr;

		bool m_IsRunning = false;

		LayerStack m_LayerStack;

		Ref<Window> m_Window = nullptr;
		Ref<EventQueue> m_EventQueue = nullptr;
	};


}
