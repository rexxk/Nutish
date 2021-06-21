#pragma once


#include "Event.h"


namespace Nut
{

	class MouseScrolledEvent : public Event
	{
	public:
		MouseScrolledEvent(int32_t direction, uint32_t x, uint32_t y)
			: Event(EventType::MouseWheel), m_Direction(direction), m_X(x), m_Y(y)
		{

		}

		int32_t Direction() const { return m_Direction; }

		uint32_t ScreenX() const { return m_X; }
		uint32_t ScreenY() const { return m_Y; }

		std::pair<uint32_t, uint32_t> ScreenPosition() const { return std::make_pair(m_X, m_Y); }

	private:
		int32_t m_Direction;

		uint32_t m_X;
		uint32_t m_Y;
	};


	class MouseMovedEvent : public Event
	{
	public:
		MouseMovedEvent(uint32_t x, uint32_t y)
			: Event(EventType::MouseMove), m_X(x), m_Y(y)
		{

		}

		uint32_t X() const { return m_X; }
		uint32_t Y() const { return m_Y; }

		std::pair<uint32_t, uint32_t> Position() const { return std::make_pair(m_X, m_Y); }

	private:
		uint32_t m_X;
		uint32_t m_Y;
	};


	class MouseButtonPressed : public Event
	{
	public:
		MouseButtonPressed(uint32_t button, int16_t x, int16_t y)
			: Event(EventType::MouseButtonPress), m_Button(button), m_X(x), m_Y(y)
		{

		}

		uint32_t Button() const { return m_Button; }

		int16_t X() const { return m_X; }
		int16_t Y() const { return m_Y; }

		std::pair<int16_t, int16_t> Position() const { return std::make_pair(m_X, m_Y); }

	private:
		uint32_t m_Button;

		int16_t m_X;
		int16_t m_Y;
	};

	class MouseButtonReleased : public Event
	{
	public:
		MouseButtonReleased(uint32_t button, int16_t x, int16_t y)
			: Event(EventType::MouseButtonRelease), m_Button(button), m_X(x), m_Y(y)
		{

		}

		uint32_t Button() const { return m_Button; }

		int16_t X() const { return m_X; }
		int16_t Y() const { return m_Y; }

		std::pair<int16_t, int16_t> Position() const { return std::make_pair(m_X, m_Y); }

	private:
		uint32_t m_Button;

		int16_t m_X;
		int16_t m_Y;
	};

}
