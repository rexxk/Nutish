#include "nutpch.h"
#include "Shader.h"

#include "Renderer.h"

#include "Platform/OpenGL/OpenGLShader.h"


namespace Nut
{


	Ref<Shader> Shader::Create(const std::string& shaderFile)
	{
		switch (Renderer::API())
		{
		case RendererAPIType::OpenGL: return CreateRef<OpenGLShader>(shaderFile);
		}

		LOG_CORE_ERROR("Shader: No API defined.");

		return nullptr;
	}


	ShaderMaterialDescriptor::Type ShaderMaterialStringToType(const std::string& type)
	{
		if (type == "bool") return ShaderMaterialDescriptor::Type::Bool;
		if (type == "int") return ShaderMaterialDescriptor::Type::Int;
		if (type == "uint") return ShaderMaterialDescriptor::Type::UInt;
		if (type == "float") return ShaderMaterialDescriptor::Type::Float;
		if (type == "double") return ShaderMaterialDescriptor::Type::Double;
		if (type == "vec2") return ShaderMaterialDescriptor::Type::Vec2;
		if (type == "vec3") return ShaderMaterialDescriptor::Type::Vec3;
		if (type == "vec4") return ShaderMaterialDescriptor::Type::Vec4;
		if (type == "matrix3x3") return ShaderMaterialDescriptor::Type::Matrix3x3;
		if (type == "matrix4x4") return ShaderMaterialDescriptor::Type::Matrix4x4;
		if (type == "texture2D") return ShaderMaterialDescriptor::Type::Texture2D;
		if (type == "textureCube") return ShaderMaterialDescriptor::Type::TextureCube;
		if (type == "textureSphere") return ShaderMaterialDescriptor::Type::TextureSphere;

		return ShaderMaterialDescriptor::Type::Unknown;
	}

	std::string ShaderMaterialTypeToString(const ShaderMaterialDescriptor::Type type)
	{
		switch (type)
		{
			case ShaderMaterialDescriptor::Type::Bool: return "bool";
			case ShaderMaterialDescriptor::Type::Int: return "int";
			case ShaderMaterialDescriptor::Type::UInt: return "uint";
			case ShaderMaterialDescriptor::Type::Float: return "float";
			case ShaderMaterialDescriptor::Type::Double: return "double";
			case ShaderMaterialDescriptor::Type::Vec2: return "vec2";
			case ShaderMaterialDescriptor::Type::Vec3: return "vec3";
			case ShaderMaterialDescriptor::Type::Vec4: return "vec4";
			case ShaderMaterialDescriptor::Type::Matrix3x3: return "matrix3x3";
			case ShaderMaterialDescriptor::Type::Matrix4x4: return "matrix4x4";
			case ShaderMaterialDescriptor::Type::Texture2D: return "texture2D";
			case ShaderMaterialDescriptor::Type::TextureCube: return "textureCube";
			case ShaderMaterialDescriptor::Type::TextureSphere: return "textureSphere";
		}

		return "<unknown>";
	}


	enum class Type
	{
		Unknown = 0,
		Bool,
		Int,
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



}
