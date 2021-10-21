#ifndef __NUTLIB_WINDOWEVENTS_H
#define __NUTLIB_WINDOWEVENTS_H


#include "Event.h"


namespace Nut
{


	class WindowClosedEvent : public Event
	{
	public:
		WindowClosedEvent(void* windowHandle)
			: Event(EventType::WindowClose), m_WindowHandle(windowHandle)
		{

		}

		void* Handle() { return m_WindowHandle; }
		
	private:
		void* m_WindowHandle;
	};

	class WindowResizedEvent : public Event
	{
	public:
		WindowResizedEvent(uint32_t width, uint32_t height)
			: Event(EventType::WindowResize), m_Width(width), m_Height(height)
		{

		}

		uint32_t Width() const { return m_Width; }
		uint32_t Height() const { return m_Height; }

		std::pair<uint32_t, uint32_t> Size() const { return std::make_pair(m_Width, m_Height); }

	private:
		uint32_t m_Width;
		uint32_t m_Height;
	};

	class WindowRenderEvent : public Event
	{
	public:
		WindowRenderEvent()
			: Event(EventType::WindowRender)
		{

		}
	};

}


#endif
