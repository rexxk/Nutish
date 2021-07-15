#pragma once

#include "NutLib/Renderer/Shader.h"

#include "NutLib/Tools/ShaderFile.h"


namespace Nut
{

	class OpenGLShader : public Shader
	{
	public:
		OpenGLShader(const std::string& shaderFile);
		virtual ~OpenGLShader();

		virtual void Reload(const std::string& shaderFile) override;

		virtual RendererID ID() const override { return m_ID; }

		virtual void Bind() const override;
		virtual void Unbind() const override;

	private:
		void Reflect(const std::string& source);
		void Compile(ShaderType domain, const std::string& source);
		void LinkProgram();

		void ReleaseShader();

	private:
		std::unordered_map<ShaderType, std::string> m_ShaderSources;

		RendererID m_ID = 0;

		std::vector<RendererID>* m_ShaderIDs = nullptr;
	};


}
