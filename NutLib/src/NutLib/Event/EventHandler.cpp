#include "nutpch.h"
#include "EventHandler.h"

#include "Events.h"

#include "NutLib/Core/Application.h"


namespace Nut
{

	void ProcessEvent(Ref<Event> event)
	{
		if (event == nullptr)
			return;

		switch (event->Type())
		{
		case EventType::WindowClose: EventCallback<WindowClosedEvent>::Execute(std::dynamic_pointer_cast<WindowClosedEvent>(event)); return;
		case EventType::WindowResize: EventCallback<WindowResizedEvent>::Execute(std::dynamic_pointer_cast<WindowResizedEvent>(event)); return;
		case EventType::MouseWheel: EventCallback<MouseScrolledEvent>::Execute(std::dynamic_pointer_cast<MouseScrolledEvent>(event)); return;
		case EventType::MouseMove: EventCallback<MouseMovedEvent>::Execute(std::dynamic_pointer_cast<MouseMovedEvent>(event)); return;
		case EventType::MouseButtonPress: EventCallback<MouseButtonPressed>::Execute(std::dynamic_pointer_cast<MouseButtonPressed>(event)); return;
		case EventType::MouseButtonRelease: EventCallback<MouseButtonReleased>::Execute(std::dynamic_pointer_cast<MouseButtonReleased>(event)); return;
		case EventType::KeyPress: EventCallback<KeyPressedEvent>::Execute(std::dynamic_pointer_cast<KeyPressedEvent>(event)); return;
		case EventType::KeyRelease: EventCallback<KeyReleasedEvent>::Execute(std::dynamic_pointer_cast<KeyReleasedEvent>(event)); return;
		case EventType::TimerTimeout: EventCallback<TimerTimeoutEvent>::Execute(std::dynamic_pointer_cast<TimerTimeoutEvent>(event)); return;
		}

	}

	void AddEvent(Ref<Event> event)
	{
		Application::Get().GetEventQueue()->AddEvent(event);
	}

	void ExecuteEvent(Ref<Event> event)
	{
		ProcessEvent(event);
	}
}
