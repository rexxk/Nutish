#include "nutpch.h"
#include "Entrypoint.h"

#include "Input.h"


int main()
{
	Nut::Log::Init();

	std::unique_ptr<Nut::Application> app = Create();

	Nut::Input::Init();


	app->Run();

	app->Shutdown();

	return 0;
}
