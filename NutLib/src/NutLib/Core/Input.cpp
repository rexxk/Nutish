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

				s_Instance->m_MouseDeltaX = s_Instance->m_CurrentMouseX - event.X();
				s_Instance->m_MouseDeltaY = s_Instance->m_CurrentMouseY - event.Y();

				s_Instance->m_CurrentMouseX = event.X();
				s_Instance->m_CurrentMouseY = event.Y();

				return false;
			});

		SubscribeToEvent<MouseButtonReleased>([&](MouseButtonReleased& event)
			{
				s_Instance->m_ButtonState[event.Button()] = false;

				s_Instance->m_MouseDeltaX = s_Instance->m_CurrentMouseX - event.X();
				s_Instance->m_MouseDeltaY = s_Instance->m_CurrentMouseY - event.Y();

				s_Instance->m_CurrentMouseX = event.X();
				s_Instance->m_CurrentMouseY = event.Y();

				return false;
			});

		SubscribeToEvent<MouseMovedEvent>([&](MouseMovedEvent& event)
			{
				s_Instance->m_MouseDeltaX = s_Instance->m_CurrentMouseX - event.X();
				s_Instance->m_MouseDeltaY = s_Instance->m_CurrentMouseY - event.Y();

				s_Instance->m_CurrentMouseX = event.X();
				s_Instance->m_CurrentMouseY = event.Y();

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

	std::pair<int32_t, int32_t> Input::MouseDelta()
	{
		return std::make_pair(s_Instance->m_MouseDeltaX, s_Instance->m_MouseDeltaY);
	}

	std::pair<int32_t, int32_t> Input::MousePosition()
	{
		return std::make_pair(s_Instance->m_CurrentMouseX, s_Instance->m_CurrentMouseY);
	}



}
