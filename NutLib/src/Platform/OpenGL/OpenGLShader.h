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

		virtual void Set(const std::string& name, int32_t value) override;
		virtual void Set(const std::string& name, float value) override;
		virtual void Set(const std::string& name, const glm::vec2& value) override;
		virtual void Set(const std::string& name, const glm::vec3& value) override;
		virtual void Set(const std::string& name, const glm::vec4& value) override;
		virtual void Set(const std::string& name, const glm::mat4& matrix) override;

//		virtual std::vector<ShaderMaterialDescriptor>& GetShaderDescriptors() override { return m_MaterialDescriptors; }
//		virtual std::unordered_map<ShaderLayoutDescriptor::Slot, ShaderLayoutDescriptor>& GetShaderLayout() override { return m_LayoutDescriptors; }
		virtual DataBufferLayout<ShaderMaterialItem>& GetShaderMaterialLayout() override { return m_ShaderMaterialLayout; }
		virtual DataBufferLayout<ShaderLayoutItem>& GetShaderLayout() override { return m_ShaderLayout; }


	private:
		void Reflect(const std::string& source);
		void ReflectLayout(const std::string& source);
		void Compile(ShaderType domain, const std::string& source);
		void LinkProgram();

		void ResolveLocations();

		void ReleaseShader();

		int32_t GetLocation(const std::string& name);

	private:
		std::unordered_map<ShaderType, std::string> m_ShaderSources;

		RendererID m_ID = 0;

		std::string m_ShaderPath;

		std::vector<RendererID>* m_ShaderIDs = nullptr;


//		std::vector<ShaderMaterialDescriptor> m_MaterialDescriptors;
		DataBufferLayout<ShaderMaterialItem> m_ShaderMaterialLayout;
//		std::unordered_map<ShaderLayoutDescriptor::Slot, ShaderLayoutDescriptor> m_LayoutDescriptors;
		DataBufferLayout<ShaderLayoutItem> m_ShaderLayout;
	};


}


#endif
