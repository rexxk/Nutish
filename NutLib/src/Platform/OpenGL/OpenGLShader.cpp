#include "nutpch.h"
#include "OpenGLShader.h"

#include "NutLib/Renderer/RenderCommandQueue.h"

#include "NutLib/Tools/StringHelpers.h"


#include <glad/glad.h>


namespace Nut
{

	ShaderMaterialDescriptor::Type OpenGLUniformToDescriptorType(const std::string& type)
	{
		if (type == "bool") return ShaderMaterialDescriptor::Type::Bool;
		if (type == "int") return ShaderMaterialDescriptor::Type::Int;
		if (type == "uint") return ShaderMaterialDescriptor::Type::UInt;
		if (type == "float") return ShaderMaterialDescriptor::Type::Float;
		if (type == "double") return ShaderMaterialDescriptor::Type::Double;
		if (type == "vec2") return ShaderMaterialDescriptor::Type::Vec2;
		if (type == "vec3") return ShaderMaterialDescriptor::Type::Vec3;
		if (type == "vec4") return ShaderMaterialDescriptor::Type::Vec4;
		if (type == "mat3") return ShaderMaterialDescriptor::Type::Matrix3x3;
		if (type == "mat4") return ShaderMaterialDescriptor::Type::Matrix4x4;
		if (type == "sampler2D") return ShaderMaterialDescriptor::Type::Texture2D;
		if (type == "samplerCube") return ShaderMaterialDescriptor::Type::TextureCube;
//		if (type == "textureSphere") return ShaderMaterialDescriptor::Type::TextureSphere;

		return ShaderMaterialDescriptor::Type::Unknown;
	}

	uint32_t OpenGLUniformSize(ShaderMaterialDescriptor::Type type)
	{
		switch (type)
		{
			case ShaderMaterialDescriptor::Type::Bool: return 1;
			case ShaderMaterialDescriptor::Type::Int: return 4;
			case ShaderMaterialDescriptor::Type::UInt: return 4;
			case ShaderMaterialDescriptor::Type::Float: return 4;
			case ShaderMaterialDescriptor::Type::Double: return 4;
			case ShaderMaterialDescriptor::Type::Vec2: return 4 * 2;
			case ShaderMaterialDescriptor::Type::Vec3: return 4 * 3;
			case ShaderMaterialDescriptor::Type::Vec4: return 4 * 4;
			case ShaderMaterialDescriptor::Type::Matrix3x3: return 4 * 3 * 3;
			case ShaderMaterialDescriptor::Type::Matrix4x4: return 4 * 4 * 4;
			case ShaderMaterialDescriptor::Type::Texture2D: return 4;
			case ShaderMaterialDescriptor::Type::TextureCube: return 4;
		}

		return 0;
	}


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
		if (m_MaterialDescriptors.size() > 0)
			m_MaterialDescriptors.clear();

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

		ResolveLocations();
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
//		if (m_MaterialDescriptors.size() > 0)
//			m_MaterialDescriptors.clear();

//		LOG_CORE_TRACE("Shader reflect");

		// Find uniforms and resolve the locations

		size_t pos = 0;

		while ((pos = source.find("uniform", pos)) != std::string::npos)
		{
			std::string line;

			size_t endpos = source.find_first_of(';', pos);

			line = source.substr(pos, endpos - pos);

			auto tokens = Tokenize(line, ' ');

			if (tokens[0] == "uniform")
			{
				std::string type = tokens[1];
				std::string name = tokens[2];

				ShaderMaterialDescriptor desc;
				desc.Name = name;
				desc.DescriptorType = OpenGLUniformToDescriptorType(type);

				NUT_CORE_ASSERT((desc.DescriptorType != ShaderMaterialDescriptor::Type::Unknown), "GLSL syntax error, type is not valid!");

				desc.Size = OpenGLUniformSize(desc.DescriptorType);

				m_MaterialDescriptors.emplace_back(desc);
			}

			pos = endpos;
		}

	}

	void OpenGLShader::ResolveLocations()
	{
		for (auto& desc : m_MaterialDescriptors)
		{
			RenderCommandQueue::Submit([&]()
				{
					desc.Location = glGetUniformLocation(m_ID, desc.Name.c_str());
					desc.Resolved = true;
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


	void OpenGLShader::SetInt(const std::string& name, int32_t value)
	{
		for (auto& desc : m_MaterialDescriptors)
		{
			if ((desc.Name == name) && (desc.Location != -1))
			{
				RenderCommandQueue::Submit([=]()
					{
						glUniform1i(desc.Location, value);
					});

				return;
			}
		}

		LOG_CORE_TRACE("Shader: uniform {0} not found", name.c_str());
	}

	void OpenGLShader::SetFloat4(const std::string& name, float x, float y, float z, float w)
	{
		for (auto& desc : m_MaterialDescriptors)
		{
			if ((desc.Name == name) && (desc.Location != -1))
			{
				RenderCommandQueue::Submit([=]()
					{
						glUniform4f(desc.Location, x, y, z, w);
					});

				return;
			}
		}

		LOG_CORE_TRACE("Shader: uniform {0} not found", name.c_str());
	}

	void OpenGLShader::SetMatrix4(const std::string& name, float* values)
	{
		float matrixValues[16];
		memcpy(matrixValues, values, sizeof(float) * 16);

		for (auto& desc : m_MaterialDescriptors)
		{
			if ((desc.Name == name) && (desc.Location != -1))
			{
				RenderCommandQueue::Submit([=]()
					{
						glUniformMatrix4fv(desc.Location, 1, false, matrixValues);
					});

				return;
			}
		}

		LOG_CORE_TRACE("Shader: uniform {0} not found", name.c_str());
	}

}
