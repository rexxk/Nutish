#pragma once

#include "Event.h"


namespace Nut
{

	class TimerTimeoutEvent : public Event
	{
	public:
		TimerTimeoutEvent(uint64_t id, double timeoutTime)
			: Event(EventType::TimerTimeout), m_Id(id), m_TimeoutTime(timeoutTime)
		{

		}

		uint64_t Id() const { return m_Id; }
		double Timeout() const { return m_TimeoutTime; }

	private:
		uint64_t m_Id;

		double m_TimeoutTime;
	};

}
