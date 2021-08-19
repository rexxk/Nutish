#ifndef __NUTLIB_OPENGL_SHADER_H
#define __NUTLIB_OPENGL_SHADER_H


#include "NutLib/Renderer/Shader.h"

#include "NutLib/Tools/ShaderFile.h"


namespace Nut
{

	class OpenGLShader : public Shader
	{
	public:
		OpenGLShader(const std::string& shaderFile);
		virtual ~OpenGLShader();

		virtual void Reload() override;

		virtual RendererID ID() const override { return m_ID; }

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void SetInt(const std::string& name, int32_t value) override;
		virtual void SetFloat4(const std::string& name, float x, float y, float z, float w) override;
		virtual void SetMatrix4(const std::string& name, float* values) override;

		virtual std::vector<ShaderMaterialDescriptor>& GetShaderDescriptors() override { return m_MaterialDescriptors; }

	private:
		void Reflect(const std::string& source);
		void Compile(ShaderType domain, const std::string& source);
		void LinkProgram();

		void ResolveLocations();

		void ReleaseShader();

	private:
		std::unordered_map<ShaderType, std::string> m_ShaderSources;

		RendererID m_ID = 0;

		std::string m_ShaderPath;

		std::vector<RendererID>* m_ShaderIDs = nullptr;


		std::vector<ShaderMaterialDescriptor> m_MaterialDescriptors;
	};


}


#endif
