#include "nutpch.h"
#include "OpenGLShader.h"

#include "NutLib/Renderer/RenderThread.h"

#include "NutLib/Tools/StringHelpers.h"


#include <glad/glad.h>

#include <glm/gtc/type_ptr.hpp>


namespace Nut
{

	ShaderLayoutItem::ShaderSlot OpenGLLayoutNameToShaderSlot(const std::string& name)
	{
		if (name == "a_Position") return ShaderLayoutItem::ShaderSlot::Vertex;
		if (name == "a_TexCoord") return ShaderLayoutItem::ShaderSlot::TexCoord;
		if (name == "a_Normal") return ShaderLayoutItem::ShaderSlot::Normal;
		if (name == "a_Color") return ShaderLayoutItem::ShaderSlot::Color;
		if (name == "a_InstanceMatrix") return ShaderLayoutItem::ShaderSlot::InstanceMatrix;

		return ShaderLayoutItem::ShaderSlot::Unknown;
	}

	OpenGLShader::OpenGLShader(const std::string& shaderFile)
		: m_ShaderPath(shaderFile)
	{
		Reload();
	}

	OpenGLShader::~OpenGLShader()
	{
		LOG_CORE_TRACE("Shader destructor");

		ReleaseShader();

		if (m_ShaderIDs)
			delete[] m_ShaderIDs;

	}

	void OpenGLShader::ReleaseShader()
	{
		if (m_ShaderIDs)
		{
			for (auto it = m_ShaderIDs->begin(); it != m_ShaderIDs->end(); it++)
			{
				RendererID id = (*it);

				RenderThread::Submit([=]()
					{
						glDeleteShader(id);
					});
			}

			m_ShaderIDs->clear();
		}

		if (m_ID)
		{
			RenderThread::Submit([=]()
				{
					glDeleteProgram(m_ID);
				});

			m_ID = 0;
		}

	}

	void OpenGLShader::Reload()
	{
//		if (m_MaterialDescriptors.size() > 0)
//			m_MaterialDescriptors.clear();

		m_ShaderMaterialLayout.Clear();
		m_ShaderLayout.Clear();

		m_ShaderSources = GetShaderSourcesFromFile(m_ShaderPath);

		if (m_ID)
		{
			ReleaseShader();
		}

		RenderThread::Submit([&]()
			{
				m_ID = glCreateProgram();

				m_ShaderIDs = new std::vector<RendererID>();
			});

		for (auto& shaderSource : m_ShaderSources)
		{
			Reflect(shaderSource.second);
			Compile(shaderSource.first, shaderSource.second);
		}

		if (!m_ShaderSources[ShaderType::Vertex].empty())
		{
			ReflectLayout(m_ShaderSources[ShaderType::Vertex]);
		}

		LinkProgram();

		ResolveLocations();
	}

	void OpenGLShader::Bind() const
	{
		RenderThread::Submit([=]()
			{
				glUseProgram(m_ID);
			});
	}

	void OpenGLShader::Unbind() const
	{
		RenderThread::Submit([=]()
			{
				glUseProgram(0);
			});
	}


	void OpenGLShader::Reflect(const std::string& source)
	{
		// Find uniforms and resolve the locations

		size_t pos = 0;

		while ((pos = source.find("uniform", pos)) != std::string::npos)
		{
			std::string line;

			size_t endpos = source.find_first_of(';', pos);

			line = source.substr(pos, endpos - pos);

			auto tokens = Tokenize(line, " ");

			if (tokens[0] == "uniform")
			{
				std::string type = tokens[1];
				std::string name = tokens[2];

				ShaderMaterialItem item;
				item.Name = name;
				item.Type = StringToDataType(type);

				NUT_CORE_ASSERT((item.Type != DataType::Unknown), "GLSL syntax error, type is not valid!");

				item.Size = DataTypeSize(item.Type);

				m_ShaderMaterialLayout.Items().emplace_back(item);
			}

			pos = endpos;
		}

	}

	void OpenGLShader::ReflectLayout(const std::string& source)
	{
		size_t pos = 0;

		while ((pos = source.find("layout", pos)) != std::string::npos)
		{
			ShaderLayoutItem shaderItem;

			std::string line;

			size_t endpos = source.find_first_of(';', pos);

			line = source.substr(pos, endpos - pos);

			auto tokens = Tokenize(line, " =()");

			size_t i = 0;

			for (auto& token : tokens)
			{
				if (token == "location")
				{
					size_t sublocation = i;
					while (tokens[++sublocation] == "")
					{

					}

					shaderItem.Location = std::stoi(tokens[sublocation]);
				}

				if (token == "in")
				{
					shaderItem.Type = StringToDataType(tokens[i + 1]);
					shaderItem.Name = tokens[i + 2];
					break;
				}

				i++;
			}

			shaderItem.Slot = OpenGLLayoutNameToShaderSlot(shaderItem.Name);
			m_ShaderLayout.Items().emplace_back(shaderItem);

			pos = endpos;
		}

		m_ShaderLayout.UpdateOffsets();
	}

	void OpenGLShader::ResolveLocations()
	{
		for (auto& item : m_ShaderMaterialLayout.Items())
		{
			RenderThread::Submit([&]()
				{
					item.Location = glGetUniformLocation(m_ID, item.Name.c_str());
					item.Resolved = true;
				});

		}

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

		RenderThread::Submit([=, &shaderIDs]()
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
		RenderThread::Submit([=]()
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

	int32_t OpenGLShader::GetLocation(const std::string& name)
	{
		for (auto& item : m_ShaderMaterialLayout.Items())
		{
			if ((item.Name == name) && (item.Location != -1))
			{
				return item.Location;
			}
		}

		LOG_CORE_TRACE("Shader: uniform {0} not found or not used", name.c_str());

		return -1;
	}

	void OpenGLShader::Set(const std::string& name, int32_t value)
	{
		int32_t location = GetLocation(name);

		RenderThread::Submit([=]()
			{
				glUniform1i(location, value);
			});
	}

	void OpenGLShader::Set(const std::string& name, float value)
	{
		int32_t location = GetLocation(name);

		RenderThread::Submit([=]()
			{
				glUniform1f(location, value);
			});
	}

	void OpenGLShader::Set(const std::string& name, const glm::vec2& value)
	{
		int32_t location = GetLocation(name);

		RenderThread::Submit([=]()
			{
				glUniform2f(location, value.x, value.y);
			});
	}

	void OpenGLShader::Set(const std::string& name, const glm::vec3& value)
	{
		int32_t location = GetLocation(name);

		RenderThread::Submit([=]()
			{
				glUniform3f(location, value.x, value.y, value.z);
			});
	}

	void OpenGLShader::Set(const std::string& name, const glm::vec4& value)
	{
		int32_t location = GetLocation(name);

		RenderThread::Submit([=]()
			{
				glUniform4f(location, value.x, value.y, value.z, value.w);
			});
	}

	void OpenGLShader::Set(const std::string& name, const glm::mat4& matrix)
	{
		int32_t location = GetLocation(name);

		RenderThread::Submit([=]()
			{
				glUniformMatrix4fv(location, 1, false, glm::value_ptr(matrix));
			});
	}

}
