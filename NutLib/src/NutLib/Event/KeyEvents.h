#pragma once

#include "Event.h"


namespace Nut
{


	class KeyPressedEvent : public Event
	{
	public:
		KeyPressedEvent(uint32_t key)
			: Event(EventType::KeyPress), m_Key(key)
		{

		}

		uint32_t Key() const { return m_Key; }

	private:
		uint32_t m_Key;
	};

	class KeyReleasedEvent : public Event
	{
	public:
		KeyReleasedEvent(uint32_t key)
			: Event(EventType::KeyRelease), m_Key(key)
		{

		}

		uint32_t Key() const { return m_Key; }

	private:
		uint32_t m_Key;
	};

}
