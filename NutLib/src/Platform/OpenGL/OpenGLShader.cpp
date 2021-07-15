#include "nutpch.h"
#include "OpenGLShader.h"

#include "NutLib/Renderer/RenderCommandQueue.h"

#include <glad/glad.h>


namespace Nut
{

	OpenGLShader::OpenGLShader(const std::string& shaderFile)
		: m_ShaderPath(shaderFile)
	{
		Reload();
	}

	OpenGLShader::~OpenGLShader()
	{
		ReleaseShader();

		if (m_ShaderIDs)
			delete m_ShaderIDs;
	}

	void OpenGLShader::ReleaseShader()
	{
		if (m_ShaderIDs)
		{
			for (auto it = m_ShaderIDs->begin(); it != m_ShaderIDs->end(); it++)
			{
				RendererID id = (*it);

				RenderCommandQueue::Submit([=]()
					{
						glDeleteShader(id);
					});
			}

			m_ShaderIDs->clear();
		}

		if (m_ID)
		{
			RenderCommandQueue::Submit([=]()
				{
					glDeleteProgram(m_ID);
				});

			m_ID = 0;
		}

	}

	void OpenGLShader::Reload()
	{
		m_ShaderSources = GetShaderSourcesFromFile(m_ShaderPath);

		if (m_ID)
		{
			ReleaseShader();
		}

		RenderCommandQueue::Submit([&]()
			{
				m_ID = glCreateProgram();

				m_ShaderIDs = new std::vector<RendererID>();
			});

		for (auto& shaderSource : m_ShaderSources)
		{
			Reflect(shaderSource.second);
			Compile(shaderSource.first, shaderSource.second);
		}

		LinkProgram();
	}

	void OpenGLShader::Bind() const
	{
		RenderCommandQueue::Submit([=]()
			{
				glUseProgram(m_ID);
			});
	}

	void OpenGLShader::Unbind() const
	{
		RenderCommandQueue::Submit([=]()
			{
				glUseProgram(0);
			});
	}


	void OpenGLShader::Reflect(const std::string& source)
	{

	}

	void OpenGLShader::Compile(ShaderType domain, const std::string& source)
	{
		GLenum shaderDomain = 0;

		switch (domain)
		{
			case ShaderType::Vertex: shaderDomain = GL_VERTEX_SHADER; break;
			case ShaderType::Pixel:
			case ShaderType::Fragment: shaderDomain = GL_FRAGMENT_SHADER; break;
			case ShaderType::Geometry: shaderDomain = GL_GEOMETRY_SHADER; break;
			case ShaderType::Compute: shaderDomain = GL_COMPUTE_SHADER; break;
		}

		auto& shaderIDs = m_ShaderIDs;

		RenderCommandQueue::Submit([=, &shaderIDs]()
			{
				const char* glSource = source.data();
				GLint length = (GLint)source.length();

				uint32_t shaderID = glCreateShader(shaderDomain);
				glShaderSource(shaderID, 1, &glSource, &length);
				glCompileShader(shaderID);

				if (domain != ShaderType::Compute)
				{
					int32_t status = 0;
					glGetShaderiv(shaderID, GL_COMPILE_STATUS, &status);

					if (status != GL_TRUE)
					{
						int32_t infoLogLength;
						glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &infoLogLength);

						std::vector<GLchar> infoLog(infoLogLength);
						glGetShaderInfoLog(shaderID, infoLogLength, &infoLogLength, infoLog.data());

						LOG_CORE_ERROR("Failed to compile shader: {0}", infoLog.data());
					}
				}

				glAttachShader(m_ID, shaderID);
				shaderIDs->push_back(shaderID);

			});

	}

	void OpenGLShader::LinkProgram()
	{
		RenderCommandQueue::Submit([=]()
			{
				glLinkProgram(m_ID);

				int32_t status = 0;
				glGetProgramiv(m_ID, GL_LINK_STATUS, &status);

				if (status != GL_TRUE)
				{
					int32_t infoLogLength;
					glGetProgramiv(m_ID, GL_INFO_LOG_LENGTH, &infoLogLength);

					std::vector<GLchar> infoLog(infoLogLength);
					glGetProgramInfoLog(m_ID, infoLogLength, &infoLogLength, infoLog.data());

					LOG_CORE_ERROR("Failed to link shader: {0}", infoLog.data());

					m_ID = 0;
				}
			});

	}

}
