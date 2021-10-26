#include "nutpch.h"
#include "OpenGLRenderContext.h"

#include "NutLib/Core/Application.h"
#include "NutLib/Renderer/RenderThread.h"

#include <glad/glad.h>



namespace Nut
{

	OpenGLRenderContext::OpenGLRenderContext(void* handle)
		: m_ContextHandle(handle)
	{
	
		m_Capabilities.ContextVersion = (const char*)glGetString(GL_VERSION);
		m_Capabilities.Vendor = (const char*)glGetString(GL_VENDOR);
		m_Capabilities.Renderer = (const char*)glGetString(GL_RENDERER);
		m_Capabilities.ShaderVersion = (const char*)glGetString(GL_SHADING_LANGUAGE_VERSION);

		glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, (GLint*)&m_Capabilities.MaxTextureUnits);

		LOG_CORE_TRACE("OpenGL context");
		LOG_CORE_TRACE("  Version           : {0}", m_Capabilities.ContextVersion.c_str());
		LOG_CORE_TRACE("  Vendor            : {0}", m_Capabilities.Vendor.c_str());
		LOG_CORE_TRACE("  Renderer          : {0}", m_Capabilities.Renderer.c_str());
		LOG_CORE_TRACE("  Shader version    : {0}", m_Capabilities.ShaderVersion.c_str());
		LOG_CORE_TRACE("  Max texture count : {0}", m_Capabilities.MaxTextureUnits);

		m_Parameters.Wireframe = false;


		SubscribeToEvent<WindowResizedEvent>([](WindowResizedEvent& event)
			{
				RenderThread::Submit([=]()
					{
						glViewport(0, 0, event.Width(), event.Height());
					});

				return false;
			});
	}

	OpenGLRenderContext::~OpenGLRenderContext()
	{

	}

	void OpenGLRenderContext::Bind()
	{
		#ifdef _WIN32
		wglMakeCurrent(GetDC(static_cast<HWND>(Application::Get().GetWindow()->GetNativeHandle())), static_cast<HGLRC>(m_ContextHandle));
		#elif __linux__

		#endif

	}

	void OpenGLRenderContext::Unbind()
	{
#ifdef _WIN32
		wglMakeCurrent(NULL, NULL);
#elif __linux__

#endif
	}

}
