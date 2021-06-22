#include "nutpch.h"
#include "OpenGLRenderer.h"

#include "NutLib/Core/Application.h"

#include "NutLib/Renderer/RenderCommandQueue.h"


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
		RenderCommandQueue::Submit([]()
			{
				Application::Get().GetWindow()->GetRenderContext()->Bind();

				glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
				glClear(GL_COLOR_BUFFER_BIT);
			});
	}

	void OpenGLRenderer::EndSceneImplementation()
	{


	}

}
