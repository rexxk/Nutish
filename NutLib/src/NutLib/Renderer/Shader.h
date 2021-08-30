#ifndef __NUTLIB_RENDERER_SHADER_H
#define __NUTLIB_RENDERER_SHADER_H


#include <glm/glm.hpp>


namespace Nut
{



	class ShaderMaterialDescriptor
	{
	public:

		enum class Type
		{
			Unknown = 0,
			Bool,
			Int,
			UInt,
			Float,
			Double,
			Vec2,
			Vec3,
			Vec4,
			Matrix3x3,
			Matrix4x4,
			Texture2D,
			TextureCube,
			TextureSphere,
		};

		int32_t Location = 0;
		uint32_t Size = 0;

		Type DescriptorType = Type::Unknown;
		std::string Name = "<unknown>";

		bool Resolved = false;

	};



	class Shader
	{
	public:
		static Ref<Shader> Create(const std::string& shaderFile);

		virtual ~Shader() {}

		virtual void Reload() = 0;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual RendererID ID() const = 0;

		virtual void Set(const std::string& name, int32_t value) = 0;
		virtual void Set(const std::string& name, float value) = 0;
		virtual void Set(const std::string& name, const glm::vec2& value) = 0;
		virtual void Set(const std::string& name, const glm::vec3& value) = 0;
		virtual void Set(const std::string& name, const glm::vec4& value) = 0;
		virtual void Set(const std::string& name, const glm::mat4& matrix) = 0;

//		virtual void SetInt(const std::string& name, int32_t value) = 0;
//		virtual void SetFloat4(const std::string& name, float x, float y, float z, float w) = 0;
//		virtual void SetMatrix4(const std::string& name, float* values) = 0;

		virtual std::vector<ShaderMaterialDescriptor>& GetShaderDescriptors() = 0;

	};


}


#endif
