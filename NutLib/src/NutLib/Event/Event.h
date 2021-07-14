#ifndef __NUTLIB_EVENT_H
#define __NUTLIB_EVENT_H

#include "nutpch.h"


namespace Nut
{


	enum class EventType
	{
		WindowClose, WindowResize, WindowMinimize, WindowMaximize, WindowRestore, WindowRender,
		KeyPress, KeyRelease, KeyType,
		MouseButtonPress, MouseButtonRelease, MouseMove, MouseWheel,
		TimerTimeout,
	};




	class Event
	{
	public:
		Event(EventType type)
			: m_Type(type)
		{

		}

		virtual ~Event()
		{

		}

		EventType Type() const { return m_Type; }

		bool Handled = false;

	private:
		EventType m_Type;
	};



}


#endif
