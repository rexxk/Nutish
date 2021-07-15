#ifndef __NUTLIB_RENDERER_SHADER_H
#define __NUTLIB_RENDERER_SHADER_H



namespace Nut
{


	class Shader
	{
	public:
		static Ref<Shader> Create(const std::string& shaderFile);

		virtual ~Shader() {}

		virtual void Reload(const std::string& shaderFile) = 0;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual RendererID ID() const = 0;

	};


}


#endif
