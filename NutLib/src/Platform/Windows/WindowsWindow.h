#pragma once

#include "NutLib/Core/Window.h"

#include "NutLib/Renderer/RenderContext.h"



namespace Nut
{


	class WindowsWindow : public Window
	{
		using PFNWGLSWAPINTERVALEXT = void(*)(int);

	public:
		WindowsWindow(const WindowProperties& props);
		virtual ~WindowsWindow();

		virtual void Update() override;

		virtual bool VSync() const override;
		virtual void EnableVSync(bool value) override;

		virtual std::pair<uint32_t, uint32_t> GetWindowSize() override { return std::make_pair(m_Properties.Width, m_Properties.Height); }
		virtual void* GetNativeHandle() override { return (void*)m_Window; }

	private:
		void Create();

		PFNWGLSWAPINTERVALEXT wglSwapInterval;

	private:
		HWND m_Window = nullptr;

		Ref<RenderContext> m_RenderContext = nullptr;

		WindowProperties m_Properties;
	};


}
