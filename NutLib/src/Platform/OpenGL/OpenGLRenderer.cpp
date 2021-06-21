#include "nutpch.h"
#include "OpenGLRenderer.h"

#include "NutLib/Core/Application.h"

#include <glad/glad.h>


namespace Nut
{

	OpenGLRenderer::OpenGLRenderer()
	{
		LOG_CORE_TRACE("Creating OpenGL Renderer");
	}

	OpenGLRenderer::~OpenGLRenderer()
	{

	}

	void OpenGLRenderer::BeginSceneImplementation()
	{
		glClearColor(1.0f, 0.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
	}

	void OpenGLRenderer::EndSceneImplementation()
	{

#ifdef _WIN32
		SwapBuffers(GetDC(static_cast<HWND>(Application::Get().GetWindow()->GetNativeHandle())));
#endif

	}

}
