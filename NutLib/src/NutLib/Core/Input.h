#ifndef __NUTLIB_INPUT_H
#define __NUTLIB_INPUT_H

#include "NutLib/Core/Keycodes.h"


namespace Nut
{

	class Input
	{
	public:
		static void Init();

		static bool IsKeyPressed(uint32_t key);
		static bool IsMouseButtonPressed(uint32_t button);

		static std::pair<int32_t, int32_t> MouseDelta();
		static std::pair<int32_t, int32_t> MousePosition();

	private:
		static inline Input* s_Instance = nullptr;

		std::vector<bool> m_KeyState;
		std::vector<bool> m_ButtonState;

		int32_t m_CurrentMouseX;
		int32_t m_CurrentMouseY;

		int32_t m_MouseDeltaX;
		int32_t m_MouseDeltaY;
	};

}


#endif
