#pragma once

#include "NutLib/Core/Timestep.h"
#include "NutLib/Renderer/Shader.h"


namespace Nut
{

	class ImGuiContext
	{
	public:
		static void Init();

		ImGuiContext();
		virtual ~ImGuiContext();

		static void NewFrame(Timestep ts);

	private:
		static inline ImGuiContext* s_Instance = nullptr;

		uint32_t m_Width = 0;
		uint32_t m_Height = 0;


		Ref<Shader> m_Shader;

	};

}
