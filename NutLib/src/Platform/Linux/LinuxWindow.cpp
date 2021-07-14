#include "nutpch.h"
#include "LinuxWindow.h"


namespace Nut
{

    LinuxWindow::LinuxWindow(const WindowProperties& props)
        : m_Properties(props)
    {
        LOG_CORE_TRACE("Creating Linux window");
    }

    LinuxWindow::~LinuxWindow()
    {

    }

}
