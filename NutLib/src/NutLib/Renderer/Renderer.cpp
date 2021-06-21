#include "nutpch.h"
#include "Renderer.h"

#include "Platform/OpenGL/OpenGLRenderer.h"


namespace Nut
{

	void Renderer::Init()
	{
		switch (s_API)
		{
		case RendererAPIType::None: s_Renderer = nullptr; return;
		case RendererAPIType::OpenGL: s_Renderer = CreateScope<OpenGLRenderer>(); return;
		}
	}

	void Renderer::BeginScene()
	{
		s_Renderer->BeginSceneImplementation();
	}

	void Renderer::EndScene()
	{
		s_Renderer->EndSceneImplementation();
	}

}
