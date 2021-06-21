#include "nutpch.h"
#include "OpenGLRenderContext.h"

#include <glad/glad.h>

namespace Nut
{

	OpenGLRenderContext::OpenGLRenderContext()
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
	}

	OpenGLRenderContext::~OpenGLRenderContext()
	{

	}


}
