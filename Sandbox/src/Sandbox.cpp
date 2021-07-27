#include <NutLib.h>

#include "Sandbox/MainWindow.h"


class SandboxApp : public Nut::Application
{
public:
	SandboxApp()
	{
		AttachLayer(CreateRef<MainWindow>("MainWindow"));
	}

	virtual ~SandboxApp()
	{
		LOG_CORE_TRACE("Sandbox shutdown");
	}

};





Scope<Nut::Application> Create()
{
	Nut::Renderer::SetAPI(Nut::RendererAPIType::OpenGL);

	return CreateScope<SandboxApp>();
}
