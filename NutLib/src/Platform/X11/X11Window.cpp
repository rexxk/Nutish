#include "nutpch.h"
#include "X11Window.h"


namespace Nut
{

#ifdef __linux__

    Ref<Window> Window::Create(const WindowProperties& props)
    {
        return CreateRef<X11Window>(props);
    }


    X11Window::X11Window(const WindowProperties& props)
        : m_Properties(props)
    {
        LOG_CORE_TRACE("Creating Linux window");
    
        Create();
    }

    X11Window::~X11Window()
    {

    }

    void X11Window::Create()
    {
        m_Display = XOpenDisplay(NULL);
        NUT_CORE_ASSERT(m_Display, "X11: Cannot connect to X server");

        m_RootWindow = DefaultRootWindow(m_Display);

        int attributeList[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None };

        m_VisualInfo = glXChooseVisual(m_Display, 0, attributeList);
        NUT_CORE_ASSERT(m_VisualInfo, "X11: No appropriate visual found");

    }

    void X11Window::Update()
    {

    }

    void X11Window::Present()
    {

    }

    bool X11Window::VSync() const
    {
        return false;
    }

	void X11Window::EnableVSync(bool value)
    {

    }


#endif

}
