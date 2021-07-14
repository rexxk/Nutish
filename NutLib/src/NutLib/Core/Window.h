#ifndef __NUTLIB_WINDOW_H
#define __NUTLIB_WINDOW_H


#include "nutpch.h"

#include "NutLib/Renderer/RenderContext.h"


namespace Nut
{


	struct WindowProperties
	{
		uint32_t Width = 1280;
		uint32_t Height = 720;

		std::string Title = "NutDefault";

		bool VSync = false;
	};

	class Window
	{
	public:
		static Ref<Window> Create(const WindowProperties& props);

		virtual ~Window() {}

		virtual void Update() = 0;

		virtual void Present() = 0;

		virtual bool VSync() const = 0;
		virtual void EnableVSync(bool value) = 0;

		virtual std::pair<uint32_t, uint32_t> GetWindowSize() = 0;

		virtual void* GetNativeHandle() = 0;

		virtual Ref<RenderContext>& GetRenderContext() = 0;
	};


}


#endif
