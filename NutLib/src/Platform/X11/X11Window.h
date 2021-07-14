#ifndef __NUTISH_LINUX_LINUXWINDOW_H
#define __NUTISH_LINUX_LINUXWINDOW_H

#include "NutLib/Core/Window.h"

#include "NutLib/Renderer/RenderContext.h"

#ifdef __linux__
#include "X11/Xlib.h"
#include "GL/glx.h"
#endif


namespace Nut
{

#ifdef __linux__
    class X11Window : public Window
    {
    public:
        X11Window(const WindowProperties& props);
        virtual ~X11Window();

		virtual void Update() override;

		virtual void Present() override;

		virtual bool VSync() const override;
		virtual void EnableVSync(bool value) override;

		virtual std::pair<uint32_t, uint32_t> GetWindowSize() override { return std::make_pair(m_Properties.Width, m_Properties.Height); }
		virtual void* GetNativeHandle() override { return m_Window; }

		virtual Ref<RenderContext>& GetRenderContext() override { return m_RenderContext; }

    private:
        void Create();

    private:
        WindowProperties m_Properties;

        void* m_Window;

        Ref<RenderContext> m_RenderContext = nullptr;


        Display* m_Display = nullptr;
        unsigned long m_RootWindow;

        XVisualInfo* m_VisualInfo = nullptr;
    };

#endif

}


#endif
