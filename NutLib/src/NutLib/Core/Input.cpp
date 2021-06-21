#include "nutpch.h"
#include "Input.h"

#include <NutLib/Event/Events.h>


namespace Nut
{


	void Input::Init()
	{
		if (!s_Instance)
		{
			s_Instance = new Input();
		}

		s_Instance->m_KeyState.resize(256);
		s_Instance->m_ButtonState.resize(20);


		SubscribeToEvent<KeyPressedEvent>([&](KeyPressedEvent& event)
			{
				s_Instance->m_KeyState[event.Key()] = true;

				return false;
			});

		SubscribeToEvent<KeyReleasedEvent>([&](KeyReleasedEvent& event)
			{
				s_Instance->m_KeyState[event.Key()] = false;

				return false;
			});

		SubscribeToEvent<MouseButtonPressed>([&](MouseButtonPressed& event)
			{
				s_Instance->m_ButtonState[event.Button()] = true;

				return false;
			});

		SubscribeToEvent<MouseButtonReleased>([&](MouseButtonReleased& event)
			{
				s_Instance->m_ButtonState[event.Button()] = false;

				return false;
			});
	}


	bool Input::IsKeyPressed(uint32_t key)
	{
		if (key >= 0 && key < 256)
		{
			return s_Instance->m_KeyState[key];
		}

		return false;
	}

	bool Input::IsMouseButtonPressed(uint32_t button)
	{
		if (button >= 0 && button < 20)
		{
			return s_Instance->m_ButtonState[button];
		}

		return false;
	}

}
