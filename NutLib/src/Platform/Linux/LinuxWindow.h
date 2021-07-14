#ifndef __NUTISH_LINUX_LINUXWINDOW_H
#define __NUTISH_LINUX_LINUXWINDOW_H

#include "NutLib/Core/Window.h"

#include "NutLib/Renderer/RenderContext.h"


namespace Nut
{

    class LinuxWindow : public Window
    {
    public:
        LinuxWindow(const WindowProperties& props);
        virtual ~LinuxWindow();

		virtual void Update() override;

		virtual void Present() override;

		virtual bool VSync() const override;
		virtual void EnableVSync(bool value) override;

		virtual std::pair<uint32_t, uint32_t> GetWindowSize() override { return std::make_pair(m_Properties.Width, m_Properties.Height); }
		virtual void* GetNativeHandle() override { return m_Window; }

		virtual Ref<RenderContext>& GetRenderContext() override { return m_RenderContext; }

    private:
        WindowProperties m_Properties;

        void* m_Window;

        Ref<RenderContext> m_RenderContext = nullptr;
    }


}


#endif
