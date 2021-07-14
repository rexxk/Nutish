#include "nutpch.h"
#include "X11Window.h"


namespace Nut
{

    X11Window::X11Window(const WindowProperties& props)
        : m_Properties(props)
    {
        LOG_CORE_TRACE("Creating Linux window");
    }

    X11Window::~X11Window()
    {

    }

}
