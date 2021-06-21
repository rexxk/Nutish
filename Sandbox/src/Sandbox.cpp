#include <NutLib.h>

#include "Sandbox/MainWindow.h"


class SandboxApp : public Nut::Application
{
public:
	SandboxApp()
	{
		AttachLayer(CreateRef<MainWindow>("MainWindow"));
	}

	~SandboxApp()
	{

	}

};





Scope<Nut::Application> Create()
{
	Nut::Renderer::SetAPI(Nut::RendererAPIType::OpenGL);

	return CreateScope<SandboxApp>();
}
