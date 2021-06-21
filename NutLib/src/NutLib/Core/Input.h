#pragma once


namespace Nut
{

	class Input
	{
	public:
		static void Init();

		static bool IsKeyPressed(uint32_t key);
		static bool IsMouseButtonPressed(uint32_t button);

	private:
		static inline Input* s_Instance = nullptr;

		std::vector<bool> m_KeyState;
		std::vector<bool> m_ButtonState;

	};

}
